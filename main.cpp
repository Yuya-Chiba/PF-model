#include "vector.hpp"
#include "draw.hpp"
#include "particle.hpp"
#include "fiber.hpp"
#include "function.hpp"
#include <cmath>

int main() {
    
  Drawer drawer;

  int particle_num = 2;
  int fiber_num = 1;
  std::vector<Particle> particle_array(particle_num);
  std::vector<Fiber> fiber_array(fiber_num);
  fiber_array[0].particle1 = &particle_array[0];
  fiber_array[0].particle2 = &particle_array[1];
  const double radius = 20;
  const double fiber_thickness = 5;
  const double center_x = 400;
  const double center_y = 300;
  const double move_radius = 100;
  double x3 = 25;
  double y3 = 50;
  double font_size = 0.6;
  double time_step = 0;
  double gamma = 10; // 粘性抵抗係数
  Vector2D v; // ベクトル計算用、実際に値が入るわけではない

  particle_array[0].set_position(200,300);
  particle_array[1].set_position(500,300);
  particle_array[0].radius = radius;
  particle_array[1].radius = radius;

  while (true) {
    drawer.clear();

    // particle_array[0].set_position(0,0);
    // particle_array[0].position.set(x1,y1); という書き方でもよい
    // particle_array[1].set_position(0,0);

    fiber_array[0].thickness = fiber_thickness;

    calc_restoring_force(particle_array, fiber_array);

    Vector2D dr[particle_num];
    for(int i=0; i<particle_num; i++){
      dr[i] = v.multiple(particle_array[i].force,1/gamma);
    }

    //座標の更新
    for(int i=0; i<particle_num; i++){
      particle_array[i].position = v.add(particle_array[i].position , dr[i]); //粒子の移動
    }

    // ファイバー（赤）
    drawer.draw_fiber(fiber_array[0], cv::Scalar(0, 0, 255));

    // 粒子（緑）
    drawer.draw_particle(particle_array[0], cv::Scalar(0, 255, 0));
    drawer.draw_particle(particle_array[1], cv::Scalar(0, 255, 0));

    Vector2D fiber = v.substract(particle_array[0].position, particle_array[1].position);
    double fiber_length = fiber.length();

    // パラメータ表示
    drawer.show_param(x3, y3, font_size, "Step: "+ std::to_string(time_step));
    drawer.show_param(x3, y3+40, font_size, "fiber_length: "+ std::to_string(fiber_length));

    drawer.show("PF-model");

    time_step +=0.1;
    if (cv::waitKey(30) == 27) break; //window閉じたいときはescキー
  }

  return 0;
}