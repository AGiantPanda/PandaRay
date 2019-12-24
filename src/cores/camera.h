#pragma once

#include "../cores/pandaray.h"

class Camera {
public:
    Camera(Vec3f lookFrom, Vec3f lookAt, Vec3f Up, float vfov, float aspect, float aperture = 0, float focus_dist = 1.0)	// vfov is top to bottom in degrees 
	{
		lens_radius = aperture / 2;
		float theta = vfov * Pi / 180.0;
		float half_height = tan(theta / 2);
		float half_width = aspect * half_height;

        origin = lookFrom;
		w = unit_vector(lookFrom - lookAt);
		u = unit_vector(cross(Up, w));
		v = cross(w, u);

		lower_left_corner = origin
			- half_width * u * focus_dist
			- half_height * v * focus_dist
			- w * focus_dist;
		upper_left_corner = origin
			- half_width * u * focus_dist
			+ half_height * v * focus_dist
			- w * focus_dist;
		horizontal = 2 * half_width * u * focus_dist;
		vertical = 2 * half_height * v * focus_dist;
    }

    Ray GetRay(const float w, const float h) 
	{
		Vec3f rd = lens_radius * random_in_unit_disk();
		Vec3f offset = u * rd.x() + v * rd.y();
        Vec3f dir = unit_vector(upper_left_corner + w * horizontal - h * vertical - origin - offset);
        return Ray(origin + offset, dir);
    }

    Vec3f origin;
	Vec3f lower_left_corner;
	Vec3f upper_left_corner;
    Vec3f horizontal;
    Vec3f vertical;
	Vec3f u, v, w;
	float lens_radius;
};