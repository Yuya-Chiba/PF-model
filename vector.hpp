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
    // ベクトルの差
    Vector2D substract(Vector2D p, Vector2D q);
    // ベクトルをn倍にする
    Vector2D multiple(Vector2D p, double n);
    // ベクトルを反対方向にする
    Vector2D oppo(Vector2D p);
      
  };

#endif