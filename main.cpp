#include "draw.hpp"
#include <cmath>

int main() {
    Drawer drawer;

    float angle = 0.0;
    const float radius = 20;
    const float fiber_thickness = 5;
    const float center_x = 400;
    const float center_y = 300;
    const float move_radius = 100;
    float x3 = 25;
    float y3 = 50;
    float font_size = 1;
    int time_step = 0;

    while (true) {
      drawer.clear();

      // 粒子の位置をサイン波で動かす
      float x1 = center_x - move_radius * std::cos(angle);
      float y1 = center_y;
      float x2 = center_x + move_radius * std::cos(angle);
      float y2 = center_y;

      // ファイバー（赤）
      drawer.draw_fiber(x1, y1, x2, y2, fiber_thickness, cv::Scalar(0, 0, 255));

      // 粒子（緑）
      drawer.draw_particle(x1, y1, radius, cv::Scalar(0, 255, 0));
      drawer.draw_particle(x2, y2, radius, cv::Scalar(0, 255, 0));

      // パラメータ表示
      drawer.show_param(x3, y3, font_size, "Step: "+ std::to_string(time_step));
      drawer.show_param(x3, y3+60, font_size, "particle 1: ("+ std::to_string(x1) + "," + std::to_string(y1) + ")");
      drawer.show_param(x3, y3+120, font_size, "particle 2: ("+ std::to_string(x2) + "," + std::to_string(y2) + ")");
      drawer.show("PF-model");

      angle += 0.05;
      time_step += 1;
      if (cv::waitKey(30) == 27) break; //window閉じたいときはescキー
    }

    return 0;
}