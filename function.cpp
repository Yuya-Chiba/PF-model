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
  for (int i = 0; i <= num_peripheral_fiber; i++) {
    int j = (i + 1)% num_peripheral_fiber; // 5番目と0番目をつなぐため必要
    Fiber f = unit_particle(peripheral_particle_array[i], peripheral_particle_array[j], init_pf_thickness);
    f.id = peripheral_fiber_array.size();
    peripheral_fiber_array.push_back(f);
  }
}


// 各ファイバーそれぞれで力を計算し、両端の粒子に力(ベクトル)を足し合わせる

// 収縮力(動径方向)
void calc_contraction_force_rf(const std::vector<Fiber>& fiber_array) {
  for (int i = 0; i < fiber_array.size(); i++) {
    Fiber f = fiber_array[i];
    
    Vector2D v; // ベクトル計算用、実際に値が入るわけではない
    Vector2D fiber = v.substract((*f.particle1).position, (*f.particle2).position); // ファイバーをベクトル表記に
    Vector2D fiber_norm = v.normalize(fiber); // 単位ベクトルe
    double fiber_length = fiber.length();

    // 計算部分
    Vector2D rt;
    rt = v.multiple(fiber_norm, f.thickness*w_r);
    (*f.particle1).force = v.add((*f.particle1).force, v.oppo(rt));
    (*f.particle2).force = v.add((*f.particle2).force, rt);
  }
}
// 収縮力(外周方向)
void calc_contraction_force_pf(const std::vector<Fiber>& fiber_array) {
  for (int i = 0; i < fiber_array.size(); i++) {
    Fiber f = fiber_array[i];
    
    Vector2D v; // ベクトル計算用、実際に値が入るわけではない
    Vector2D fiber = v.substract((*f.particle1).position, (*f.particle2).position); // ファイバーをベクトル表記に
    Vector2D fiber_norm = v.normalize(fiber); // 単位ベクトルe
    double fiber_length = fiber.length();

    // 計算部分
    Vector2D rt;
    rt = v.multiple(fiber_norm, f.thickness*w_p);
    (*f.particle1).force = v.add((*f.particle1).force, v.oppo(rt));
    (*f.particle2).force = v.add((*f.particle2).force, rt);
  }
}

// 復元力(動径方向)
void calc_restoring_force_rf(const std::vector<Fiber>& fiber_array) {
  for (int i = 0; i < fiber_array.size(); i++) {
    Fiber f = fiber_array[i];
    
    Vector2D v; // ベクトル計算用、実際に値が入るわけではない
    Vector2D fiber = v.substract((*f.particle1).position, (*f.particle2).position); // ファイバーをベクトル表記に
    Vector2D fiber_norm = v.normalize(fiber); // 単位ベクトルe
    double fiber_length = fiber.length();

    // 計算部分
    Vector2D rf;
    if (fiber_length > r0_r) {
      rf = v.multiple(fiber_norm, k_r_plus*(fiber_length - r0_r));
    } else {
      rf = v.multiple(fiber_norm, k_r_minus*(fiber_length - r0_r));
    }
    (*f.particle1).force = v.add((*f.particle1).force, v.oppo(rf));
    (*f.particle2).force = v.add((*f.particle2).force, rf);
  }
}
// 復元力(外周方向)
void calc_restoring_force_pf(const std::vector<Fiber>& fiber_array) {
  for (int i = 0; i < fiber_array.size(); i++) {
    Fiber f = fiber_array[i];
    
    Vector2D v; // ベクトル計算用、実際に値が入るわけではない
    Vector2D fiber = v.substract((*f.particle1).position, (*f.particle2).position); // ファイバーをベクトル表記に
    Vector2D fiber_norm = v.normalize(fiber); // 単位ベクトルe
    double fiber_length = fiber.length();

    // 計算部分
    Vector2D rf;
    if (fiber_length > r0_p) {
      rf = v.multiple(fiber_norm, k_p_plus*(fiber_length - r0_p));
    } else {
      rf = v.multiple(fiber_norm, k_p_minus*(fiber_length - r0_p));
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

    // 計算部分
    Vector2D rt;
    rt = v.multiple(fiber_norm, -f.thickness*c_r);
    (*f.particle1).force = v.add((*f.particle1).force, v.oppo(rt));
    (*f.particle2).force = v.add((*f.particle2).force, rt);
  }
}