#include "common/draw.hpp"
#include "common/equation.hpp"
#include "common/function.hpp"
#include "common/parameter.hpp"

// Step1: 初期配置を探す、ファイバー太さは固定して粒子位置だけ動かす

int main() {
  // 粒子、ファイバー関連の宣言・初期化
  // 計算で使用する変数（高速化のためEigenを使用）
  Eigen::Array<double, num_center_particle, 2> center_particle_positions; // 中心粒子位置
  Eigen::Array<double, num_outer_particle, 2> outer_particle_positions; // 周辺粒子位置
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
  Drawer drawer;
  bool draw_flg = false;
  bool image_save_flg = false;
  std::string image_folder_path = "../result/image";

  // 入出力データ保存用設定
  auto patterns = read_thickness_patterns_from_csv("./../source/thickness_patterns.csv"); // 太さパターン読み込み
  std::filesystem::create_directories("../result/csv"); // フォルダ無い場合は作成
  std::ofstream outputfile("../result/csv/solution_init.csv");
  set_csv_header(outputfile); // 出力用ファイルヘッダー設定

  // 全2635パターンについてシミュレーションを回す
  // 条件として、全外周粒子および動径ファイバーの太さの変位が1.0E-5以下になったときに定常とみなす
  int now_pattern = 0;
  for (const auto& pattern : patterns) {
    now_pattern++;
    std::cout << "now_pattern: " << now_pattern << "/2635" << std::endl;

    // 粒子位置
    center_particle_positions.setZero();
    outer_particle_positions = set_init_outer_particle();
    // パターン別にファイバー太さ設定を行う 外周は動径から直接計算
    for (int i = 0; i < num_radial_fiber; i++) radial_fiber_thicknesses(i,0) = pattern[i];
    for (int i=0; i<num_outer_fiber; i++) {
      auto [p1, p2] = outer_fiber_to_particles[i]; // outer_fiberと接続するparticleの番号, 1細胞ではこれがradial_fiber番号と一致
      outer_fiber_thicknesses(i,0) = calc_thickness_of(radial_fiber_thicknesses(p1,0), radial_fiber_thicknesses(p2,0));
    }

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

      // 5. 運動方程式と座標rの更新
      // 中心
      auto dr_center = center_particle_forces * (delta_t/viscous_gamma);
      center_particle_positions += dr_center;
      // 外周
      auto dr_outer = outer_particle_forces * (delta_t/viscous_gamma);
      outer_particle_positions += dr_outer;

      // 収束判定用に中心粒子に対する相対座標を記録
      Eigen::Array<double, num_outer_particle, 2> displacement; // 変位、収束判定用
      for(int i=0; i<num_outer_particle; i++){
        displacement.row(i) = dr_outer.row(i) - dr_center;
      }
      // 収束判定
      bool all_stable = (displacement.abs() < threshold).all();
      if (all_stable || step >= 10000) {
        write_to_csv(outputfile, center_particle_positions, outer_particle_positions, radial_fiber_thicknesses, outer_fiber_thicknesses);
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
      if (step % 10 == 0 ) {
        drawer.save_frame(image_save_flg, int(step), image_folder_path);
      }

      step ++;
    }

  }

  return 0;
}