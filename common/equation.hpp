#ifndef EQUATION_HPP
#define EQUATION_HPP

#include "parameter.hpp"

// ファイバーにかかる力に関する関数 1ファイバーが対象
// 収縮力を計算する
Eigen::Array<double, 1, 2> calc_contraction_force_rf(const Eigen::Array<double, 1, 2>& p1, const Eigen::Array<double, 1, 2>& p2, const double thickness); // 動径
Eigen::Array<double, 1, 2> calc_contraction_force_of(const Eigen::Array<double, 1, 2>& p1, const Eigen::Array<double, 1, 2>& p2, const double thickness); // 外周
// 復元力を計算する
Eigen::Array<double, 1, 2> calc_restoring_force_rf(const Eigen::Array<double, 1, 2>& p1, const Eigen::Array<double, 1, 2>& p2); // 動径
Eigen::Array<double, 1, 2> calc_restoring_force_of(const Eigen::Array<double, 1, 2>& p1, const Eigen::Array<double, 1, 2>& p2); // 外周
// 伸展力を計算する 動径のみ
Eigen::Array<double, 1, 2> calc_extension_force(const Eigen::Array<double, 1, 2>& p1, const Eigen::Array<double, 1, 2>& p2, const double thickness);

// ファイバー成長に関する関数
double calc_thickness_variation_rf(const Eigen::Array<double, 1, 2>& p1, const Eigen::Array<double, 1, 2>& p2, const double thickness); // 動径ファイバーのdq/dtを計算する
double calc_thickness_of(const double f1_thickness, const double f2_thickness); // 両端粒子につながる動径ファイバー2つから、外周ファイバーの太さを計算する

#endif