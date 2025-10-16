#ifndef DRAW_HPP
#define DRAW_HPP

#include "parameter.hpp"

class Drawer {
private:
    cv::Mat canvas;
    int width, height;
    cv::Point2f origin_offset; // 物理座標から画面座標へのずれ補正
    int scale = 10; // 物理座標から画面座標への倍率

public:
    Drawer(int width = 800, int height = 600);
    void clear(); // 毎フレーム初期化用
    void draw_particle(const Eigen::Array<double, 1, 2>& particle_position, const cv::Scalar& color);
    void draw_fiber(const Eigen::Array<double, 1, 2>& p1_position, const Eigen::Array<double, 1, 2>& p2_position, const double thickness, const cv::Scalar& color);
    void show_param(double x1, double y1, double size, std::string str);
    void show(const std::string& window_name = "PF-model", int delay = 30);
    void save_frame(bool image_save_flg, int frame_number, const std::string& folder_path);
};

#endif