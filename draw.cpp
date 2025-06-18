#include "draw.hpp"

Drawer::Drawer(int w, int h)
  : width(w), height(h), origin_offset(w / 2.0f, h / 2.0f)
{
  if (w <= 0 || h <= 0 || w > 5000 || h > 5000) {
    std::cerr << "Invalid canvas size: " << w << "x" << h << std::endl; // memory_error回避
    exit(1);
  }
  canvas = cv::Mat(height, width, CV_8UC3, cv::Scalar(255, 255, 255));
}

void Drawer::clear() {
  canvas = cv::Mat(height, width, CV_8UC3, cv::Scalar(255, 255, 255));
}

// 1粒子の描画
void Drawer::draw_particle(Particle& p, const cv::Scalar& color) {
  cv::circle(
    canvas, 
    cv::Point2f(p.get_x() * scale, p.get_y() * scale) + origin_offset,
    p.radius * scale,
    color, 
    -1, 
    cv::LINE_AA
  );
}

// 1ファイバーの描画、両端の粒子からxy座標を取得する
void Drawer::draw_fiber(Fiber& f, const cv::Scalar& color) {
  cv::line(
    canvas, 
    cv::Point2f((*f.particle1).get_x() * scale, (*f.particle1).get_y() * scale) + origin_offset,
    cv::Point2f((*f.particle2).get_x() * scale, (*f.particle2).get_y() * scale) + origin_offset,
    color, 
    f.thickness * scale, 
    cv::LINE_AA
  );
}

void Drawer::show_param(double x1, double y1, double size, std::string str) {
  cv::putText(canvas, str, cv::Point(x1,y1), cv::FONT_HERSHEY_SIMPLEX, size, cv::Scalar(0,0,0), 2);
}

void Drawer::show(const std::string& window_name, int delay) {
  cv::imshow(window_name, canvas);
  cv::waitKey(delay);
}

void Drawer::save_frame(bool image_save_flg, int frame_number, const std::string& folder_path) {
  if (image_save_flg == true) {
    // フォルダが存在しない場合は作成
    std::filesystem::create_directories(folder_path);

    // 画像ファイル名を作成
    std::ostringstream filename;
    filename << folder_path << "/frame_" << std::setfill('0') << std::setw(5) << frame_number << ".png";

    // 保存
    cv::imwrite(filename.str(), canvas);
  }
}