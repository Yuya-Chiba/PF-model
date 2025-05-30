#include "vector.hpp"

// ベクトルのxy座標をset
void set(double a, double b) {
  x = a;
  y = b;
}

// ベクトルの長さを求める
double length(Vector2D p) {
  return std::sqrt((p.x)*(p.x) + (p.y)*(p.y));
  // return std::hypot(p.x, p.y);
}

// ベクトルの単位ベクトルを返す
Vector2D normalize(Vector2D p) {
  Vector2D q;
  q.x = p.x / p.length;
  q.y = p.y / p.length;
  return q;
}

// ベクトル和
Vector2D add(Vector2D p, Vector2D q) {
  Vector2D r;
  r.x = p.x + q.x;
  r.y = p.y + q.y;
  return r;
}

// 2ベクトル間の距離
double distance(Vector2D p, Vector2D q) {
  return std::hypot(p.x - q.x, p.y - q.y);
}