#pragma once
#include <opencv2/imgproc.hpp>

#include "shapes.hpp"
#include "util.hpp"
#include "model.hpp"

namespace builtin {
    cv::Mat wireframeImageOf(const Model &model);
    cv::Mat shadedImageOf(const Model &model);
}

namespace homemade {
    cv::Mat linesSampleImage();
    cv::Mat trianglesSampleImage();
    cv::Mat wireframeImageOf(const Model &model);
    cv::Mat shadedImageOf(const Model &model);
}