#include "vector.hpp"
#include "draw.hpp"
#include "particle.hpp"
#include "fiber.hpp"
#include "function.hpp"
#include <cmath>

int main() {
    
  Drawer drawer;

  std::vector<Particle> cp_array; // 中心粒子の配列
  std::vector<Particle> pp_array; // 周辺粒子の配列
  std::vector<Fiber> rf_array; // 動径ファイバー
  std::vector<Fiber> pf_array; // 外周ファイバー
  const double radius = 2;
  const double fiber_thickness = 0.5;
  double x3 = 25;
  double y3 = 50;
  double font_size = 0.6;
  double time_step = 0;
  double gamma = 10; // 粘性抵抗係数
  Vector2D v; // ベクトル計算用、実際に値が入るわけではない

  // 初期座標のセット
  set_regular_hexagon(cp_array, pp_array, rf_array, pf_array);
  int particle_num = pp_array.size();

  while (true) {
    drawer.clear();

    // 粒子にかかる力はリセットする
    for(int i=0; i<particle_num; i++) pp_array[i].force.set(0, 0);

    std::cerr << "particle: " << pp_array[0].get_x() << "," << pp_array[0].get_y() << std::endl;

    // 動径方向の計算(中心粒子と周辺粒子)
    calc_restoring_force(rf_array);
    calc_contraction_force(rf_array);
    calc_extension_force(rf_array);
    cp_array[0].force.set(0,0);

    // 外周方向の計算
    calc_restoring_force(pf_array);
    calc_contraction_force(pf_array);
    //calc_extension_force(pf_array);

    Vector2D dr[particle_num];
    for(int i=0; i<particle_num; i++){
      dr[i] = v.multiple(pp_array[i].force,1/gamma);
    }

    //座標の更新
    for(int i=0; i<particle_num; i++){
      pp_array[i].position = v.add(pp_array[i].position , dr[i]); //粒子の移動
    }

    // ファイバー（赤）描画
    for(int i=0; i<rf_array.size(); i++){
      drawer.draw_fiber(rf_array[i], cv::Scalar(0, 0, 255));
    }
    for(int i=0; i<pf_array.size(); i++){
      drawer.draw_fiber(pf_array[i], cv::Scalar(0, 0, 255));
    }
    // 粒子（緑）描画
    for(int i=0; i<pp_array.size(); i++){
      drawer.draw_particle(pp_array[i], cv::Scalar(0, 255, 0));
    }
    drawer.draw_particle(cp_array[0], cv::Scalar(0, 255, 0));

    // パラメータ表示
    drawer.show_param(x3, y3, font_size, "Step: "+ std::to_string(time_step));

    drawer.show("PF-model");

    time_step +=0.1;
    if (cv::waitKey(30) == 27) break; //window閉じたいときはescキー
  }

  return 0;
}