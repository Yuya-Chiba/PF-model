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
      void set_radius(double radius);
      double get_x();
      double get_y();
      double get_radius();
      int get_id();
      Particle add(Particle a, Particle b);

  };

#endif