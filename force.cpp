#include "force.hpp"

// 合力（動径方向）
Vector2D Force::total_radial() {
  return Vector2D::add(Vector2D::add(contraction_radial, extension_radial),restoring_radial);
}

// 合力（外周方向）
Vector2D Force::total_peripheral() {
  return Vector2D::add(Vector2D::add(contraction_peripheral, extension_peripheral),restoring_peripheral);
}

// 全方向の合力
Vector2D Force::total() {
  return Vector2D::add(total_radial(), total_peripheral());
}

// 全部の力をリセット
void Force::clear() {
  contraction_radial.set(0,0);
  contraction_peripheral.set(0,0);
  extension_radial.set(0,0);
  extension_peripheral.set(0,0);
  restoring_radial.set(0,0);
  restoring_peripheral.set(0,0);
}
