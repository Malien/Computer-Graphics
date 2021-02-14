#pragma once
#include <array>
#include <opencv2/core/mat.hpp>

#include "model.hpp"

typedef std::array<cv::Point, 3> TrianglePolygon;

cv::Vec2f project2d(const cv::Vec3f vertex, const cv::Size canvasSize);

double length(const cv::Vec2i &vec);

cv::Vec2d normalize(const cv::Vec2i &original);

TrianglePolygon triangleForFace(const Model& model, const std::vector<int>& face, const cv::Size size);

cv::Vec3b randomColor();

template<class T>
T rescale_into_range(T x, T from, T to) {
    return (to - from) * x + from;
}

inline double rescale_screen_coords(int x, int original_max, double scaled_min, double scaled_max) {
    return rescale_into_range(1.0 * x / original_max, scaled_min, scaled_max);
}
