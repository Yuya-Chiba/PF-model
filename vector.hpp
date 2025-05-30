#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cmath>

// ベクトル演算を行うための汎用クラス std::vector(可変長配列)とは別物

class Vector2D {

  public:
    // xy座標を持たせる
    double x, y;

    // ベクトルのxy座標をsetする
    void set(double a, double b);
    // ベクトルの長さを求める
    double length();
    // ベクトルの単位ベクトルを返す
    Vector2D normalize(Vector2D p);
    // ベクトルの和
    Vector2D add(Vector2D p, Vector2D q);
    // (始点が同じ)2ベクトル間の(終点の)距離
    double distance(Vector2D p, Vector2D q);
      
  };

#endif