#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include "parameter.hpp"

// 周囲6粒子の初期配置を設定
Eigen::Array<double, num_outer_particle, 2> set_init_outer_particle();

// 太さ全パターン列挙csvファイルを読み込む
std::vector<std::array<double, 6>> read_thickness_patterns_from_csv(const std::string& filepath);

// 出力用csvファイルのヘッダー設定
void set_csv_header(std::ofstream& file);
// 粒子位置, ファイバー太さの書き出し 1行につき1slice
void write_to_csv(
  std::ofstream& file,
  const Eigen::Array<double, num_center_particle, 2> center_particle_positions,
  const Eigen::Array<double, num_outer_particle, 2> outer_particle_positions,
  const Eigen::Array<double, num_radial_fiber, 1> radial_fiber_thicknesses,
  const Eigen::Array<double, num_outer_fiber, 1> outer_fiber_thicknesses
);

#endif