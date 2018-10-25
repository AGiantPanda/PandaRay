#pragma once

#include "../cores/vec3.h"

// template<class T>
class Ray {
private:
    Vec3f A;
    Vec3f B;

public:
    Ray(const Vec3f& a, const Vec3f& b) { A = a; B = unit_vector(b - a); };
    Vec3f Origin() const { return A; }
    Vec3f Direction() const { return B; }
    Vec3f Point_at_parameter(float t) const { return A + t*B; }
};