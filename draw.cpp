#include "draw.hpp"

Drawer::Drawer(int w, int h)
  : width(w), height(h), origin_offset(w / 2.0f, h / 2.0f)
{
  canvas = cv::Mat(height, width, CV_8UC3, cv::Scalar(255, 255, 255));
}

void Drawer::clear() {
  canvas = cv::Mat(height, width, CV_8UC3, cv::Scalar(255, 255, 255));
}

// 1粒子の描画
void Drawer::draw_particle(const Eigen::Array<double, 1, 2>& particle_position, const cv::Scalar& color) {
  cv::circle(
    canvas, 
    cv::Point2f(particle_position(0,0) * scale, particle_position(0,1) * scale) + origin_offset,
    init_particle_radius * scale,
    color, 
    -1, 
    cv::LINE_AA
  );
}

// 1ファイバーの描画
// [注] 太さが1未満だと0に丸められて描画されるので、描画時だけ1を足す
void Drawer::draw_fiber(const Eigen::Array<double, 1, 2>& p1_position, const Eigen::Array<double, 1, 2>& p2_position, const double thickness, const cv::Scalar& color) {
  cv::line(
    canvas, 
    cv::Point2f(p1_position(0,0) * scale, p1_position(0,1) * scale) + origin_offset,
    cv::Point2f(p2_position(0,0) * scale, p2_position(0,1) * scale) + origin_offset,
    color, 
    (thickness * scale) + 1,
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