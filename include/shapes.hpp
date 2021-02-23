#pragma once
#include <opencv2/core/mat.hpp>

#include "color_range.hpp"
#include "model.hpp"
#include "util.hpp"

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
    const TrianglePolygon2d triangle,
    const cv::Vec3b color = { 255, 255, 255 }
);

template <class T>
cv::Mat mandelbrot(const cv::Size& size, const cv::Rect2d& bounds, const uint max_iterations, const ColorRange<T>& colorRange) {
    cv::Mat img {size, CV_8UC3};
    for (size_t i = 0; i < size.height; ++i) {
        for (size_t j = 0; j < size.width; ++j) {
            const double x0 = rescale_screen_coords(j, size.width, bounds.x, bounds.x + bounds.width);
            const double y0 = rescale_screen_coords(i, size.width, bounds.y, bounds.y + bounds.height);
            double x = 0;
            double y = 0;
            uint iteration = 0;
            while (x*x + y*y <= 2*2 && iteration < max_iterations) {
                double xtemp = x*x - y*y + x0;
                y = 2*x*y + y0;
                x = xtemp;
                iteration++;
            }
            img.at<cv::Vec3b>(i, j) = colorRange.pickValue(1.0 * iteration / max_iterations);
        }
    }

    return img;
}
