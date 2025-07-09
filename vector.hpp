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
    double length() const;
    // ベクトルの単位ベクトルを返す
    static Vector2D normalize(const Vector2D& p);
    // ベクトルの和
    static Vector2D add(const Vector2D& p, const Vector2D& q);
    // ベクトルの差
    static Vector2D subtract(const Vector2D& p, const Vector2D& q);
    // ベクトルをn倍にする
    static Vector2D multiple(const Vector2D& p, double n);
    // ベクトルを反対方向にする
    static Vector2D oppo(const Vector2D& p);
      
  };

#endif