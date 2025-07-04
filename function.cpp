#include "function.hpp"
#include "parameter.hpp"

// 初期配置メソッド
// 2つのparticleを結んで1つのfiberを返す
Fiber unit_particle(Particle& p1, Particle& p2, double fiber_thickness) {
  Fiber f;
  f.thickness = fiber_thickness;
  f.particle1 = &p1;
  f.particle2 = &p2;
  return f;
}

// 中心1粒子+周囲6粒子の初期配置を設定する, idもここで設定
void set_regular_hexagon(
  std::vector<Particle>& center_particle_array, 
  std::vector<Particle>& peripheral_particle_array, 
  std::vector<Fiber>& radial_fiber_array,
  std::vector<Fiber>& peripheral_fiber_array
) {

  // 粒子、ファイバー数はパラメータファイルに記載
  // 配列長の変更、fiberは1つずつpush_backするのでclearで0にする
  center_particle_array.resize(num_center_particle);
  peripheral_particle_array.resize(num_peripheral_particle);
  radial_fiber_array.clear();
  peripheral_fiber_array.clear();

  // 中心粒子の設置
  center_particle_array[0].id = 0;
  center_particle_array[0].set_position(0, 0);
  center_particle_array[0].radius = init_particle_radius;

  // 周囲粒子
  for (int i = 0; i < num_peripheral_particle; i++) {
    double angle = 2 * M_PI / num_peripheral_particle * i;  // 6粒子だと60度ずつの設置
    double x = r0_r * cos(angle);
    double y = r0_r * sin(angle);

    peripheral_particle_array[i].id = i;
    peripheral_particle_array[i].set_position(x, y);
    peripheral_particle_array[i].radius = init_particle_radius;

    // ファイバーを中心粒子とつなぐ
    Fiber f = unit_particle(center_particle_array[0], peripheral_particle_array[i], init_rf_thickness);
    f.id = radial_fiber_array.size();
    radial_fiber_array.push_back(f);
  }

  // 周囲粒子同士をファイバーでつなぐ
  for (int i = 0; i < num_peripheral_fiber; i++) {
    int j = (i + 1)% num_peripheral_fiber; // 5番目と0番目をつなぐため必要
    Fiber f = unit_particle(peripheral_particle_array[i], peripheral_particle_array[j], init_pf_thickness);
    f.id = peripheral_fiber_array.size();
    peripheral_fiber_array.push_back(f);
  }
}

// 各ファイバーそれぞれで力を計算する(ファイバー両端の粒子それぞれに対応した力のペア)

// 収縮力(動径方向)
Vector2D calc_contraction_force_rf(const Fiber& f) {
  Vector2D fiber = Vector2D::subtract((*f.particle1).position, (*f.particle2).position); // ファイバーをベクトル表記に
  Vector2D fiber_norm = Vector2D::normalize(fiber); // 単位ベクトルe
  double fiber_length = fiber.length();

  // 計算部分
  Vector2D rt;
  rt = Vector2D::multiple(fiber_norm, f.thickness*w_r);
  return rt;
}
// 収縮力(外周方向)
Vector2D calc_contraction_force_pf(const Fiber& f) {    
  Vector2D fiber = Vector2D::subtract((*f.particle1).position, (*f.particle2).position); // ファイバーをベクトル表記に
  Vector2D fiber_norm = Vector2D::normalize(fiber); // 単位ベクトルe
  double fiber_length = fiber.length();

  // 計算部分
  Vector2D rt;
  rt = Vector2D::multiple(fiber_norm, f.thickness*w_p);
  return rt;
}

// 復元力(動径方向)
Vector2D calc_restoring_force_rf(const Fiber& f) {    
  Vector2D fiber = Vector2D::subtract((*f.particle1).position, (*f.particle2).position); // ファイバーをベクトル表記に
  Vector2D fiber_norm = Vector2D::normalize(fiber); // 単位ベクトルe
  double fiber_length = fiber.length();

  // 計算部分
  Vector2D rf;
  if (fiber_length > r0_r) {
    rf = Vector2D::multiple(fiber_norm, k_r_plus*(fiber_length - r0_r));
  } else {
    rf = Vector2D::multiple(fiber_norm, k_r_minus*(fiber_length - r0_r));
  }
  return rf;
}
// 復元力(外周方向)
Vector2D calc_restoring_force_pf(const Fiber& f) {    
  Vector2D fiber = Vector2D::subtract((*f.particle1).position, (*f.particle2).position); // ファイバーをベクトル表記に
  Vector2D fiber_norm = Vector2D::normalize(fiber); // 単位ベクトルe
  double fiber_length = fiber.length();

  // 計算部分
  Vector2D rf;
  if (fiber_length > r0_p) {
    rf = Vector2D::multiple(fiber_norm, k_p_plus*(fiber_length - r0_p));
  } else {
    rf = Vector2D::multiple(fiber_norm, k_p_minus*(fiber_length - r0_p));
  }
  return rf;
}

// 伸展力を計算する 動径方向のみ
Vector2D calc_extension_force(const Fiber& f) {
  Vector2D fiber = Vector2D::subtract((*f.particle1).position, (*f.particle2).position); // ファイバーをベクトル表記に
  Vector2D fiber_norm = Vector2D::normalize(fiber); // 単位ベクトルe
  double fiber_length = fiber.length();

  // 計算部分
  Vector2D rt;
  rt = Vector2D::multiple(fiber_norm, -f.thickness*c_r);
  return rt;
}

// ファイバーについて計算した力を、その両端の粒子に足す
// 動径方向
void add_radial_forces (const Fiber& f) {
  Vector2D restoring_force = calc_restoring_force_rf(f);
  Vector2D contraction_force = calc_contraction_force_rf(f);
  Vector2D extension_force = calc_extension_force(f);

  // 両端粒子にそれぞれ対称な力を加える p1_forceなどは&を使って直接参照する
  Force& force_p1 = (*f.particle1).force;
  Force& force_p2 = (*f.particle2).force;
  force_p1.restoring_radial = Vector2D::add(force_p1.restoring_radial, Vector2D::oppo(restoring_force));
  force_p2.restoring_radial = Vector2D::add(force_p2.restoring_radial, restoring_force);
  force_p1.contraction_radial = Vector2D::add(force_p1.contraction_radial, Vector2D::oppo(contraction_force));
  force_p2.contraction_radial = Vector2D::add(force_p2.contraction_radial, contraction_force);
  force_p1.extension_radial = Vector2D::add(force_p1.extension_radial, Vector2D::oppo(extension_force));
  force_p2.extension_radial = Vector2D::add(force_p2.extension_radial, extension_force);
}
// 外周方向
void add_peripheral_forces (const Fiber& f) {
  Vector2D restoring_force = calc_restoring_force_pf(f);
  Vector2D contraction_force = calc_contraction_force_pf(f);

  // 両端粒子にそれぞれ対称な力を加える p1_forceなどは&を使って直接参照する
  Force& force_p1 = (*f.particle1).force;
  Force& force_p2 = (*f.particle2).force;
  force_p1.restoring_peripheral = Vector2D::add(force_p1.restoring_peripheral, Vector2D::oppo(restoring_force));
  force_p2.restoring_peripheral = Vector2D::add(force_p2.restoring_peripheral, restoring_force);
  force_p1.contraction_peripheral = Vector2D::add(force_p1.contraction_peripheral, Vector2D::oppo(contraction_force));
  force_p2.contraction_peripheral = Vector2D::add(force_p2.contraction_peripheral, contraction_force);
}
// ここからファイバーの成長に関する関数
// 動径ファイバーのdq/dtを計算する ここでは1ファイバーに対して計算
double calc_thickness_variation_rf(const Fiber& f) {

  // u(G)の計算
  return 0;

}