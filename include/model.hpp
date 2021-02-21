#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/core/cvdef.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

#include "result.hpp"
#include "util.hpp"

struct Model {
    std::vector<cv::Vec3f> verts;
    std::vector<std::vector<int>> faces;

    [[nodiscard]] static Result<Model, IOError> fromObjectFile(const std::string_view filename);
    [[nodiscard]] VertexBuffer vertexBuffer() const;

    TrianglePolygon triangleForFaceUnprojected(const std::vector<int> &face) const noexcept;
};

[[nodiscard]] TrianglePolygon2d triangleForFace(const Model& model, const std::vector<int>& face, const cv::Size size);