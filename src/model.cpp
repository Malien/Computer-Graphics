#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.hpp"
#include "result.hpp"

Result<Model, IOError> Model::fromObjectFile(const std::string_view filename) {
    Model model;

    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if (in.fail()) return Result<Model, IOError>::error(IOError {});
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            cv::Vec3f v;
            for (int i=0;i<3;i++) iss >> v[i];
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
