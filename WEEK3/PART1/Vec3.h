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
    Vec3 unit() const { return (*this) / (this->length()); }
    inline Vec3 unit_vector(const Vec3& v) {
    return v / v.length();
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
inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
    return out << v.p[0] << ' ' << v.p[1] << ' ' << v.p[2];
}
inline Vec3 unit_vector(const Vec3& v) {
    return v / v.length();
}
using point3 = Vec3;
#endif 