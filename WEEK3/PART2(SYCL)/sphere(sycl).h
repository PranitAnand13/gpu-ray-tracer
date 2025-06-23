#ifndef SPHERE_SYCL_H
#define SPHERE_SYCL_H

#include "ray(sycl).h"

struct hit_record {
    point3 p;
    Vec3 normal;
    float t;
    bool front_face;

    inline void set_face_normal(const ray& r, const Vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Sphere {
private:
    point3 center;
    float radius;

public:
    Sphere() {}
    Sphere(point3 cen, float r) : center(cen), radius(r) {}

    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
        Vec3 oc = r.origin() - center;
        float a = dot(r.direction(), r.direction());
        float half_b = dot(oc, r.direction());
        float c = dot(oc, oc) - radius * radius;
        float discriminant = half_b * half_b - a * c;

        if (discriminant < 0) return false;
        float sqrt_d = sycl::sqrt(discriminant);

        // Find the nearest root in the acceptable range
        float root = (-half_b - sqrt_d) / a;
        if (root < t_min || root > t_max) {
            root = (-half_b + sqrt_d) / a;
            if (root < t_min || root > t_max)
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        Vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        return true;
    }
};

#endif
