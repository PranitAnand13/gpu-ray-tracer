#ifndef VEC3_SYCL_H
#define VEC3_SYCL_H

#include <CL/sycl.hpp>
#include <cmath>  // for std::fmin, std::fabs, std::sqrt 
using namespace sycl;

class Vec3 {
public:
    float p[3];

    Vec3(float x = 0, float y = 0, float z = 0) {
        p[0] = x;
        p[1] = y;
        p[2] = z;
    }

    float x() const { return p[0]; }
    float y() const { return p[1]; }
    float z() const { return p[2]; }

    float r() const { return p[0]; }
    float g() const { return p[1]; }
    float b() const { return p[2]; }

    Vec3 operator+(const Vec3& oth) const {
        return Vec3(p[0] + oth.p[0], p[1] + oth.p[1], p[2] + oth.p[2]);
    }

    Vec3 operator-(const Vec3& oth) const {
        return Vec3(p[0] - oth.p[0], p[1] - oth.p[1], p[2] - oth.p[2]);
    }

    Vec3 operator*(const float t) const {
        return Vec3(p[0] * t, p[1] * t, p[2] * t);
    }

    Vec3 operator/(const float t) const {
        float invT = 1.0f / t;
        return Vec3(p[0] * invT, p[1] * invT, p[2] * invT);
    }

    Vec3 operator-() const {
        return Vec3(-p[0], -p[1], -p[2]);
    }

    float operator[](int i) const { return p[i]; }

    float length() const {
        return sycl::sqrt(length_squared());
    }

    float length_squared() const {
        return p[0] * p[0] + p[1] * p[1] + p[2] * p[2];
    }

    Vec3 unit() const {
        return (*this) / length();
    }

    [[sycl::device]]
    bool near_zero() const {
        const auto s = 1e-8f;
        return (sycl::fabs(p[0]) < s) && (sycl::fabs(p[1]) < s) && (sycl::fabs(p[2]) < s);
    }
};

inline float dot(const Vec3& u, const Vec3& v) {
    return u.p[0] * v.p[0] + u.p[1] * v.p[1] + u.p[2] * v.p[2];
}

inline Vec3 cross(const Vec3& u, const Vec3& v) {
    return Vec3(
        u.p[1] * v.p[2] - u.p[2] * v.p[1],
        u.p[2] * v.p[0] - u.p[0] * v.p[2],
        u.p[0] * v.p[1] - u.p[1] * v.p[0]
    );
}

inline Vec3 operator*(const Vec3& u, const Vec3& v) {
    return Vec3(u.p[0] * v.p[0], u.p[1] * v.p[1], u.p[2] * v.p[2]);
}

inline Vec3 unit_vector(const Vec3& v) {
    return v / v.length();
}

#ifndef __SYCL_DEVICE_ONLY__
#include <iostream>
inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
    return out << v.p[0] << ' ' << v.p[1] << ' ' << v.p[2];
}
#endif

inline Vec3 reflect(const Vec3& v, const Vec3& n) {
    return v - n * 2 * dot(v, n);
}



// Schlick approximation for reflectance
inline float schlick_reflectance(float cosine, float ref_idx) {
    float r0 = (1.0f - ref_idx) / (1.0f + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * sycl::pow((1 - cosine), 5.0f);
}

using point3 = Vec3;

#endif // VEC3_SYCL_H
