#pragma once

#include <math.h>
#include <stdlib.h>
#include <iostream>

template<class T>
class Vec3 {
private:
    T e[3];

public:
    Vec3() = default;
    Vec3(T e0, T e1, T e2) { e[0] = e0; e[1] = e1; e[2] = e2; }
    inline T x() const { return e[0]; }
    inline T y() const { return e[1]; }
    inline T z() const { return e[2]; }
    inline T r() const { return e[0]; }
    inline T g() const { return e[1]; }
    inline T b() const { return e[2]; }

    inline const Vec3& operator+() const { return *this; }
    inline Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
    inline T operator[](int i) const { return e[i]; }
    inline T& operator[](int i) { return e[i]; }

    inline Vec3 operator+(const Vec3 &v) const {
        return Vec3(e[0] + v[0], e[1] + v[1], e[2] + v[2]);
    }
    inline Vec3 operator-(const Vec3 &v) const {
        return Vec3(e[0] - v[0], e[1] - v[1], e[2] - v[2]);
    }

    inline Vec3 operator*(const Vec3 &v) const {
        return Vec3(e[0] * v[0], e[1] * v[1], e[2] * v[2]);
    }

    inline Vec3 operator/(const Vec3 &v) const {
        return Vec3(e[0] / v[0], e[1] / v[1], e[2] / v[2]);
    }

    template<class U>
    inline Vec3 operator*(U t) const {
        return Vec3(e[0]*(T)t, e[1]*(T)t, e[2]*(T)t);
    }

    template<class U>
    inline Vec3 operator/(U t) const {
        return Vec3(e[0]/(T)t, e[1]/(T)t, e[2]/(T)t);
    }

    inline Vec3& operator+=(const Vec3 &v) {
        e[0] += v[0];
        e[1] += v[1];
        e[2] += v[2];
        return *this;
    };
    inline Vec3& operator*=(const Vec3 &v) {
        e[0] *= v[0];
        e[1] *= v[1];
        e[2] *= v[2];
        return *this;
    };
    inline Vec3& operator/=(const Vec3 &v) {
        e[0] /= v[0];
        e[1] /= v[1];
        e[2] /= v[2];
        return *this;
    };
    inline Vec3& operator-=(const Vec3 &v) {
        e[0] -= v[0];
        e[1] -= v[1];
        e[2] -= v[2];
        return *this;
    };

    template<class U>
    inline Vec3& operator*=(const U t) {
        e[0] *= (T)t;
        e[1] *= (T)t;
        e[2] *= (T)t;
        return *this;
    };
    template<class U>
    inline Vec3& operator/=(const U t){
        T k = 1.0 / (T)k;

        e[0] *= (T)k;
        e[1] *= (T)k;
        e[2] *= (T)k;
        return *this;
    };

    inline T length() const {
        return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
    }
    inline T squared_length() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }
    inline void make_unit_vector() {
        T k = 1.0 / sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
        e[0] *= k; e[1] *= k; e[2] *= k;
    };
};

template<class T, class U>
inline Vec3<T> operator*(U t, const Vec3<T> &v){
    return Vec3<T>(v[0]*(T)t, v[1]*(T)t, v[2]*(T)t);
}

template<class T>
inline std::istream& operator>>(std::istream &is, Vec3<T> &t){
    is >> t[0] >> t[1] >> t[2];
    return is;
}

template<class T>
inline std::ostream& operator<<(std::ostream &os, const Vec3<T> &t) {
    os << t[0] << " " << t[1] << " " << t[2];
    return os;
}

// dot, cross and unit_vector make
template<class T>
inline T dot(const Vec3<T> &v1, const Vec3<T> &v2) {
    return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
}

template<class T>
inline Vec3<T> cross(const Vec3<T> &v1, const Vec3<T> &v2) {
    return Vec3<T>( (v1[1]*v2[2] - v1[2]*v2[1]),
                   -(v1[0]*v2[2] - v1[2]*v2[0]),
                    (v1[0]*v2[1] - v1[1]*v2[0]));
}

template<class T>
inline Vec3<T> unit_vector(Vec3<T> v) {
    return v / v.length();
}

typedef Vec3<int> Vec3i;
typedef Vec3<float> Vec3f;
typedef Vec3<double> Vec3d;