#include "draw.hpp"
#include "function.hpp"
#include "parameter.hpp"

int main() {
  // 粒子、ファイバー関連の宣言・初期化
  // 計算で使用する変数（高速化のためEigenを使用）
  Eigen::Array<double, num_center_particle, 2> center_particle_positions; // 中心粒子位置
  center_particle_positions.setZero();
  Eigen::Array<double, num_outer_particle, 2> outer_particle_positions = set_init_outer_particle(); // 周辺粒子位置
  Eigen::Array<double, num_radial_fiber, 1> radial_fiber_thicknesses; // 動径ファイバー太さ
  for (int i=0; i<num_radial_fiber; i++) radial_fiber_thicknesses(i,0) = init_rf_thickness;
  Eigen::Array<double, num_outer_fiber, 1> outer_fiber_thicknesses; // 外周ファイバー太さ
  for (int i=0; i<num_outer_fiber; i++) outer_fiber_thicknesses(i,0) = init_of_thickness;
  Eigen::Array<double, num_center_particle, 2> center_particle_forces; // 中心粒子に働く力
  Eigen::Array<double, num_outer_particle, 2> outer_particle_forces; // 周辺粒子に働く力
  // 粒子とファイバーの接続情報 複数細胞用に拡張可能
  std::vector<std::pair<int, int>> outer_fiber_to_particles = {
    {0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 0} };

  // 描画・ファイル保存用設定
  bool draw_flg = false;
  bool image_save_flg = false;
  std::string folder_path = "../result/only_force";

  Drawer drawer;
  int step = 1;
  while (step <= max_step) {
    drawer.clear();

    // 粒子にかかる力をリセットする
    center_particle_forces.setZero();
    outer_particle_forces.setZero();

    std::cout << "Step: " << step << std::endl;

    // 2. 働く力の計算
    // 動径方向の計算(中心粒子と周辺粒子)
    for (int i = 0; i < num_radial_fiber; i++) {
      center_particle_forces -= calc_contraction_force_rf(center_particle_positions, outer_particle_positions.row(i), radial_fiber_thicknesses(i,0));
      outer_particle_forces.row(i) += calc_contraction_force_rf(center_particle_positions, outer_particle_positions.row(i), radial_fiber_thicknesses(i,0));
      center_particle_forces -= calc_restoring_force_rf(center_particle_positions, outer_particle_positions.row(i));
      outer_particle_forces.row(i) += calc_restoring_force_rf(center_particle_positions, outer_particle_positions.row(i));
      outer_particle_forces.row(i) += calc_extension_force(center_particle_positions, outer_particle_positions.row(i), radial_fiber_thicknesses(i,0)); // 伸展力は外周粒子のみ
    }
    // 外周方向の計算
    for (int i = 0; i < num_outer_fiber; i++) {
      auto [p1, p2] = outer_fiber_to_particles[i]; // fiberと接続するparticleの番号
      outer_particle_forces.row(p1) -= calc_contraction_force_of(outer_particle_positions.row(p1), outer_particle_positions.row(p2), outer_fiber_thicknesses(i,0));
      outer_particle_forces.row(p2) += calc_contraction_force_of(outer_particle_positions.row(p1), outer_particle_positions.row(p2), outer_fiber_thicknesses(i,0));
      outer_particle_forces.row(p1) -= calc_restoring_force_of(outer_particle_positions.row(p1), outer_particle_positions.row(p2));
      outer_particle_forces.row(p2) += calc_restoring_force_of(outer_particle_positions.row(p1), outer_particle_positions.row(p2));
    }

    // 3. 成長方程式と太さqの更新
    // 動径ファイバー 方程式
    for (int i=0; i<num_radial_fiber; i++) {
      double dq_radial = calc_thickness_variation_rf(center_particle_positions, outer_particle_positions.row(i), radial_fiber_thicknesses(i,0)) * delta_t;
      radial_fiber_thicknesses(i,0) += dq_radial;
    }
    // 外周ファイバー 直接太さを更新
    for (int i=0; i<num_outer_fiber; i++) {
      auto [p1, p2] = outer_fiber_to_particles[i]; // outer_fiberと接続するparticleの番号, 1細胞ではこれがradial_fiber番号と一致
      outer_fiber_thicknesses(i,0) = calc_thickness_of(radial_fiber_thicknesses(p1,0), radial_fiber_thicknesses(p2,0));
    }

    // 5. 運動方程式と座標rの更新
    // 中心
    auto dr_center = center_particle_forces * (delta_t/viscous_gamma);
    center_particle_positions += dr_center;
    // 外周
    auto dr_outer = outer_particle_forces * (delta_t/viscous_gamma);
    outer_particle_positions += dr_outer;

    // 6. 描画
    // ファイバー（赤）
    for(int i=0; i<num_radial_fiber; i++) {
      drawer.draw_fiber(center_particle_positions, outer_particle_positions.row(i), radial_fiber_thicknesses(i,0));
    }
    for(int i=0; i<num_outer_fiber; i++) {
      auto [p1, p2] = outer_fiber_to_particles[i];
      drawer.draw_fiber(outer_particle_positions.row(p1), outer_particle_positions.row(p2), outer_fiber_thicknesses(i,0));
    }
    // 粒子（緑）
    drawer.draw_particle(center_particle_positions);
    for(int i=0; i<num_outer_particle; i++) drawer.draw_particle(outer_particle_positions.row(i));

    // step表示
    drawer.show_param(25, 50, 0.6, "Step: "+ std::to_string(step));
    drawer.show("PF-model");

    // 7. 画像保存
    if (step % 10 == 0 ) {
      drawer.save_frame(image_save_flg, int(step), folder_path);
    }

    step ++;
  }

  return 0;
}