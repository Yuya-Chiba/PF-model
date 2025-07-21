#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <cmath>
#include <vector>
#include <array>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "particle.hpp"
#include "fiber.hpp"
#include "vector.hpp"

// 太さ全パターン列挙csvファイルを読み込む
std::vector<std::array<double, 6>> read_thickness_patterns_from_csv(const std::string& filepath);

// 初期配置メソッド
// 2つのparticleを結んで1つのfiberを返す
Fiber unit_particle(Particle& p1, Particle& p2, double fiber_thickness);
// 中心1粒子+周囲6粒子の初期配置を設定する, idもここで設定
void set_regular_hexagon(
  std::vector<Particle>& center_particle_array, 
  std::vector<Particle>& peripheral_particle_array, 
  std::vector<Fiber>& radial_fiber_array,
  std::vector<Fiber>& peripheral_fiber_array
);

// 各ファイバーそれぞれで力を計算する(ファイバー両端の粒子それぞれに対応した力のペア)
// 収縮力を計算する
Vector2D calc_contraction_force_rf(const Fiber& f); // 動径
Vector2D calc_contraction_force_pf(const Fiber& f); // 外周
// 復元力を計算する
Vector2D calc_restoring_force_rf(const Fiber& f); // 動径
Vector2D calc_restoring_force_pf(const Fiber& f); // 外周
// 伸展力を計算する
Vector2D calc_extension_force(const Fiber& f);

// ファイバーについて計算した力を、その両端の粒子に足す
void add_radial_forces (const Fiber& f); // 動径
void add_peripheral_forces (const Fiber& f); // 外周

// ここからファイバーの成長に関する関数
double calc_thickness_variation_rf(const Fiber& f); // 動径ファイバーのdq/dtを計算する
double calc_thickness_pf(const Fiber& f1, const Fiber& f2); // 両端粒子につながる動径ファイバー2つから、外周ファイバーの太さを計算する
// 外周ファイバーの両端粒子につながる動径ファイバー2本を探す
std::vector<Fiber> find_connected_radial_fibers(
    const Fiber& pf,
    const std::vector<Fiber>& rf_array
);

#endif