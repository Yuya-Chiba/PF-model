#include "draw.hpp"
#include "particle.hpp"
#include "fiber.hpp"

Drawer::Drawer(int w, int h) : width(w), height(h) {
  canvas = cv::Mat(height, width, CV_8UC3, cv::Scalar(255, 255, 255));
}

void Drawer::clear() {
  canvas = cv::Mat(height, width, CV_8UC3, cv::Scalar(255, 255, 255));
}

void Drawer::draw_particle(Particle& p, const cv::Scalar& color) {
  cv::circle(canvas, cv::Point2f(p.x, p.y), p.radius, color, -1, cv::LINE_AA);
}

void Drawer::draw_fiber(double x1, double y1, double x2, double y2, double thickness, const cv::Scalar& color) {
  cv::line(canvas, cv::Point(x1, y1), cv::Point(x2, y2), color, thickness, cv::LINE_AA);
}

void Drawer::show_param(double x1, double y1, double size, std::string str) {
  cv::putText(canvas, str, cv::Point(x1,y1), cv::FONT_HERSHEY_SIMPLEX, size, cv::Scalar(0,0,0), 1.5);
}

void Drawer::show(const std::string& window_name, int delay) {
    cv::imshow(window_name, canvas);
    cv::waitKey(delay);
}