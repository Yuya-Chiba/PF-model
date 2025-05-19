#include "draw.hpp"
#include "particle.hpp"
#include <cmath>

int main() {
    Drawer drawer;

    Particle p, q;
    double angle = 0.0;
    const double radius = 20;
    const double fiber_thickness = 5;
    const double center_x = 400;
    const double center_y = 300;
    const double move_radius = 100;
    double x3 = 25;
    double y3 = 50;
    double font_size = 0.6;
    int time_step = 0;

    while (true) {
      drawer.clear();

      // 粒子の位置をサイン波で動かす
      double x1 = center_x - move_radius * std::cos(angle);
      double y1 = center_y;
      double x2 = center_x + move_radius * std::cos(angle);
      double y2 = center_y;

      p.set_xy(x1,y1);
      q.set_xy(x2,y2);
      p.radius = radius;
      q.radius = radius;

      // ファイバー（赤）
      drawer.draw_fiber(x1, y1, x2, y2, fiber_thickness, cv::Scalar(0, 0, 255));
      // 長さ
      double fiber_length = std::sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));

      // 粒子（緑）
      drawer.draw_particle(p, cv::Scalar(0, 255, 0));
      drawer.draw_particle(q, cv::Scalar(0, 255, 0));

      // パラメータ表示
      //drawer.show_param(x3, y3, font_size, "Step: "+ std::to_string(time_step) + "      fiber_length: "+ std::to_string(fiber_length));
      //drawer.show_param(x3, y3+40, font_size, "particle 1: ("+ std::to_string(x1) + "," + std::to_string(y1) + ")");
      //drawer.show_param(x3, y3+80, font_size, "particle 2: ("+ std::to_string(x2) + "," + std::to_string(y2) + ")");
      
      drawer.show("PF-model");

      angle += 0.05;
      time_step += 1;
      if (cv::waitKey(30) == 27) break; //window閉じたいときはescキー
    }

    return 0;
}