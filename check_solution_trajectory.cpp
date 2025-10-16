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
  int now_pattern = 0;
  bool image_save_flg = false;
  bool esc_pressed = false;
  std::string folder_path = "../result/solution_trajectory";
  
  // (従来との変更点)動径ファイバーの太さを一定の範囲内で全パターン組み合わせを行う
  std::string file_path = "./../source/thickness_patterns.csv";
  auto patterns = read_thickness_patterns_from_csv(file_path);

  // 全パターン(2635通り)についてシミュレーションを回す
  // 条件として、全外周粒子および動径ファイバーの太さの変位が1.0E-8以下になったときに定常とみなす
  for (const auto& pattern : patterns) {

    now_pattern++;
    std::cout << "now_pattern: " << now_pattern << std::endl;
    cp_array.clear();
    pp_array.clear();
    rf_array.clear();
    pf_array.clear();
    // 粒子を初期座標にセット
    Eigen::Array<double, num_outer_particle, 2> outer_particle_positions = set_init_outer_particle();
    
    
    // 動径ファイバー太さ決定
    for (int i = 0; i < num_radial_fiber; i++) rf_array[i].thickness = pattern[i];
    // 動径ファイバー太さに基づいて外周ファイバーの太さ決定
    for (int i=0; i<num_outer_fiber; i++) {
      std::vector<Fiber> connected = find_connected_radial_fibers(pf_array[i], rf_array);
      // std::cout << "thickness: " << rf_array[i].thickness << std::endl;
      pf_array[i].thickness = calc_thickness_pf(connected[0], connected[1]);
    }

    const double threshold = 1e-4; // 閾値
    std::vector<double> displacement; // 変位、収束判定に使用

    // Step1. 初期配置を探す、ファイバー太さは固定して粒子位置だけ動かす
    int step = 0;
    while (true) {
      drawer.clear();
      displacement.clear();
      
      // 粒子にかかる力はリセットする
      for(int i=0; i<num_outer_particle; i++) pp_array[i].force.clear();
      cp_array[0].force.clear();

      // 働く力の計算
      for (int i = 0; i < num_radial_fiber; i++) add_radial_forces(rf_array[i]); // 動径方向の計算(中心粒子と周辺粒子)
      for (int i = 0; i < num_outer_fiber; i++) add_peripheral_forces(pf_array[i]); // 外周方向の計算

      // 運動方程式, 座標rの更新
      Vector2D dr_center = Vector2D::multiple(cp_array[0].force.total(), time_step/viscous_gamma);
      cp_array[0].position = Vector2D::add(cp_array[0].position , dr_center); //中心粒子

      for (int i = 0; i < num_outer_particle; i++) {
        Vector2D dr = Vector2D::multiple(pp_array[i].force.total(), time_step / viscous_gamma);
        pp_array[i].position = Vector2D::add(pp_array[i].position, dr); // 外周粒子

        // 本stepでの変位を中心粒子に対する相対座標で記録する
        Vector2D rel_displacement = Vector2D::subtract(dr, dr_center);
        displacement.push_back(rel_displacement.x);
        displacement.push_back(rel_displacement.y);
      }

      // 収束判定
      bool converged = std::all_of(displacement.begin(), displacement.end(), [threshold](double d) { return (d > -threshold && d < threshold ); });
      if (converged) break;
      if (step > 1000) break;

      // ファイバー（赤）描画
      for(int i=0; i<rf_array.size(); i++) drawer.draw_fiber(rf_array[i], cv::Scalar(0, 0, 255));
      for(int i=0; i<pf_array.size(); i++) drawer.draw_fiber(pf_array[i], cv::Scalar(0, 0, 255));
      // 粒子（緑）描画
      for(int i=0; i<pp_array.size(); i++) drawer.draw_particle(pp_array[i], cv::Scalar(0, 255, 0));
      drawer.draw_particle(cp_array[0], cv::Scalar(0, 255, 0));
      // パラメータ表示
      drawer.show_param(25, 50, 0.6, "Pattern " + std::to_string(now_pattern) + "/" + std::to_string(patterns.size()) + ", Phase: 1, Step: "+ std::to_string(step));
      drawer.show("PF-model");
      // 画像保存
      if (int(step*10)%10 == 0 ) drawer.save_frame(image_save_flg, int(step), folder_path);
      step ++;
      if (cv::waitKey(30) == 27 || step >= max_step) {
        esc_pressed = true;
        break; //window閉じたいときはescキー
      }
    }
    if (esc_pressed) break;

    // Step2. 定常状態を探す、運動方程式と成長方程式どちらも使用
    step = 0;
    while (true) {
      drawer.clear();
      displacement.clear();
      

      // 粒子にかかる力はリセットする
      for(int i=0; i<num_outer_particle; i++) pp_array[i].force.clear();
      cp_array[0].force.clear();

      // 働く力の計算
      for (int i = 0; i < num_radial_fiber; i++) add_radial_forces(rf_array[i]); // 動径方向の計算(中心粒子と周辺粒子)
      for (int i = 0; i < num_outer_fiber; i++) add_peripheral_forces(pf_array[i]); // 外周方向の計算

      // 3. 成長方程式と太さqの更新
      
      // 動径ファイバー 方程式
      double dq_radial[num_radial_fiber];
      for (int i=0; i<num_radial_fiber; i++) {
        dq_radial[i] = calc_thickness_variation_rf(rf_array[i]) * time_step;
      }
      // 動径ファイバー太さ更新
      for(int i=0; i<num_radial_fiber; i++){
        rf_array[i].thickness = rf_array[i].thickness + dq_radial[i];
        displacement.push_back(dq_radial[i]);
      }
      // 外周ファイバー 直接太さを更新
      for (int i=0; i<num_outer_fiber; i++) {
        std::vector<Fiber> connected = find_connected_radial_fibers(pf_array[i], rf_array);
        pf_array[i].thickness = calc_thickness_pf(connected[0], connected[1]);
      }
      
      // 運動方程式, 座標rの更新
      Vector2D dr_center = Vector2D::multiple(cp_array[0].force.total(), time_step/viscous_gamma);
      cp_array[0].position = Vector2D::add(cp_array[0].position , dr_center); //中心粒子

      for (int i = 0; i < num_outer_particle; i++) {
        Vector2D dr = Vector2D::multiple(pp_array[i].force.total(), time_step / viscous_gamma);
        pp_array[i].position = Vector2D::add(pp_array[i].position, dr); // 外周粒子
        
        // 本stepでの変位を中心粒子に対する相対座標で記録する
        Vector2D rel_displacement = Vector2D::subtract(dr, dr_center);
        displacement.push_back(rel_displacement.x);
        displacement.push_back(rel_displacement.y);
      }

      // 収束判定
      bool converged = std::all_of(displacement.begin(), displacement.end(), [threshold](double d) { return (d > -threshold && d < threshold ); });
      if (converged) break;
      if (step > 1000) break;

      // ファイバー（赤）描画
      for(int i=0; i<rf_array.size(); i++) drawer.draw_fiber(rf_array[i], cv::Scalar(0, 0, 255));
      for(int i=0; i<pf_array.size(); i++) drawer.draw_fiber(pf_array[i], cv::Scalar(0, 0, 255));
      // 粒子（緑）描画
      for(int i=0; i<pp_array.size(); i++) drawer.draw_particle(pp_array[i], cv::Scalar(0, 255, 0));
      drawer.draw_particle(cp_array[0], cv::Scalar(0, 255, 0));
      // パラメータ表示
      drawer.show_param(25, 50, 0.6, "Pattern " + std::to_string(now_pattern) + "/" + std::to_string(patterns.size()) + ", Phase: 2, Step: "+ std::to_string(step));
      drawer.show("PF-model");
      // 画像保存
      if (int(step*10)%10 == 0 ) drawer.save_frame(image_save_flg, int(step), folder_path);
      step ++;
      if (cv::waitKey(30) == 27 || step >= max_step) {
        esc_pressed = true;
        break; //window閉じたいときはescキー
      }
    }
    if (esc_pressed) break;
  }

  return 0;
}