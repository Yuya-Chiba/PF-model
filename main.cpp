#include "vector.hpp"
#include "draw.hpp"
#include "particle.hpp"
#include "fiber.hpp"
#include "force.hpp"
#include <cmath>

int main() {
    
    Drawer drawer;

    std::vector<Particle> particle_array(2);
    std::vector<Fiber> fiber_array(1);
    fiber_array[0].particle1 = &particle_array[0];
    fiber_array[0].particle2 = &particle_array[1];
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

      particle_array[0].set_position(x1,y1);
      // particle_array[0].position.set(x1,y1); という書き方でもよい
      particle_array[1].set_position(x2,y2);
      particle_array[0].radius = radius;
      particle_array[1].radius = radius;

      fiber_array[0].thickness = fiber_thickness;

      // ファイバー（赤）
      drawer.draw_fiber(fiber_array[0], cv::Scalar(0, 0, 255));

      // 粒子（緑）
      drawer.draw_particle(particle_array[0], cv::Scalar(0, 255, 0));
      drawer.draw_particle(particle_array[1], cv::Scalar(0, 255, 0));

      // パラメータ表示
      //drawer.show_param(x3, y3, font_size, "Step: "+ std::to_string(time_step) + "      fiber_length: "+ std::to_string(fiber_length));
      //drawer.show_param(x3, y3+40, font_size, "particle 1: ("+ std::to_string(x1) + "," + std::to_string(y1) + ")");
      
      drawer.show("PF-model");

      angle += 0.05;
      time_step += 1;
      if (cv::waitKey(30) == 27) break; //window閉じたいときはescキー
    }

    return 0;
}