#include "samples.hpp"

template <class T> cv::Mat wireframeImageOf(const Model &model, T drawLineFn) {
    const cv::Size size{800, 800};

    cv::Mat image(size, CV_8UC3, cv::Scalar(0));

    for (const auto &face : model.faces) {
        const auto [p1, p2, p3] = triangleForFace(model, face, size);
        drawLineFn(image, p1, p2);
        drawLineFn(image, p2, p3);
        drawLineFn(image, p3, p1);
    }

    return image;
}

template <class T>
cv::Mat shadedImageOf(const Model &model, T polygonDrawFn) {
    const cv::Size size{800, 800};
    cv::Mat image{size, CV_8UC3};

    for (const auto &face : model.faces) {
        const auto polygon = triangleForFace(model, face, size);
        polygonDrawFn(image, triangleForFace(model, face, size));
        cv::fillConvexPoly(image, polygon.data(), 3, randomColor(), cv::LINE_AA);
    }

    return image;
}

cv::Mat builtin::wireframeImageOf(const Model &model) {
    return wireframeImageOf(model, [](cv::Mat &image, cv::Point from, cv::Point to) {
        cv::line(image, from, to, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
    });
}

cv::Mat builtin::shadedImageOf(const Model &model) {
    return shadedImageOf(model, [](cv::Mat &image, const TrianglePolygon& triangle) {
        cv::fillConvexPoly(image, triangle.data(), 3, randomColor(), cv::LINE_AA);
    });
}

cv::Mat homemade::linesSampleImage() {
    cv::Mat res(400, 400, CV_8UC3);

    drawCircle(res, {300, 300}, 50);
    drawCircle(res, {200, 200}, 150, {0, 0, 255});

    drawLine(res, {100, 100}, {10, 10});

    drawLine(res, {100, 100}, {100, 200});

    drawLine(res, {20, 40}, {60, 80});
    drawLine(res, {20, 380}, {380, 20});

    for (int i = 0; i < 20; ++i) {
        drawLine(res, {10, i * 10}, {200, i * 10});
    }

    return res;
}

cv::Mat homemade::trianglesSampleImage() {
    cv::Mat res(400, 400, CV_8UC3);

    drawTriangle(res, {{{100, 300}, {200, 100}, {300, 200}}});

    return res;
}

cv::Mat homemade::wireframeImageOf(const Model &model) {
    return wireframeImageOf(model, [](cv::Mat &image, cv::Point from, cv::Point to) {
        drawLine(image, from, to);
    });
}

cv::Mat homemade::shadedImageOf(const Model &model) {
    return shadedImageOf(model, [](cv::Mat &image, const TrianglePolygon& triangle) {
        drawTriangle(image, triangle, randomColor());
    });
}