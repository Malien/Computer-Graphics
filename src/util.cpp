#include "util.hpp"

cv::Vec2f project2d(const cv::Vec3f vertex, const cv::Size canvasSize) {
    const auto [width, height] = canvasSize;
    return {(vertex[0] + 1.f) * width / 2.f, height - (vertex[1] + 1.f) * height / 2.f};
}

double length(const cv::Vec2i &vec) { return sqrt(vec[0] * vec[0] + vec[1] * vec[1]); }

cv::Vec2d normalize(const cv::Vec2i &original) {
    const double len = length(original);
    return cv::Vec2d(original[0] / len, original[1] / len);
}

std::array<cv::Point, 3> triangleForFace(const Model& model, const std::vector<int>& face, const cv::Size size) {
    std::array<cv::Point, 3> polygon;
    std::transform(
        begin(face),
        end(face),
        polygon.begin(),
        [&model, size](const int vertexID) {
            return static_cast<cv::Point>(project2d(model.verts[vertexID], size));
        }
    );
    return polygon;
}

cv::Vec3b randomColor() {
    const int color = rand();
    const uchar r = color;
    const uchar g = color >> 8;
    const uchar b = color >> 16;
    return {r, g, b};
}
