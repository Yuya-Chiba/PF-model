#include "vector.hpp"
#include "draw.hpp"
#include "particle.hpp"
#include "fiber.hpp"
#include "force.hpp"
#include "function.hpp"
#include "parameter.hpp"
#include <cmath>

int main() {
    
  Drawer drawer;

  std::vector<Particle> cp_array; // 中心粒子の配列
  std::vector<Particle> pp_array; // 周辺粒子の配列
  std::vector<Fiber> rf_array; // 動径ファイバー
  std::vector<Fiber> pf_array; // 外周ファイバー
  int now_step = 0;
  bool image_save_flg = false;
  std::string folder_path = "../result/only_force";
  

  // 初期座標のセット
  set_regular_hexagon(cp_array, pp_array, rf_array, pf_array);
  int particle_num = pp_array.size();

  while (true) {
    drawer.clear();

    // 粒子にかかる力はリセットする
    for(int i=0; i<particle_num; i++) pp_array[i].force.clear();
    cp_array[0].force.clear();

    // 時間の計算
    double now_time = now_step * time_step;

    //std::cerr << "particle: " << cp_array[0].get_x() << "," << cp_array[0].get_y() << std::endl;

    // 2. 働く力の計算
    // 動径方向の計算(中心粒子と周辺粒子)
    for (int i = 0; i < num_radial_fiber; i++) {
      Vector2D restoring_force = calc_restoring_force_rf(rf_array[i]);
      Vector2D contraction_force = calc_contraction_force_rf(rf_array[i]);
      Vector2D extension_force = calc_extension_force(rf_array[i]);

      // 両端粒子にそれぞれ対称な力を加える p1_forceなどは&を使って直接参照する
      Force& force_p1 = (*rf_array[i].particle1).force;
      Force& force_p2 = (*rf_array[i].particle2).force;
      force_p1.restoring_radial = Vector2D::add(force_p1.restoring_radial, Vector2D::oppo(restoring_force));
      force_p2.restoring_radial = Vector2D::add(force_p2.restoring_radial, restoring_force);
      force_p1.contraction_radial = Vector2D::add(force_p1.contraction_radial, Vector2D::oppo(contraction_force));
      force_p2.contraction_radial = Vector2D::add(force_p2.contraction_radial, contraction_force);
      force_p1.extension_radial = Vector2D::add(force_p1.extension_radial, Vector2D::oppo(extension_force));
      force_p2.extension_radial = Vector2D::add(force_p2.extension_radial, extension_force);
    }
    // 外周方向の計算
    for (int i = 0; i < num_radial_fiber; i++) {
      Vector2D restoring_force = calc_restoring_force_pf(pf_array[i]);
      Vector2D contraction_force = calc_contraction_force_pf(pf_array[i]);

      // 両端粒子にそれぞれ対称な力を加える p1_forceなどは&を使って直接参照する
      Force& force_p1 = (*pf_array[i].particle1).force;
      Force& force_p2 = (*pf_array[i].particle2).force;
      force_p1.restoring_peripheral = Vector2D::add(force_p1.restoring_peripheral, Vector2D::oppo(restoring_force));
      force_p2.restoring_peripheral = Vector2D::add(force_p2.restoring_peripheral, restoring_force);
      force_p1.contraction_peripheral = Vector2D::add(force_p1.contraction_peripheral, Vector2D::oppo(contraction_force));
      force_p2.contraction_peripheral = Vector2D::add(force_p2.contraction_peripheral, contraction_force);
    }

    // 3. 成長方程式と太さqの更新
    
    // 外周ファイバーは動径ファイバー依存

    // 5. 運動方程式と座標rの更新
    Vector2D dc = Vector2D::multiple(cp_array[0].force.total(), time_step/viscous_gamma);
    Vector2D dr[particle_num];
    for(int i=0; i<particle_num; i++){
      dr[i] = Vector2D::multiple(pp_array[i].force.total(), time_step/viscous_gamma);
    }

    cp_array[0].position = Vector2D::add(cp_array[0].position , dc); //中心粒子の移動
    for(int i=0; i<particle_num; i++){
      pp_array[i].position = Vector2D::add(pp_array[i].position , dr[i]); //外周粒子の移動
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
    drawer.show_param(25, 50, 0.6, "Step: "+ std::to_string(now_time));
    drawer.show("PF-model");

    // 画像保存
    if (int(now_time*10)%10 == 0 ) {
      drawer.save_frame(image_save_flg, int(now_time), folder_path);
    }
    now_step ++;

    if (cv::waitKey(30) == 27 || now_time >= max_time) break; //window閉じたいときはescキー
  }

  return 0;
}