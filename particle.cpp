#include "particle.hpp"

// 粒子を特定座標にセット　初期座標設定用 
void Particle::set_position(double m, double n) {
  position.x = m;
  position.y = n;
}
