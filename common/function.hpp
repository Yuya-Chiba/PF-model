#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include "parameter.hpp"

// 周囲6粒子の初期配置を設定
Eigen::Array<double, num_outer_particle, 2> set_init_outer_particle();

// 太さ全パターン列挙csvファイルを読み込む
std::vector<std::array<double, 6>> read_thickness_patterns_from_csv(const std::string& filepath);

#endif