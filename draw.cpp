#include "draw.hpp"

Drawer::Drawer(int w, int h) : width(w), height(h) {
    canvas = cv::Mat::zeros(height, width, CV_8UC3);
}

void Drawer::clear() {
    canvas = cv::Mat::zeros(height, width, CV_8UC3);
}

void Drawer::draw_particle(float x, float y, float radius, const cv::Scalar& color) {
    cv::circle(canvas, cv::Point(x, y), radius, color, -1, cv::LINE_AA);
}

void Drawer::draw_fiber(float x1, float y1, float x2, float y2, float thickness, const cv::Scalar& color) {
    cv::line(canvas, cv::Point(x1, y1), cv::Point(x2, y2), color, thickness, cv::LINE_AA);
}

void Drawer::show(const std::string& window_name, int delay) {
    cv::imshow(window_name, canvas);
    cv::waitKey(delay);
}