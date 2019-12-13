#pragma once

#include "../cores/pandaray.h"

Vec3f random_in_unit_disk()
{
	Vec3f p;
	
	do
	{
		p = 2.0 * Vec3f(random_double(), random_double(), 0) - Vec3f(1, 1, 0);
	} while (dot(p, p) >= 1.0);

	return p;
}

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
		horizontal = 2 * half_width * u * focus_dist;
		vertical = 2 * half_height * v * focus_dist;
    }

    Ray GetRay(const float s, const float t) 
	{
		Vec3f rd = lens_radius * random_in_unit_disk();
		Vec3f offset = u * rd.x() + v * rd.y();
        Vec3f dir = unit_vector(lower_left_corner + s * horizontal + t * vertical - origin - offset);
        return Ray(origin + offset, dir);
    }

    Vec3f origin;
    Vec3f lower_left_corner;
    Vec3f horizontal;
    Vec3f vertical;
	Vec3f u, v, w;
	float lens_radius;
};