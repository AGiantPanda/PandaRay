#pragma once

#include "../cores/vec3.h"
#include "../cores/ray.h"

class Sphere
{
private:
    float radius;
    Vec3f center;

public:
    Sphere() = default;
    Sphere(Vec3f cc, float rr);
    Sphere(float p0, float p1, float p2, float rr);
    bool Intersect(const Ray &ray, float &tHit) const;
};