#include "draw.hpp"

Drawer::Drawer(int w, int h) : width(w), height(h) {
  canvas = cv::Mat(height, width, CV_8UC3, cv::Scalar(255, 255, 255));
}

void Drawer::clear() {
  canvas = cv::Mat(height, width, CV_8UC3, cv::Scalar(255, 255, 255));
}

// 1粒子の描画
void Drawer::draw_particle(Particle& p, const cv::Scalar& color) {
  cv::circle(canvas, cv::Point2f(p.position.x, p.position.y), p.radius, color, -1, cv::LINE_AA);
}

// 1ファイバーの描画、両端の粒子からxy座標を取得する
void Drawer::draw_fiber(Fiber& f, const cv::Scalar& color) {
  Vector2D p1_position = (*f.particle1).position;
  Vector2D p2_position = (*f.particle2).position;
  cv::line(canvas, cv::Point2f(p1_position.x, p1_position.y), cv::Point2f(p2_position.x, p2_position.y), color, f.thickness, cv::LINE_AA);
}

void Drawer::show_param(double x1, double y1, double size, std::string str) {
  cv::putText(canvas, str, cv::Point(x1,y1), cv::FONT_HERSHEY_SIMPLEX, size, cv::Scalar(0,0,0), 1.5);
}

void Drawer::show(const std::string& window_name, int delay) {
  cv::imshow(window_name, canvas);
  cv::waitKey(delay);
}