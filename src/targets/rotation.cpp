
#include "model.hpp"
#include "samples.hpp"
#include "shapes.hpp"
#include "util.hpp"
#include <algorithm>
#include <opencv2/core.hpp>
#include <opencv2/core/matx.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

std::vector<cv::Vec4f> addPadding(const VertexBuffer& vb) {
    std::vector<cv::Vec4f> vec(vb.size());
    const auto data = reinterpret_cast<const cv::Vec3f*>(vb.data());
    std::transform(
        data,
        data + vb.size() * 3,
        begin(vec),
        [](const cv::Vec3f& vertex){
            return cv::Vec4f { vertex[0], vertex[1], vertex[2], 0 };
        }
    );
    return vec;
}

cv::Matx<float, 3, 3> xRotationMatrix(double angle) {
    const float angleCos = cos(angle);
    const float angleSin = sin(angle);

    return {
        1, 0, 0,
        0, angleCos, angleSin,
        0, -angleSin, angleCos
    };
}

cv::Matx<float, 3, 3> yRotationMatrix(double angle) {
    const float angleCos = cos(angle);
    const float angleSin = sin(angle);

    return {
        angleCos, 0, angleSin,
        0, 1, 0,
        -angleSin, 0, angleCos
    };
}

cv::Matx<float, 3, 3> zRotationMatrix(double angle) {
    const float angleCos = cos(angle);
    const float angleSin = sin(angle);

    return {
        angleCos, -angleSin, 0,
        angleSin,  angleCos, 0,
        0, 0, angleCos
    };
}

int main() {
    const auto model = Model::fromObjectFile("african_head.obj").unwrap();

    // const auto vertexes = addPadding(model.vertexBuffer());
    const auto vb = model.vertexBuffer();
    const auto data = reinterpret_cast<const float*>(vb.data());

    const cv::Size size { 3, static_cast<int>(vb.size() * 3) };
    const cv::Mat verts { size, CV_32F, const_cast<float*>(data) };
    const auto rotation = yRotationMatrix(-M_PI/4);
    const cv::Mat rotated = verts * rotation;

    const cv::Size canvasSize { 800, 800 };
    cv::Mat image { canvasSize, CV_8UC3 };
    const auto rotated_data = reinterpret_cast<TrianglePolygon*>(rotated.data);
    for (size_t i = 0; i < vb.size(); ++i) {
        const auto polygon2d = project2d(rotated_data[i], canvasSize);
        // drawTriangle(image, polygon2d, randomColor());
        cv::fillConvexPoly(image, polygon2d.data(), 3, randomColor());
    }

    const auto original = builtin::shadedImageOf(model);
    cv::imshow("Rotated", image);
    cv::imshow("Original", original);
    cv::waitKey();
}