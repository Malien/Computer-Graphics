#include "util.hpp"

cv::Vec2f project2d(const cv::Vec3f vertex, const cv::Size canvasSize) {
    const auto [width, height] = canvasSize;
    return {(vertex[0] + 1.f) * width / 2.f, height - (vertex[1] + 1.f) * height / 2.f};
}

TrianglePolygon2d project2d(const TrianglePolygon& polygon, const cv::Size canvasSize) {
    const auto& [a, b, c] = polygon;
    return {
        cv::Point(project2d(a, canvasSize)),
        cv::Point(project2d(b, canvasSize)),
        cv::Point(project2d(c, canvasSize))
    };
}

double length(const cv::Vec2i &vec) { return sqrt(vec[0] * vec[0] + vec[1] * vec[1]); }

cv::Vec2d normalize(const cv::Vec2i &original) {
    const double len = length(original);
    return cv::Vec2d(original[0] / len, original[1] / len);
}

cv::Vec3b randomColor() {
    const int color = rand();
    const uchar r = color;
    const uchar g = color >> 8;
    const uchar b = color >> 16;
    return {r, g, b};
}
