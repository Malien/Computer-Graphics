#include "model.hpp"
#include "samples.hpp"

int main() {
    const Model model = Model::fromObjectFile("african_head.obj").unwrap();

    cv::Mat wireframe = builtin::wireframeImageOf(model);
    cv::Mat myWireframe = homemade::wireframeImageOf(model);

    cv::Mat lines = homemade::linesSampleImage();

    cv::imshow("Lines", lines);
    cv::imshow("Wireframe", wireframe);
    cv::imshow("My Own", myWireframe);

    cv::waitKey(0);
}
