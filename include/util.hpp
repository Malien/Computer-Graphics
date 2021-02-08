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
