#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

#include "result.hpp"

struct Model {
    std::vector<cv::Vec3f> verts;
    std::vector<std::vector<int>> faces;

    static Result<Model, IOError> fromObjectFile(const std::string_view filename);
};
