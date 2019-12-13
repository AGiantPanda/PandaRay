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

inline bool refract(const Vec3f &v, const Vec3f &n, float ni_over_nt, Vec3f &refracted)
{
	Vec3f uv = unit_vector(v);
	float dt = dot(uv, n);
	float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
	if (discriminant > 0) 
	{
		refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
		return true;
	}
	else
		return false;
}

inline float schlick(float cosine, float ref_idx) 
{
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}