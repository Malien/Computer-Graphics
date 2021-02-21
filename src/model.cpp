#include "model.hpp"
#include "result.hpp"
#include "util.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

Result<Model, IOError> Model::fromObjectFile(const std::string_view filename) {
    Model model;

    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail())
        return Result<Model, IOError>::error(IOError{});
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            cv::Vec3f v;
            for (int i = 0; i < 3; i++)
                iss >> v[i];
            model.verts.push_back(v);
        } else if (!line.compare(0, 2, "f ")) {
            std::vector<int> f;
            int itrash, idx;
            iss >> trash;
            while (iss >> idx >> trash >> itrash >> trash >> itrash) {
                idx--; /// in wavefront obj all indices start at 1, not zero
                f.push_back(idx);
            }
            model.faces.push_back(f);
        }
    }

    return Result<Model, IOError>::ok(model);
}

VertexBuffer Model::vertexBuffer() const {
    VertexBuffer vb(faces.size());

    std::transform(
        begin(faces),
        end(faces),
        begin(vb),
        [this](const std::vector<int> &face) {
            return triangleForFaceUnprojected(face);
        });

    return vb;
}

TrianglePolygon Model::triangleForFaceUnprojected(const std::vector<int> &face) const noexcept {
    TrianglePolygon polygon;
    std::transform(
        begin(face),
        end(face),
        begin(polygon),
        [this](const int vertexID) {
            return verts[vertexID];
        });
    return polygon;
}


std::array<cv::Point, 3> triangleForFace(const Model &model, const std::vector<int> &face, const cv::Size size) {
    std::array<cv::Point, 3> polygon;
    std::transform(
        begin(face),
        end(face),
        polygon.begin(),
        [&model, size](const int vertexID) {
            return static_cast<cv::Point>(project2d(model.verts[vertexID], size));
        });
    return polygon;
}
