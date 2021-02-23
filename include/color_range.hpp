#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

template<class T>
struct linearInterpolation {
    T operator()(double percentage, T min, T max) const noexcept {
        return min + (max - min) * percentage;
    }
};

struct ColorStop {
    double keypoint;
    cv::Vec3b color;
};

template <class InterpolationFn = linearInterpolation<uchar>>
struct ColorRange {
    InterpolationFn interpolate = InterpolationFn();
    std::vector<ColorStop> stopList;

    ColorRange(cv::Vec3b from, cv::Vec3b to, InterpolationFn interpolation = {}): 
        stopList{{0, from}, {1, to}},
        interpolate{interpolation} {}

    ColorRange(std::initializer_list<ColorStop> initList, InterpolationFn interpolation = {}):
        stopList{initList},
        interpolate{interpolation} 
    {
        assert(initList.size() > 0);
    }

    ColorRange(const ColorRange&) = default;
    ColorRange(ColorRange&&) = default;
    ColorRange& operator=(const ColorRange&) = default;
    ColorRange& operator=(ColorRange&&) = default;

    std::pair<ColorStop, ColorStop> closestRange(double percentage) const {
        for (size_t i = 0; i < stopList.size(); ++i) {
            const auto& colorStop = stopList[i];
            if (percentage <= colorStop.keypoint) {
                if (i == 0) {
                    return { { 0, colorStop.color }, { 1, colorStop.color } };
                }
                const auto& prevStop = stopList[i - 1];
                return { prevStop, colorStop };
            }
        }
        const auto& [_, color] = stopList.at(stopList.size() - 1);
        return { { 0, color }, { 1, color } };
    }

    cv::Vec3b pickValue(double percentage) const {
        const auto& [from, to] = closestRange(percentage);
        // const double innerPercentage = rescale_into_range(percentage, from.keypoint, to.keypoint);
        const double innerPercentage = (percentage - from.keypoint) / (to.keypoint - from.keypoint);
        return { 
            interpolate(innerPercentage, from.color[0], to.color[0]),
            interpolate(innerPercentage, from.color[1], to.color[1]),
            interpolate(innerPercentage, from.color[2], to.color[2])
        };
    }
};

typedef ColorRange<linearInterpolation<uchar>> LinearColorRange;

template <class T>
void drawColorGradient(cv::Mat image, const ColorRange<T>& gradient, const cv::Rect& bounds) {
    for (size_t i = bounds.y; i < bounds.y + bounds.height; ++i) {
        const double percentage = (1.0 * i - bounds.y) / bounds.height;
        const cv::Vec3b color = gradient.pickValue(percentage);
        for (size_t j = bounds.x; j < bounds.x + bounds.width; ++j) {
            image.at<cv::Vec3b>(i, j) = color;
        }
    }
}
