#include "samples.hpp"

int main() {
    const Model model = Model::fromObjectFile("african_head.obj").unwrap();

    cv::Mat wireframe = builtin::wireframeImageOf(model);
    cv::Mat shadedHead = builtin::shadedImageOf(model);

    cv::Mat myShadedHead = homemade::shadedImageOf(model);
    cv::Mat triangles = homemade::trianglesSampleImage();

    cv::imshow("Wireframe", wireframe);
    cv::imshow("Triangle", triangles);
    cv::imshow("Shaded", shadedHead);
    cv::imshow("My shading", myShadedHead);

    cv::waitKey(0);
}
