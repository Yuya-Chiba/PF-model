#include "draw.hpp"

Drawer::Drawer(int w, int h) : width(w), height(h) {
  canvas = cv::Mat(height, width, CV_8UC3, cv::Scalar(255, 255, 255));
}

void Drawer::clear() {
  canvas = cv::Mat(height, width, CV_8UC3, cv::Scalar(255, 255, 255));
}

// 1粒子の描画
void Drawer::draw_particle(Particle& p, const cv::Scalar& color) {
  cv::circle(canvas, cv::Point2f(p.get_x(), p.get_y()), p.radius*10, color, -1, cv::LINE_AA);
}

// 1ファイバーの描画、両端の粒子からxy座標を取得する
void Drawer::draw_fiber(Fiber& f, const cv::Scalar& color) {
  cv::line(canvas, cv::Point2f((*f.particle1).get_x(), (*f.particle1).get_y()), cv::Point2f((*f.particle2).get_x(), (*f.particle2).get_y()), color, f.thickness*10, cv::LINE_AA);
}

void Drawer::show_param(double x1, double y1, double size, std::string str) {
  cv::putText(canvas, str, cv::Point(x1,y1), cv::FONT_HERSHEY_SIMPLEX, size, cv::Scalar(0,0,0), 2);
}

void Drawer::show(const std::string& window_name, int delay) {
  cv::imshow(window_name, canvas);
  cv::waitKey(delay);
}