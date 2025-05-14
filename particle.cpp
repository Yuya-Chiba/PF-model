#include "particle.hpp"

// 粒子を特定座標,半径にセット
void Particle::set_xy(double m, double n) {
  x = m;
  y = n;
}
void Particle::set_radius(double r) {
  radius = r;
}

// 粒子の座標取得 x,y 半径取得 radius
double Particle::get_x() {
  return x;
}
double Particle::get_y() {
  return y;
}
double Particle::get_radius() {
  return radius;
}

// 粒子番号idの取得
int Particle::get_id() {
  return id;
}

// 2粒子の足し算
Particle add(Particle a, Particle b) {
  Particle c;
  c.set_xy(a.get_x() + b.get_x(), a.get_y() + b.get_y());
  return c;
}