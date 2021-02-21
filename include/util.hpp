#pragma once
#include <array>
#include <opencv2/core/mat.hpp>

typedef std::array<cv::Point, 3> TrianglePolygon2d;
typedef std::array<cv::Vec3f, 3> TrianglePolygon;

typedef std::vector<TrianglePolygon> VertexBuffer;

cv::Vec2f project2d(const cv::Vec3f vertex, const cv::Size canvasSize);

TrianglePolygon2d project2d(const TrianglePolygon&, const cv::Size canvasSize);

double length(const cv::Vec2i &vec);

cv::Vec2d normalize(const cv::Vec2i &original);

cv::Vec3b randomColor();

template<class T>
T rescale_into_range(T x, T from, T to) {
    return (to - from) * x + from;
}

inline double rescale_screen_coords(int x, int original_max, double scaled_min, double scaled_max) {
    return rescale_into_range(1.0 * x / original_max, scaled_min, scaled_max);
}

cv::Matx<float, 3, 3> xRotationMatrix(double angle);
cv::Matx<float, 3, 3> yRotationMatrix(double angle);
cv::Matx<float, 3, 3> zRotationMatrix(double angle);
