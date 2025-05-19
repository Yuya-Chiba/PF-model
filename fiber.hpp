#ifndef FIBER_HPP
#define FIBER_HPP

#include <cmath>

class Fiber {

  public:
      // thickness: ファイバー太さ id: ファイバー番号
      // particle1_id, particle2_id: ファイバーの両辺に接続している粒子のid
      double thickness;
      int id;
      int particle1_id, particle2_id;

      // メソッド
      int get_id(int p1_id, int p2_id);
      // int get_particle_id();
      
  };

#endif