#include "vector.hpp"
#include "draw.hpp"
#include "particle.hpp"
#include "fiber.hpp"
#include "function.hpp"
#include "parameter.hpp"
#include <cmath>

int main() {
    
  Drawer drawer;

  std::vector<Particle> cp_array; // 中心粒子の配列
  std::vector<Particle> pp_array; // 周辺粒子の配列
  std::vector<Fiber> rf_array; // 動径ファイバー
  std::vector<Fiber> pf_array; // 外周ファイバー
  double time_step = 0;
  bool image_save_flg = false;
  std::string folder_path = "../result";
  Vector2D v; // ベクトル計算用、実際に値が入るわけではない

  // 初期座標のセット
  set_regular_hexagon(cp_array, pp_array, rf_array, pf_array);
  int particle_num = pp_array.size();

  while (true) {
    drawer.clear();

    // 粒子にかかる力はリセットする
    for(int i=0; i<particle_num; i++) pp_array[i].force.set(0, 0);
    cp_array[0].force.set(0,0);

    //std::cerr << "particle: " << cp_array[0].get_x() << "," << cp_array[0].get_y() << std::endl;

    // 動径方向の計算(中心粒子と周辺粒子)
    calc_restoring_force_rf(rf_array);
    calc_contraction_force_rf(rf_array);
    calc_extension_force(rf_array);

    // 外周方向の計算
    calc_restoring_force_pf(pf_array);
    calc_contraction_force_pf(pf_array);

    Vector2D dc = v.multiple(cp_array[0].force, 1/viscous_gamma);
    Vector2D dr[particle_num];
    for(int i=0; i<particle_num; i++){
      dr[i] = v.multiple(pp_array[i].force, 1/viscous_gamma);
    }

    //座標の更新
    cp_array[0].position = v.add(cp_array[0].position , dc); //中心粒子の移動
    for(int i=0; i<particle_num; i++){
      pp_array[i].position = v.add(pp_array[i].position , dr[i]); //外周粒子の移動
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
    drawer.show_param(25, 50, 0.6, "Step: "+ std::to_string(time_step));
    drawer.show("PF-model");

    // 画像保存
    if (std::floor(time_step) == time_step) {
    drawer.save_frame(image_save_flg, int(time_step), folder_path);
    }

    time_step +=0.1;
    if (cv::waitKey(30) == 27 || time_step == max_time) break; //window閉じたいときはescキー
  }

  return 0;
}