#include "vector.hpp"

// ベクトルのxy座標をset
void Vector2D::set(double a, double b) {
  x = a;
  y = b;
}

// ベクトルの長さを求める
double Vector2D::length() {
  return std::hypot(x, y);
}

// ベクトルの単位ベクトルを返す
Vector2D Vector2D::normalize(Vector2D p) {
  Vector2D q;
  q.set(p.x / p.length(), p.y / p.length());
  return q;
}

// ベクトル和
Vector2D Vector2D::add(Vector2D p, Vector2D q) {
  Vector2D r;
  r.set(p.x + q.x, p.y + q.y);
  return r;
}

// ベクトル差
Vector2D Vector2D::substract(Vector2D p, Vector2D q) {
  Vector2D r;
  r.set(p.x - q.x, p.y - q.y);
  return r;
}

// ベクトルのn倍
Vector2D Vector2D::multiple(Vector2D p, double n) {
  Vector2D q;
  q.set(p.x * n, p.y * n);
  return q;
}

// ベクトルを反対方向にする
Vector2D Vector2D::oppo(Vector2D p) {
  Vector2D q;
  q.set(-(p.x), -(p.y));
  return q;
}