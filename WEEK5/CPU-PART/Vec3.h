#ifndef Vec3_h
#define Vec3_h


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

    Vec3 operator+(const Vec3& oth) const { return Vec3(p[0] + oth.p[0], p[1] + oth.p[1], p[2] + oth.p[2]); }
    Vec3 operator-(const Vec3& oth) const { return Vec3(p[0] - oth.p[0], p[1] - oth.p[1], p[2] - oth.p[2]); }
    Vec3 operator*(const float t) const { return Vec3(p[0] * t, p[1] * t, p[2] * t); }
    Vec3 operator/(const float t) const { return Vec3(p[0] * (1 / t), p[1] * (1 / t), p[2] * (1 / t)); }
    Vec3 operator-() const { return Vec3(-p[0], -p[1], -p[2]); }
    float operator[](const int i) const { return p[i]; }
    float length() const { return sqrt((p[0] * p[0]) + (p[1] * p[1]) + (p[2] * p[2])); }
    float length_squared() const {
        return p[0]*p[0] + p[1]*p[1] + p[2]*p[2]; }
    
    bool near_zero() const {
        // Return true if the vector is close to zero in all dimensions.
        auto s = 1e-8;
        return (std::fabs(p[0]) < s) && (std::fabs(p[1]) < s) && (std::fabs(p[2]) < s);
    }

    Vec3 unit() const { return (*this) / (this->length()); }
    inline Vec3 unit_vector(const Vec3& v) {
    return v / v.length();
    
}
 static Vec3 random() {
        return Vec3(random_double(), random_double(), random_double());
    }

    static Vec3 random(double min, double max) {
        return Vec3(random_double(min,max), random_double(min,max), random_double(min,max));
    }   
};
inline float dot(const Vec3& u, const Vec3& v) {
    return u.p[0] * v.p[0]
         + u.p[1] * v.p[1]
         + u.p[2] * v.p[2];}
inline Vec3 cross(const Vec3& u, const Vec3& v) {
    return Vec3(u.p[1] * v.p[2] - u.p[2] * v.p[1],
                u.p[2] * v.p[0] - u.p[0] * v.p[2],
                u.p[0] * v.p[1] - u.p[1] * v.p[0]);
}
inline Vec3 operator*(const Vec3& u, const Vec3& v) {
    return Vec3(u.p[0] * v.p[0], u.p[1] * v.p[1], u.p[2] * v.p[2]);
}
inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
    return out << v.p[0] << ' ' << v.p[1] << ' ' << v.p[2];
}
inline Vec3 unit_vector(const Vec3& v) {
    return v / v.length();
}
inline Vec3 random_unit_vector() {
    while (true) {
        auto p = Vec3::random(-1,1);
        auto lensq = p.length_squared();
        if (1e-160 < lensq && lensq <= 1)
            return p / sqrt(lensq);
    }
}
inline Vec3 random_on_hemisphere(const Vec3& normal) {
    Vec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}
inline Vec3 reflect(const Vec3& v, const Vec3& n) {
    return v - n*2*dot(v,n);
}
inline Vec3 refract(const Vec3& uv, const Vec3& n, double etai_over_etat) {
    auto cos_theta = std::fmin(dot(-uv, n), 1.0);
    Vec3 r_out_perp =   (uv + n*cos_theta)*etai_over_etat;
    Vec3 r_out_parallel = -n*std::sqrt(std::fabs(1.0 - r_out_perp.length_squared()));
    return r_out_perp + r_out_parallel;
}

using point3 = Vec3;
#endif 