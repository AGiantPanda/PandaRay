#pragma once

#include "../cores/material.h"

class Dielectric : public Material {
public:
    Dielectric(float ri) : ref_idx(ri) {}
    
	virtual bool scatter(const Ray &r, const hit_record &rec, Vec3f &attenuation, Ray &scattered) const 
	{
		Vec3f outward_normal;
		Vec3f reflected = reflect(r.Direction(), rec.normal);
		float ni_over_nt;
		attenuation = Vec3f(1.0, 1.0, 1.0);
		Vec3f refracted;

		float reflect_prob;
		float cosine;

		if (dot(r.Direction(), rec.normal) > 0) 
		{
			outward_normal = -rec.normal;
			ni_over_nt = ref_idx;
			cosine = ref_idx * dot(r.Direction(), rec.normal) / r.Direction().length();
		}
		else
		{
			outward_normal = rec.normal;
			ni_over_nt = 1.0 / ref_idx;
			cosine = -dot(r.Direction(), rec.normal) / r.Direction().length();
		}

		if (refract(r.Direction(), outward_normal, ni_over_nt, refracted))
		{
			reflect_prob = schlick(cosine, ref_idx);
		}
		else
		{
			reflect_prob = 1.0;
		}

		if (random_double() < reflect_prob)
		{
			scattered = Ray(rec.p, reflected);
		}
		else
		{
			scattered = Ray(rec.p, refracted);
		}

		return true;
	}

    float ref_idx;
};