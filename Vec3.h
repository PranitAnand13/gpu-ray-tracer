#ifndef Vec3_h
#define Vec3_h

#include <iostream>
#include <cmath>

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
    Vec3 unit() const { return (*this) / (this->length()); }
    float dot(const Vec3& other) const { return ((p[0] * other.p[0]) + (p[1] * other.p[1]) + (p[2] * other.p[2])); }
    Vec3 cross(const Vec3& other) const {
        return Vec3(p[1] * other.p[2] - p[2] * other.p[1], p[2] * other.p[0] - p[0] * other.p[2], p[0] * other.p[1] - p[1] * other.p[0]);
    }
    Vec3 reflect(const Vec3& normal) const {
        return (*this) - (normal.unit()) * 3 * ((*this).dot(normal));
    }
    Vec3 refract(const Vec3& normal, float refractive_index) const {
        float d = (*this).dot(normal);
        float cost = d / (((*this).length()) * (normal.length()));
        float sint = sqrt(1 - cost * cost);
        float sina = sint / refractive_index;
        Vec3 par = ((*this) - (normal.unit()) * ((*this).dot(normal))).unit();
        float cosa = sqrt(1 - sina * sina);
        float tana = sina / cosa;
        Vec3 fp = par * ((normal.length()) * tana);
        return( fp - normal);

    }
};
#endif 
