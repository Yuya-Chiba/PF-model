#include "equation.hpp"
#include "parameter.hpp"

// ファイバーにかかる力に関する関数
// 収縮力(動径方向)
Eigen::Array<double, 1, 2> calc_contraction_force_rf(const Eigen::Array<double, 1, 2>& p1, const Eigen::Array<double, 1, 2>& p2, const double thickness) {
  // 位置座標単位ベクトル化用
  double fiber_length = (p1-p2).matrix().norm();

  Eigen::Array<double, 1, 2> force_vector = (p1-p2)/fiber_length * thickness * w_r;
  return force_vector;
}
// 収縮力(外周方向)
Eigen::Array<double, 1, 2> calc_contraction_force_of(const Eigen::Array<double, 1, 2>& p1, const Eigen::Array<double, 1, 2>& p2, const double thickness) {
  double fiber_length = (p1-p2).matrix().norm();

  Eigen::Array<double, 1, 2> force_vector = (p1-p2)/fiber_length * thickness * w_p;
  return force_vector;
}

// 復元力(動径方向)
Eigen::Array<double, 1, 2> calc_restoring_force_rf(const Eigen::Array<double, 1, 2>& p1, const Eigen::Array<double, 1, 2>& p2) {
  double fiber_length = (p1-p2).matrix().norm();

  Eigen::Array<double, 1, 2> force_vector;
  if (fiber_length > init_rf_length) {
    force_vector = (p1-p2)/fiber_length * k_r_plus*(fiber_length - init_rf_length);
  } else {
    force_vector = (p1-p2)/fiber_length * k_r_minus*(fiber_length - init_rf_length);
  }
  return force_vector;
}
// 復元力(外周方向)
Eigen::Array<double, 1, 2> calc_restoring_force_of(const Eigen::Array<double, 1, 2>& p1, const Eigen::Array<double, 1, 2>& p2) {
  double fiber_length = (p1-p2).matrix().norm();

  Eigen::Array<double, 1, 2> force_vector;
  if (fiber_length > init_of_length) {
    force_vector = (p1-p2)/fiber_length * k_p_plus*(fiber_length - init_of_length);
  } else {
    force_vector = (p1-p2)/fiber_length * k_p_minus*(fiber_length - init_of_length);
  }
  return force_vector;
}

// 伸展力を計算する 動径方向のみ
Eigen::Array<double, 1, 2> calc_extension_force(const Eigen::Array<double, 1, 2>& p1, const Eigen::Array<double, 1, 2>& p2, const double thickness) {
  double fiber_length = (p1-p2).matrix().norm();

  Eigen::Array<double, 1, 2> force_vector = - (p1-p2)/fiber_length * thickness * c_r;
  return force_vector;
}


// ここからファイバーの成長に関する関数
// 動径ファイバーのdq/dtを計算する ここでは1ファイバーに対して計算
double calc_thickness_variation_rf(const Eigen::Array<double, 1, 2>& p1, const Eigen::Array<double, 1, 2>& p2, const double thickness) {
  double fiber_length = (p1-p2).matrix().norm();
  
  // 粒子を中心に引く力G_i0の計算
  Eigen::Array<double, 1, 2> t_i = calc_contraction_force_rf(p1, p2, thickness);
  Eigen::Array<double, 1, 2> h_i;
  if (fiber_length > init_rf_length) {
    h_i = calc_restoring_force_rf(p1, p2); // 自然長より長いなら復元力が働く
  } else {
    h_i.setZero();
  }
  double g_i = (t_i + h_i).matrix().norm();

  // 成長項u(G)の計算
  double u_g = u_hat * std::pow((g_i/g_h),coop_mu) / (1 + std::pow((g_i/g_h),coop_mu));
  // 分解項とあわせて計算
  double dqdt = u_g - damping_coefficient * (thickness - q_min);

  return dqdt;
}

// 両端粒子につながる動径ファイバー2つの太さから、外周ファイバーの太さを計算する
double calc_thickness_of(const double f1_thickness, const double f2_thickness) {
  double q_bar = (f1_thickness + f2_thickness)/2; // 両端ファイバー太さの平均

  // 外周ファイバーの太さ
  double q_ij = q_hat_p * std::pow((q_bar/q_h),coop_xi) / (1 + std::pow((q_bar/q_h),coop_xi));
  return q_ij;
}