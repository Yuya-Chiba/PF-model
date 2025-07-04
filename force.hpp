#ifndef FORCE_HPP
#define FORCE_HPP

#include "vector.hpp"

class Force {
public:
    Vector2D contraction_radial;
    Vector2D contraction_peripheral;

    Vector2D extension_radial;
    Vector2D extension_peripheral;

    Vector2D restoring_radial;
    Vector2D restoring_peripheral;

    // 力の合計を計算
    Vector2D total_radial();
    Vector2D total_peripheral();
    Vector2D total();

    void clear(); // 力の全リセット、毎step行う
};

#endif
