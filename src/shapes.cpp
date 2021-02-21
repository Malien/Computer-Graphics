#include <algorithm>
#include <cstddef>
#include <opencv2/core/types.hpp>
// #include <range/v3/view.hpp>

// #include "range/v3/range_fwd.hpp"
// #include "range/v3/view/span.hpp"
// #include "range/v3/view/subrange.hpp"
// #include "range/v3/view/transform.hpp"
#include "shapes.hpp"

template <class T>
void drawLineImpl(
    T yCmpFunc,
    cv::Mat &image,
    const cv::Point from,
    const cv::Point to,
    const cv::Vec3b color
) {
    cv::Vec2i translation = to - from;
    cv::Vec2d displacement = cv::normalize(static_cast<cv::Vec2d>(translation));
    cv::Vec2d current = static_cast<cv::Vec2i>(from);
    int x = current[0];
    int y = current[1];
    while (x < to.x || yCmpFunc(y, to.y)) {
        if (x >= 0 && x < image.cols && y >= 0 && y < image.cols) {
            image.at<cv::Vec3b>(y, x) = color;
        }
        current += displacement;
        x = current[0];
        y = current[1];
    }
}

void drawLine(
    cv::Mat &image,
    const cv::Point from,
    const cv::Point to,
    const cv::Vec3b color
) {
    const auto lessCmp = [](double y, double toY) { return y < toY; };
    const auto greaterCmp = [](double y, double toY) { return y > toY; };

    if (to.x < from.x)
        return drawLine(image, to, from, color);
    if (to.y < from.y)
        return drawLineImpl(greaterCmp, image, from, to, color);
    return drawLineImpl(lessCmp, image, from, to, color);
}

void drawCircle(
    cv::Mat &image,
    const cv::Point center,
    const uint radius,
    const cv::Vec3b color
) {
    for (int x = center.x - radius; x < center.x + radius; ++x) {
        const float d = sqrt(radius * radius - pow(x - center.x, 2));
        const int y1 = center.y - d;
        const int y2 = center.y + d;
        if (y1 >= 0 && y1 < image.rows) {
            image.at<cv::Vec3b>(y1, x) = color;
        }
        if (y2 >= 0 && y2 < image.rows) {
            image.at<cv::Vec3b>(y2, x) = color;
        }
    }
    for (int y = center.y - radius; y < center.y + radius; ++y) {
        const float d = sqrt(radius * radius - pow(y - center.y, 2));
        const int x1 = center.x - d;
        const int x2 = center.x + d;
        if (x1 >= 0 && x1 < image.cols) {
            image.at<cv::Vec3b>(y, x1) = color;
        }
        if (x2 >= 0 && x2 < image.cols) {
            image.at<cv::Vec3b>(y, x2) = color;
        }
    }
}

int edgeFunction(const cv::Point& p, const cv::Point& v0, const cv::Point& v1) {
    return (p.x - v1.x) * (v0.y - v1.y) - (p.y - v1.y) * (v0.x - v1.x);
}

bool isWithinTriangle(const cv::Point& p, const TrianglePolygon2d& triangle) {
    const auto& [v0, v1, v2] = triangle;
    return edgeFunction(p, v0, v1) >= 0 && edgeFunction(p, v1, v2) >= 0 && edgeFunction(p, v2, v0) >= 0;
}

cv::Rect boundingBox(const TrianglePolygon2d &triangle) {
    std::array<int, 3> xs, ys;
    std::transform(begin(triangle), end(triangle), begin(xs), [](const cv::Point &p) {
        return p.x;
    });
    std::transform(begin(triangle), end(triangle), begin(ys), [](const cv::Point &p) {
        return p.y;
    });
    // const auto range = ranges::make_subrange(begin(triangle), end(triangle));
    // const auto xs = range | ranges::views::transform([](const cv::Point& p) {
    // return p.x; }); const auto ys = range | ranges::views::transform([](const
    // cv::Point& p) { return p.y; });

    const auto [minx, maxx] = std::minmax_element(begin(xs), end(xs));
    const auto [miny, maxy] = std::minmax_element(begin(ys), end(ys));

    return {cv::Point{*minx, *miny}, cv::Point{*maxx, *maxy}};
}

void drawTriangle(cv::Mat &image, const TrianglePolygon2d triangle, const cv::Vec3b color) {
    const auto [x, y, width, height] = boundingBox(triangle);
    for (int i = y; i < y + height; ++i) {
        for (int j = x; j < x + width; ++j) {
            if (isWithinTriangle({j, i}, triangle)) {
                image.at<cv::Vec3b>(i, j) = color;
            }
        }
    }
}