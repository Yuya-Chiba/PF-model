#include "particle.hpp"

// 粒子を特定座標にセット　初期座標設定用 
void Particle::set_xy(double m, double n) {
  x = m;
  y = n;
}

// 2粒子の足し算
Particle add(Particle a, Particle b) {
  Particle c;
  c.set_xy(a.x + b.x, a.y + b.y);
  return c;
}