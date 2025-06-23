#pragma once
#include "Vec3(sycl).h"
#include "ray(sycl).h"
#include "sphere(sycl).h"
#include <sycl/sycl.hpp>
#include <limits>  // for std::numeric_limits

constexpr float infinity = std::numeric_limits<float>::infinity();


[[sycl::device]]
bool hit_world_flat(const ray& r, const Sphere* spheres, int sphere_count, float t_min, float t_max, hit_record& rec) {
    hit_record temp_rec;
    bool hit_anything = false;
    float closest_so_far = t_max;

    for (int i = 0; i < sphere_count; ++i) {
        if (spheres[i].hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

[[sycl::device]]
Vec3 ray_color(const ray& r, const Sphere* spheres, int sphere_count) {
    hit_record rec;
    if (hit_world_flat(r, spheres, sphere_count, 0.001f, infinity, rec)) {
        return (rec.normal + Vec3(1,1,1))*0.5f;
    }
    Vec3 unit_direction = r.direction().unit();
    float t = 0.5f * (unit_direction.y() + 1.0f);
    return Vec3(1.0f, 1.0f, 1.0f)*(1.0f - t) + Vec3(0.5f, 0.7f, 1.0f)*t;
}
