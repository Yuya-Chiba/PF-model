#include "function.hpp"

// 各ファイバーそれぞれで力を計算し、両端の粒子に力(ベクトル)を足し合わせる

// 収縮力を計算する
void calc_contraction_force(std::vector<Particle>& particle_array, const std::vector<Fiber>& fiber_array) {

}

// 動径方向の復元力を計算する
void calc_restoring_force(std::vector<Particle>& particle_array, const std::vector<Fiber>& fiber_array) {
  for (int i = 0; i < fiber_array.size(); i++) {
    Fiber f = fiber_array[i];
    Vector2D p1_position = (*f.particle1).position;
    Vector2D p2_position = (*f.particle2).position;
    
    Vector2D v; // ベクトル計算用、実際に値が入るわけではない
    Vector2D fiber = v.substract(p1_position, p2_position); // ファイバーをベクトル表記に
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
    (*f.particle1).force = v.oppo(rf);
    (*f.particle2).force = rf;
  }
}

// 伸展力を計算する
void calc_extension_force(std::vector<Particle>& particle_array, const std::vector<Fiber>& fiber_array) {

}