#include <algorithm>
#include <opencv2/core.hpp>
#include <opencv2/core/matx.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "model.hpp"
#include "samples.hpp"
#include "shapes.hpp"
#include "util.hpp"

int main() {
    const auto model = Model::fromObjectFile("african_head.obj").unwrap();

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
        cv::fillConvexPoly(image, polygon2d.data(), 3, randomColor());
    }

    const auto original = builtin::shadedImageOf(model);
    cv::imshow("Rotated", image);
    cv::imshow("Original", original);
    cv::waitKey();
}