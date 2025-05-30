#include <cmath>
#include <vector>
#include "particle.hpp"
#include "fiber.hpp"
#include "vector.hpp"

// 各ファイバーそれぞれで力を計算し、両端の粒子に力(ベクトル)を足し合わせる

// 収縮力を計算する
void calc_contraction_force(std::vector<Particle>& particle_array, const std::vector<Fiber>& fiber_array) {

}

// 復元力を計算する
void calc_restoring_force(std::vector<Particle>& particle_array, const std::vector<Fiber>& fiber_array) {
  for (int i = 0; i < fiber_array.size(); i++) {
    // std::cout << "hello" << std::endl;
  }
}

// 伸長力を計算する
void calc_extension_force(std::vector<Particle>& particle_array, const std::vector<Fiber>& fiber_array) {

}