#ifndef FIBER_HPP
#define FIBER_HPP

#include <cmath>
#include "particle.hpp"

class Fiber {

  public:
      // thickness: ファイバー太さ id: ファイバー番号
      // particle1,particle2: 両端の粒子、ポインタで指定することで内部値を直接参照できる 
      // 参照時は(*f.particle1).position.xなど
      double thickness;
      int id;
      Particle* particle1;
      Particle* particle2;

      // メソッド
      // int get_id(int p1_id, int p2_id);
      // int get_particle_id();
      
  };

#endif