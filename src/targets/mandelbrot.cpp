#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "shapes.hpp"
#include "color_range.hpp"

int main() {
    const LinearColorRange colorRange {
        {0    , { 80 , 0  , 0   }},
        {0.3  , { 200, 50, 50   }},
        {0.7  , { 0  , 255, 255 }},
        {0.99 , { 0  , 0  , 255 }},
        {0.999, { 0  , 0  , 0   }},
        {1    , { 0  , 0  , 0   }}
    };
    // const LinearColorRange colorRange { {255, 0, 0}, {0, 255, 255}};
    const auto whole = mandelbrot({ 600, 600 }, { -2.5, -1.75, 3.5, 3.5 }, 50, colorRange);
    draw_color_gradient(whole, colorRange, {585, 0, 15, 600});

    const auto more_detailed = mandelbrot({ 600, 600 }, { -2.5, -1.75, 3.5, 3.5 }, 400, colorRange);
    draw_color_gradient(more_detailed, colorRange, {585, 0, 15, 600});

    const auto zoomed_in = mandelbrot({ 1200, 1200 }, { -2.5 / 8 - 0.55, -1.75 / 8, 3.5 / 8, 3.5 / 8 }, 100, colorRange);
    draw_color_gradient(zoomed_in, colorRange, {1170, 0, 30, 1200});

    const auto zoomed_in_further = mandelbrot({ 1200, 1200 }, { -2.5 / 32 - 0.75, -1.75 / 32 - 0.2, 3.5 / 32, 3.5 / 32 }, 200, colorRange);
    draw_color_gradient(zoomed_in_further, colorRange, {1170, 0, 30, 1200});

    cv::imshow("Zoomed In Further", zoomed_in_further);
    cv::imshow("Zoomed In", zoomed_in);
    cv::imshow("More Detailed", more_detailed);
    cv::imshow("Mandelbrot", whole);

    cv::waitKey(0);
}