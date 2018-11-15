#pragma once

#include "../cores/pandaray.h"

class Camera {
public:
    Camera() {
        lower_left_corner = Vec3f(-2.0, -1.0, -1.0);
        horizontal = Vec3f(4.0, 0.0, 0.0);
        vertical = Vec3f(0.0, 2.0, 0.0);
        origin = Vec3f(0.0, 0.0, 0.0);
    }
    Ray GetRay(const float u, const float v) {
        Vec3f dir = unit_vector(lower_left_corner+u*horizontal+v*vertical);
        return Ray(origin, dir);
    }

    Vec3f origin;
    Vec3f lower_left_corner;
    Vec3f horizontal;
    Vec3f vertical;
};