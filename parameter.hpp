#ifndef PARAMETER_HPP
#define PARAMETER_HPP

#include <cmath>

// パラメータはここに記載する inline constexprで重複なし定義ができる

// 各粒子とファイバーの数
inline constexpr int num_center_particle = 1; //中心粒子
inline constexpr int num_peripheral_particle = 6; //周辺粒子
inline constexpr int num_radial_fiber = 6; //動径ファイバー
inline constexpr int num_peripheral_fiber = 6; //外周ファイバー

// ファイバーの初期値関連
inline constexpr double r0_r = 14; // 動径ファイバー自然長
inline constexpr double r0_p = 2*r0_r*sin(M_PI/num_peripheral_particle); // 外周ファイバー自然長
inline constexpr double init_rf_thickness = 0.5; // 動径ファイバー初期太さ
inline constexpr double init_pf_thickness = 0.5; // 外周ファイバー初期太さ

// 運動方程式、力関連
inline constexpr double viscous_gamma = 10; // 粘性抵抗係数
inline constexpr double c_r = 12; // 単位太さあたり伸展力(動径)
inline constexpr double w_r = 2; // 単位太さあたり収縮力(動径)
inline constexpr double w_p = 4; // 単位太さあたり収縮力(外周)
inline constexpr double k_r_plus = 0.5; // 復元力バネ係数の＋(動径)
inline constexpr double k_r_minus = 1; // 復元力バネ係数の－(動径)
inline constexpr double k_p_plus = 0.4; // 復元力バネ係数の＋(外周)
inline constexpr double k_p_minus = 1; // 復元力バネ係数の－(外周)

//　描画関連(modelの挙動に関係しない)
inline constexpr double init_particle_radius = 2; // 粒子の初期大きさ(接着かどうかで変わる、変更予定)
inline constexpr double max_time = 10000; // 最大シミュレーション時間

#endif