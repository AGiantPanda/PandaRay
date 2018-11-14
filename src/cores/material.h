#pragma once

#include "../cores/pandaray.h"
#include "../cores/shape.h"

class Material {
public:
    virtual bool scatter(const Ray &r, const hit_record &rec, Vec3f &attenuation, Ray &scattered) const = 0;
};