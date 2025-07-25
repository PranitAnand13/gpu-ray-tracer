#pragma once

#include "Vec3(sycl).h"

class ray {
public:
    // Default constructor
    ray() = default;

    // Parameterized constructor: takes origin and direction vectors
    ray(const Vec3& origin, const Vec3& direction)
        : orig(origin), dir(direction) {}

    // Getters for origin and direction
    Vec3 origin() const { return orig; }
    Vec3 direction() const { return dir; }

    // Computes point along the ray at distance 't'
    Vec3 at(float t) const {
        return orig + dir * t;
    }

private:
    Vec3 orig; // Ray origin
    Vec3 dir;  // Ray direction
};
