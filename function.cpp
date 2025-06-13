#include "function.hpp"

// 初期配置メソッド
// 2つのparticleを結んで1つのfiberを返す
Fiber unit_particle(Particle p1, Particle p2, double fiber_thickness) {
  Fiber f;
  f.thickness = fiber_thickness;
  *f.particle1 = p1;
  *f.particle2 = p2;
  return f;
}

// 中心1粒子+周囲6粒子の初期配置を設定する, idもここで設定
void set_regular_hexagon(
  std::vector<Particle>& center_particle_array, 
  std::vector<Particle>& peripheral_particle_array, 
  std::vector<Fiber>& radial_fiber_array,
  std::vector<Fiber>& peripheral_fiber_array
) {
  const int center_index = 0;
  const int num_surround = 6;

  particles.resize(1 + num_surround);  // 中心 + 周囲6粒子
  fibers.clear();

  // 中心粒子
  particles[center_index].id = center_index;
  particles[center_index].set_position(center_x, center_y);
  particles[center_index].radius = radius;

  // 周囲粒子
  double r = natural_length;
  for (int i = 0; i < num_surround; ++i) {
    double angle = M_PI / 3 * i;  // 60度ずつ
    double x = center_x + r * cos(angle);
    double y = center_y + r * sin(angle);
    int idx = i + 1;

    particles[idx].id = idx;
    particles[idx].set_position(x, y);
    particles[idx].radius = radius;

    // 中心とのファイバー
    Fiber f;
    f.particle1 = &particles[center_index];
    f.particle2 = &particles[idx];
    f.thickness = 0.5;
    f.id = fibers.size();
    fibers.push_back(f);
  }

  // 周囲同士を繋ぐファイバー（オプション）
  for (int i = 1; i <= num_surround; ++i) {
    int next = (i % num_surround) + 1;
    Fiber f;
    f.particle1 = &particles[i];
    f.particle2 = &particles[next];
    f.thickness = 0.5;
    f.id = fibers.size();
    fibers.push_back(f);
  }
}



// 各ファイバーそれぞれで力を計算し、両端の粒子に力(ベクトル)を足し合わせる

// 動径方向の収縮力を計算する
void calc_contraction_force(std::vector<Particle>& particle_array, const std::vector<Fiber>& fiber_array) {
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
void calc_restoring_force(std::vector<Particle>& particle_array, const std::vector<Fiber>& fiber_array) {
  for (int i = 0; i < fiber_array.size(); i++) {
    Fiber f = fiber_array[i];
    
    Vector2D v; // ベクトル計算用、実際に値が入るわけではない
    Vector2D fiber = v.substract((*f.particle1).position, (*f.particle2).position); // ファイバーをベクトル表記に
    Vector2D fiber_norm = v.normalize(fiber); // 単位ベクトルe
    double fiber_length = fiber.length();

    double r0 = 200; // 自然長、後で別ファイルに移植
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
void calc_extension_force(std::vector<Particle>& particle_array, const std::vector<Fiber>& fiber_array) {
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