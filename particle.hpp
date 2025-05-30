#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <cmath>

class Particle {

  public:
      // x,y: 粒子の座標  radius: 粒子半径　id: 粒子番号
      double x, y;
      double radius;
      int id;

      // メソッド
      void set_xy(double m, double n);

  };

#endif