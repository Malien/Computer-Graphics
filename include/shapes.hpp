#pragma once
#include "util.hpp"
#include <opencv2/core/mat.hpp>

void drawLine(
    cv::Mat &image,
    const cv::Point from,
    const cv::Point to,
    const cv::Vec3b color = { 255, 255, 255 }
);

void drawCircle(
    cv::Mat &image,
    const cv::Point center,
    const uint radius,
    const cv::Vec3b color = { 255, 255, 255 }
);

void drawTriangle(
    cv::Mat &image,
    const TrianglePolygon triangle,
    const cv::Vec3b color = { 255, 255, 255 }
);