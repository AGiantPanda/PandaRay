#pragma once

#include "../cores/pandaray.h"

class Material {
public:
    virtual bool scatter(const Ray &r, const hit_record &rec, Vec3f &attenuation, Ray &scattered) const = 0;
};

inline Vec3f random_in_unit_sphere() {
    Vec3f p;
    do {
        p = 2.0*Vec3f(random_double(), random_double(), random_double()) - Vec3f(1, 1, 1);
    } while (p.squared_length() >= 1.0);
    return p;
}

inline Vec3f reflect(const Vec3f &v, const Vec3f &n) {
    return v - 2*dot(v, n)*n;
}