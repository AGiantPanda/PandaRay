#pragma once

#include "ray.h"

struct hit_record {
    float t;
    Vec3f p;
    Vec3f normal;
};

class Shape {
public:
    virtual bool Intersect(const Ray &ray, float &t) const = 0;
    virtual bool IntersectRec(const Ray &ray, hit_record &hit) const = 0;
};