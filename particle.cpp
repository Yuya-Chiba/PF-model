#include "particle.hpp"

// x,y座標をpositionを通さず直接取得
double Particle::get_x() { return position.x; }
double Particle::get_y() { return position.y; }

// 粒子を特定座標にセット　初期座標設定用 
void Particle::set_position(double m, double n) {
  position.x = m;
  position.y = n;
}
