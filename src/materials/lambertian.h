#pragma once

#include "../cores/material.h"

class Lambertian : public Material {
public:
    Lambertian(const Vec3f &a) : albedo(a) {}
    virtual bool scatter(const Ray &r, const hit_record &rec, Vec3f &attenuation, Ray &scattered) const {
        Vec3f tar = rec.p + rec.normal + random_in_unit_sphere();
        scattered = Ray(rec.p, tar - rec.p);
        attenuation = albedo;
        return true;
    }

    Vec3f albedo;
};