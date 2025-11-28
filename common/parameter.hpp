#ifndef PARAMETER_HPP
#define PARAMETER_HPP

#include "lib.cpp"

// パラメータはここに記載する inline constexprで重複なし定義ができる

// 各粒子とファイバーの数
inline constexpr int num_center_particle = 1; //中心粒子
inline constexpr int num_outer_particle = 6; //周辺粒子
inline constexpr int num_radial_fiber = 6; //動径ファイバー
inline constexpr int num_outer_fiber = 6; //外周ファイバー

// ファイバーの初期値関連
inline constexpr double init_rf_length = 14; // 動径ファイバー自然長
inline constexpr double angle = 2 * M_PI / num_outer_particle; // 動径ファイバー間角度
inline constexpr double init_of_length = 2 * init_rf_length * sin(angle/2); // 外周ファイバー自然長
inline constexpr double init_rf_thickness = 1; // 動径ファイバー初期太さ
inline constexpr double init_of_thickness = 1; // 外周ファイバー初期太さ

// 運動方程式、力関連
inline constexpr double viscous_gamma = 10; // 粘性抵抗係数
inline constexpr double c_r = 12; // 単位太さあたり伸展力(動径)
inline constexpr double w_r = 2; // 単位太さあたり収縮力(動径)
inline constexpr double w_p = 4; // 単位太さあたり収縮力(外周)
inline constexpr double k_r_plus = 0.5; // 復元力バネ係数の＋(動径)
inline constexpr double k_r_minus = 1; // 復元力バネ係数の－(動径)
inline constexpr double k_p_plus = 0.4; // 復元力バネ係数の＋(外周)
inline constexpr double k_p_minus = 1; // 復元力バネ係数の－(外周)

// ファイバーの成長方程式関連
inline constexpr double u_hat = 0.1; // 動径ファイバー成長速度上限
inline constexpr double g_h = 1.5; // 動径ファイバー成長速度関数の解離定数
inline constexpr double coop_mu = 3; // 動径ファイバー成長速度関数の協同性
inline constexpr double damping_coefficient = 0.1; // 動径ファイバー減衰係数
inline constexpr double q_min = 0.05; // 動径ファイバー太さ最小値
inline constexpr double q_hat_p = 1; // 外周ファイバー成長上限
inline constexpr double q_h = 0.75; // 外周ファイバー成長式の解離定数
inline constexpr double coop_xi = 20; // 外周ファイバー成長式の協同性

// 接着脱離関連
inline constexpr double q_threshold = 0.25; // 動径ファイバー太さ(接着斑大きさ)閾値, これを超えると仮足とみなす

//　描画関連(modelの挙動に関係しない)
inline constexpr double init_particle_radius = 2; // 粒子の初期大きさ(接着かどうかで変わる、変更予定)
inline constexpr int max_step = 30000; // 最大シミュレーション時間
inline constexpr double delta_t = 0.1;

#endif