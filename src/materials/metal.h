#pragma once

#include "../cores/material.h"

class Metal : public Material {
public:
    Metal(const Vec3f & a, float f) : albedo(a) { if(f < 1) fuzz = f; else fuzz = 1;}
    virtual bool scatter(const Ray &r, const hit_record &rec, Vec3f &attenuation, Ray &scattered) const {
        Vec3f reflected = reflect(r.Direction(), rec.normal);
        scattered = Ray(rec.p, reflected + fuzz*random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.Direction(), rec.normal) > 0);
    }

    Vec3f albedo;
    float fuzz;
};