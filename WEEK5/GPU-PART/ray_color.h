#pragma once

#include "Vec3(sycl).h"
#include "ray(sycl).h"
#include "sphere(sycl).h"
#include <sycl/sycl.hpp>
#include <limits>

constexpr float infinity = std::numeric_limits<float>::infinity();

// ===== RANDOM UTILS =====
[[sycl::device]]
float random_float(uint32_t& seed) {
    seed = seed * 1664525 + 1013904223;
    return (seed & 0x00FFFFFF) / float(0x01000000);
}

[[sycl::device]]
Vec3 random_in_unit_sphere(uint32_t& seed) {
    while (true) {
        Vec3 p(
            random_float(seed) * 2 - 1,
            random_float(seed) * 2 - 1,
            random_float(seed) * 2 - 1
        );
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

[[sycl::device]]
Vec3 random_unit_vector(uint32_t& seed) {
    return random_in_unit_sphere(seed).unit();
}

// ===== REFRACTION UTILS =====
[[sycl::device]]
Vec3 refract(const Vec3& uv, const Vec3& n, float eta_ratio) {
    float cos_theta = sycl::fmin(dot(-uv, n), 1.0f);
    Vec3 r_out_perp = (uv + n*cos_theta)*eta_ratio;
    Vec3 r_out_parallel = -n*sycl::sqrt(sycl::fabs(1.0f - r_out_perp.length_squared()));
    return r_out_perp + r_out_parallel;
}

[[sycl::device]]
float reflectance(float cosine, float ref_idx) {
    float r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * sycl::pow((1 - cosine), 5);
}

// ===== WORLD HIT FUNCTION =====
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

// ===== COLOR FUNCTION =====
[[sycl::device]]
Vec3 ray_color(const ray& r_in, const Sphere* spheres, int sphere_count, int max_depth, uint32_t& seed) {
    ray cur_ray = r_in;
    Vec3 cur_attenuation(1, 1, 1);

    for (int depth = 0; depth < max_depth; ++depth) {
        hit_record rec;
        if (hit_world_flat(cur_ray, spheres, sphere_count, 0.001f, infinity, rec)) {
            if (rec.mat_type == LAMBERTIAN) {
                Vec3 scatter_dir = rec.normal + random_unit_vector(seed);
                if (scatter_dir.near_zero())
                    scatter_dir = rec.normal;
                cur_ray = ray(rec.p, scatter_dir);
                cur_attenuation = cur_attenuation * rec.albedo;

            } else if (rec.mat_type == METAL) {
                Vec3 reflected = reflect(cur_ray.direction().unit(), rec.normal);
                cur_ray = ray(rec.p, reflected + random_in_unit_sphere(seed)*rec.fuzz);

                cur_attenuation = cur_attenuation * rec.albedo;
                if (dot(cur_ray.direction(), rec.normal) <= 0)
                    return Vec3(0, 0, 0);

            } else if (rec.mat_type == DIELECTRIC) {
                float refraction_ratio = rec.front_face ? (1.0f / rec.ir) : rec.ir;
                Vec3 unit_dir = (cur_ray.direction()).unit();
                float cos_theta = sycl::fmin(dot(-unit_dir, rec.normal), 1.0f);
                float sin_theta = sycl::sqrt(1.0f - cos_theta * cos_theta);

                bool cannot_refract = refraction_ratio * sin_theta > 1.0f;
                Vec3 direction;

                if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_float(seed)) {
                    direction = reflect(unit_dir, rec.normal);
                } else {
                    direction = refract(unit_dir, rec.normal, refraction_ratio);
                }

                cur_ray = ray(rec.p, direction);
                cur_attenuation = cur_attenuation * Vec3(1.0f, 1.0f, 1.0f);
            } else {
                return Vec3(0, 0, 0);
            }
        } else {
            Vec3 unit_dir = cur_ray.direction().unit();
            float t = 0.5f * (unit_dir.y() + 1.0f);
            Vec3 bg_color = Vec3(1.0f, 1.0f, 1.0f) * (1.0f - t) + Vec3(0.5f, 0.7f, 1.0f) * t;
            return cur_attenuation * bg_color;
        }
    }

    return Vec3(0, 0, 0);
}
