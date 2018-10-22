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
    inline Vec3 operator-() const { return Vec3<T>(-e[0], -e[1], -e[2]); }
    inline T operator[](int i) const { return e[i]; }
    inline T& operator[](int i) { return e[i]; }

    inline Vec3& operator+=(const Vec3 &v2);
    inline Vec3& operator-=(const Vec3 &v2);
    inline Vec3& operator*=(const Vec3 &v2);
    inline Vec3& operator/=(const Vec3 &v2);
    inline Vec3& operator*=(const T t);
    inline Vec3& operator/=(const T t);

    inline T length() const {
        return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
    }
    inline T squared_length() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }
    inline void make_unit_vector();
};

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

template<class T>
inline void Vec3<T>::make_unit_vector() {
    T k = 1.0 / sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
    e[0] *= k; e[1] *= k; e[2] *= k;
}

template<class T>
inline Vec3<T> operator+(const Vec3<T> &v1, const Vec3<T> &v2) {
    return Vec3<T>(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]);
}

template<class T>
inline Vec3<T> operator-(const Vec3<T> &v1, const Vec3<T> &v2) {
    return Vec3<T>(v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]);
}

template<class T>
inline Vec3<T> operator*(const Vec3<T> &v1, const Vec3<T> &v2) {
    return Vec3<T>(v1[0] * v2[0], v1[1] * v2[1], v1[2] * v2[2]);
}

template<class T>
inline Vec3<T> operator/(const Vec3<T> &v1, const Vec3<T> &v2) {
    return Vec3<T>(v1[0] / v2[0], v1[1] / v2[1], v1[2] / v2[2]);
}

template<class T>
inline Vec3<T> operator*(const Vec3<T> &v, T t) {
    return Vec3<T>(v[0]*t, v[1]*t, v[2]*t);
}

template<class T>
inline Vec3<T> operator*(T t, const Vec3<T> &v) {
    return Vec3<T>(v[0]*t, v[1]*t, v[2]*t);
}

template<class T>
inline Vec3<T> operator/(const Vec3<T> &v, T t) {
    return Vec3<T>(v[0]/t, v[1]/t, v[2]/t);
}

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
inline Vec3<T>& Vec3<T>::operator+=(const Vec3<T> &v) {
    e[0] += v[0];
    e[1] += v[1];
    e[2] += v[2];
    return *this;
}

template<class T>
inline Vec3<T>& Vec3<T>::operator*=(const Vec3<T> &v) {
    e[0] *= v[0];
    e[1] *= v[1];
    e[2] *= v[2];
    return *this;
}

template<class T>
inline Vec3<T>& Vec3<T>::operator/=(const Vec3<T> &v) {
    e[0] /= v[0];
    e[1] /= v[1];
    e[2] /= v[2];
    return *this;
}

template<class T>
inline Vec3<T>& Vec3<T>::operator-=(const Vec3<T> &v) {
    e[0] -= v[0];
    e[1] -= v[1];
    e[2] -= v[2];
    return *this;
}

template<class T>
inline Vec3<T>& Vec3<T>::operator*=(const T t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
}

template<class T>
inline Vec3<T>& Vec3<T>::operator/=(const T t) {
    T k = 1.0 / k;
    
    e[0] *= k;
    e[1] *= k;
    e[2] *= k;
    return *this;
}

template<class T>
inline Vec3<T> unit_vector(Vec3<T> v) {
    return v / v.length();
}

typedef Vec3<int> Vec3i;
typedef Vec3<float> Vec3f;
typedef Vec3<double> Vec3d;