#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <cmath>
#include "vector.hpp"

class Particle {

  public:
      // radius: 粒子半径　id: 粒子番号
      // position: 粒子位置、xとyの2次元ベクトルとして持たせる
      // 参照時はposition.xなど
      Vector2D position;
      double radius;
      int id;

      // メソッド
      double get_x();
      double get_y();
      void set_position(double m, double n);

  };

#endif