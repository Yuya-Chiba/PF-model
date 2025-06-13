#include "function.hpp"

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
  // 各粒子とファイバーの数
  const int num_center_particle = 1;
  const int num_peripheral_particle = 6;
  const int num_radial_fiber = 6;
  const int num_peripheral_fiber = 6;

  // 配列長の変更、fiberは1つずつpush_backするのでclearで0にする
  center_particle_array.resize(num_center_particle);
  peripheral_particle_array.resize(num_peripheral_particle);
  radial_fiber_array.clear();
  peripheral_fiber_array.clear();

  // 中心粒子の設置
  center_particle_array[0].id = 0;
  center_particle_array[0].set_position(0, 0);
  center_particle_array[0].radius = 2;

  // 周囲粒子
  double r = 14; // 動径ファイバー自然長(parameter.cpp移行予定)
  for (int i = 0; i < num_peripheral_particle; i++) {
    double angle = M_PI / 3 * i;  // 60度ずつの設置
    double x = r * cos(angle);
    double y = r * sin(angle);

    peripheral_particle_array[i].id = i;
    peripheral_particle_array[i].set_position(x, y);
    peripheral_particle_array[i].radius = 2;

    // ファイバーを中心粒子とつなぐ
    double fiber_thickness = 0.5; // 動径ファイバー初期太さ(parameter.cpp移行予定)
    Fiber f = unit_particle(center_particle_array[0], peripheral_particle_array[i], fiber_thickness);
    f.id = radial_fiber_array.size();
    radial_fiber_array.push_back(f);
  }

  // 周囲粒子同士をファイバーでつなぐ
  for (int i = 0; i <= num_peripheral_fiber; i++) {
    int j = (i + 1)% num_peripheral_fiber; // 5番目と0番目をつなぐため必要
    double fiber_thickness = 0.5; // 外周ファイバー初期太さ(parameter.cpp移行予定)
    Fiber f = unit_particle(peripheral_particle_array[i], peripheral_particle_array[j], fiber_thickness);
    f.id = peripheral_fiber_array.size();
    peripheral_fiber_array.push_back(f);
  }
}


// 各ファイバーそれぞれで力を計算し、両端の粒子に力(ベクトル)を足し合わせる

// 動径方向の収縮力を計算する
void calc_contraction_force(const std::vector<Fiber>& fiber_array) {
  for (int i = 0; i < fiber_array.size(); i++) {
    Fiber f = fiber_array[i];
    
    Vector2D v; // ベクトル計算用、実際に値が入るわけではない
    Vector2D fiber = v.substract((*f.particle1).position, (*f.particle2).position); // ファイバーをベクトル表記に
    Vector2D fiber_norm = v.normalize(fiber); // 単位ベクトルe
    double fiber_length = fiber.length();

    double wr = 2; // 単位太さあたりの力

    // 計算部分
    Vector2D rt;
    rt = v.multiple(fiber_norm, f.thickness*wr);
    (*f.particle1).force = v.add((*f.particle1).force, v.oppo(rt));
    (*f.particle2).force = v.add((*f.particle2).force, rt);
  }
}

// 動径方向の復元力を計算する
void calc_restoring_force(const std::vector<Fiber>& fiber_array) {
  for (int i = 0; i < fiber_array.size(); i++) {
    Fiber f = fiber_array[i];
    
    Vector2D v; // ベクトル計算用、実際に値が入るわけではない
    Vector2D fiber = v.substract((*f.particle1).position, (*f.particle2).position); // ファイバーをベクトル表記に
    Vector2D fiber_norm = v.normalize(fiber); // 単位ベクトルe
    double fiber_length = fiber.length();

    double r0 = 14; // 自然長、後で別ファイルに移植
    double k_plus = 0.5;
    double k_minus = 1;

    // 計算部分
    Vector2D rf;
    if (fiber_length > r0) {
      rf = v.multiple(fiber_norm, k_plus*(fiber_length - r0));
    } else {
      rf = v.multiple(fiber_norm, k_minus*(fiber_length - r0));
    }
    (*f.particle1).force = v.add((*f.particle1).force, v.oppo(rf));
    (*f.particle2).force = v.add((*f.particle2).force, rf);
  }
}

// 伸展力を計算する 動径方向のみ
void calc_extension_force(const std::vector<Fiber>& fiber_array) {
  for (int i = 0; i < fiber_array.size(); i++) {
    Fiber f = fiber_array[i];
    
    Vector2D v; // ベクトル計算用、実際に値が入るわけではない
    Vector2D fiber = v.substract((*f.particle1).position, (*f.particle2).position); // ファイバーをベクトル表記に
    Vector2D fiber_norm = v.normalize(fiber); // 単位ベクトルe
    double fiber_length = fiber.length();

    double c = 12; // 単位太さあたりの力

    // 計算部分
    Vector2D rt;
    rt = v.multiple(fiber_norm, -f.thickness*c);
    (*f.particle1).force = v.add((*f.particle1).force, v.oppo(rt));
    (*f.particle2).force = v.add((*f.particle2).force, rt);
  }
}