#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <cmath>
#include <vector>
#include "particle.hpp"
#include "fiber.hpp"
#include "vector.hpp"
#include <iostream>

// 各ファイバーそれぞれで力を計算し、両端の粒子に力(ベクトル)を足し合わせる

// 収縮力を計算する
void calc_contraction_force(std::vector<Particle>& particle_array, const std::vector<Fiber>& fiber_array);

// 復元力を計算する
void calc_restoring_force(std::vector<Particle>& particle_array, const std::vector<Fiber>& fiber_array);

// 伸展力を計算する
void calc_extension_force(std::vector<Particle>& particle_array, const std::vector<Fiber>& fiber_array);

#endif