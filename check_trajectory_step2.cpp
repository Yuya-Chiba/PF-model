#include "common/draw.hpp"
#include "common/equation.hpp"
#include "common/function.hpp"
#include "common/parameter.hpp"

// Step2: 初期位置からモデルを動かして平衡解を探す

int main() {
  // 粒子とファイバーの接続情報 複数細胞用に拡張可能
  std::vector<std::pair<int, int>> outer_fiber_to_particles = {
    {0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 0} };

  // 描画・ファイル保存用設定
  Drawer drawer;
  bool draw_flg = false;
  bool image_save_flg = false;
  std::string image_folder_path = "../image";

  // 入出力データ保存用設定
  std::vector<Eigen::Array<double, num_center_particle, 2>> init_cp_positions_vector;
  std::vector<Eigen::Array<double, num_outer_particle, 2>> init_op_positions_vector;
  std::vector<Eigen::Array<double, num_radial_fiber, 1>> init_rf_thicknesses_vector;
  read_position_thickness_from_csv("../csv/result/solution_init.csv", init_cp_positions_vector, init_op_positions_vector, init_rf_thicknesses_vector);
  std::ofstream outputfile_trajectory("../csv/result/trajectory.csv");
  std::ofstream outputfile_solutions("../csv/result/equilibrium_solutions.csv");
  set_csv_header(outputfile_trajectory); // 出力用ファイルヘッダー設定
  set_csv_header(outputfile_solutions);

  // 全2635パターンについてシミュレーションを回す
  // 条件として、全外周粒子位置および動径ファイバーの太さの変位が1.0E-5以下になったときに定常とみなす
  for (int j=0; j<init_rf_thicknesses_vector.size(); j++) {
    std::cout << "now_pattern: " << j+1 << "/2635" << std::endl;

    // 粒子、ファイバー関連の宣言・初期化
    auto center_particle_positions = init_cp_positions_vector[j]; // 中心粒子位置
    auto outer_particle_positions = init_op_positions_vector[j]; // 周辺粒子位置
    auto radial_fiber_thicknesses = init_rf_thicknesses_vector[j]; // 動径ファイバー太さ
    Eigen::Array<double, num_outer_fiber, 1> outer_fiber_thicknesses; // 外周ファイバー太さ 動径から直接計算
    for (int i=0; i<num_outer_fiber; i++) {
      auto [p1, p2] = outer_fiber_to_particles[i];
      outer_fiber_thicknesses(i,0) = calc_thickness_of(radial_fiber_thicknesses(p1,0), radial_fiber_thicknesses(p2,0));
    }
    Eigen::Array<double, num_center_particle, 2> center_particle_forces; // 中心粒子に働く力
    Eigen::Array<double, num_outer_particle, 2> outer_particle_forces; // 周辺粒子に働く力
    
    const double threshold = 1e-5; // 閾値

    int step = 1;
    while (step <= max_step) {
      if (draw_flg) drawer.clear();
      // 粒子にかかる力をリセットする
      center_particle_forces.setZero();
      outer_particle_forces.setZero();

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
      Eigen::Array<double, num_radial_fiber, 1> delta_thickness; // 前stepからの差分、収束判定用
      for (int i=0; i<num_radial_fiber; i++) {
        double dq_radial = calc_thickness_variation_rf(center_particle_positions, outer_particle_positions.row(i), radial_fiber_thicknesses(i,0)) * delta_t;
        radial_fiber_thicknesses(i,0) += dq_radial;
        delta_thickness(i,0) = dq_radial;
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

      // 解軌道の記録
      if (step % 10 == 0) write_to_csv(outputfile_trajectory, j+1, center_particle_positions, outer_particle_positions, radial_fiber_thicknesses, outer_fiber_thicknesses);

      // 収束判定, 中心粒子に対する相対座標を記録
      Eigen::Array<double, num_outer_particle, 2> displacement; // 変位、収束判定用
      for(int i=0; i<num_outer_particle; i++) {
        displacement.row(i) = dr_outer.row(i) - dr_center;
      }
      bool all_stable = (displacement.abs() < threshold).all() && (delta_thickness.abs() < threshold).all();
      if (all_stable) {
        write_to_csv(outputfile_solutions, j+1, center_particle_positions, outer_particle_positions, radial_fiber_thicknesses, outer_fiber_thicknesses);
        break;
      } else if (step >= 10000) {
        std::cout << "Not found solution" << std::endl;
        break;
      }


      // 6. 描画
      if (draw_flg) {
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
      }
      // 7. 画像保存
      if (step % 10 == 0) {
        drawer.save_frame(image_save_flg, int(step), image_folder_path);
      }

      step ++;
    }

  }

  return 0;
}