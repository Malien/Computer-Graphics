#include <algorithm>
#include <cmath>
#include <thread>
#include <opencv2/core.hpp>
#include <opencv2/core/matx.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "color_range.hpp"
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

    const cv::Size canvasSize { 800, 800 };

    double rotationAngle = 0;

    const LinearColorRange colorRange {
        {0, {255, 0, 255}},
        {1, {255, 255, 0}}
    };

    do {
        cv::Mat image = cv::Mat::zeros(canvasSize, CV_8UC3);
        rotationAngle += M_PI / 120;
        const cv::Mat rotated = verts * yRotationMatrix(rotationAngle) * xRotationMatrix(rotationAngle) * zRotationMatrix(rotationAngle);
        const auto rotated_data = reinterpret_cast<TrianglePolygon*>(rotated.data);
        for (size_t i = 0; i < vb.size(); ++i) {
            const auto polygon2d = project2d(rotated_data[i], canvasSize);
            const double percent = 1.0 * i / vb.size(); 
            // drawTriangle(image, polygon2d, colorRange.pickValue(percent));
            cv::fillConvexPoly(image, polygon2d.data(), 3, colorRange.pickValue(percent));
        }
        cv::imshow("Rotated", image);
    } while (cv::waitKey(1000 / 60) != 0);
}