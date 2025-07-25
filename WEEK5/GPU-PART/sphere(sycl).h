#ifndef SPHERE_SYCL_H
#define SPHERE_SYCL_H

#include "ray(sycl).h"

struct hit_record {
    Vec3 p;
    Vec3 normal;
    float t;
    bool front_face;

    // Material properties
    int mat_type;  // 0 = Lambertian, 1 = Metal, 2 = Dielectric
    Vec3 albedo;
    float fuzz;      // Only for Metal
    float ir;        // Index of refraction (used by Dielectric)

    [[sycl::device]]
    inline void set_face_normal(const ray& r, const Vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

enum MaterialType {
    LAMBERTIAN = 0,
    METAL = 1,
    DIELECTRIC = 2
};

struct Sphere {
    point3 center;
    float radius;
    int mat_type;

    Vec3 albedo;
    float fuzz;
    float ir;  // Index of refraction

    Sphere() {}

    //  constructor
    Sphere(point3 c, float r, int mtype, Vec3 color, float f = 0.0f,float ref=1.0f)
        : center(c), radius(r), mat_type(mtype), albedo(color), fuzz(f < 1 ? f : 1), ir(ref) {}

    
   

    [[sycl::device]]
    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
        Vec3 oc = r.origin() - center;
        float a = dot(r.direction(), r.direction());
        float half_b = dot(oc, r.direction());
        float c = dot(oc, oc) - radius * radius;
        float discriminant = half_b * half_b - a * c;

        if (discriminant < 0) return false;
        float sqrtd = sycl::sqrt(discriminant);

        float root = (-half_b - sqrtd) / a;
        if (root < t_min || root > t_max) {
            root = (-half_b + sqrtd) / a;
            if (root < t_min || root > t_max)
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        Vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);

        rec.mat_type = mat_type;
        rec.albedo = albedo;
        rec.fuzz = fuzz;
        rec.ir = ir;

        return true;
    }
};

#endif // SPHERE_SYCL_H
