#pragma once

#include "../cores/pandaray.h"

struct hit_record {
    float t;
    Vec3f p;
    Vec3f normal;
    std::shared_ptr<Material> mat_ptr;
};

class Shape {
public:
    virtual bool Intersect(const Ray &ray, float &t) const = 0;
    // return hit point's normal, point, etc.
    virtual bool IntersectRec(const Ray &ray, hit_record &hit) const = 0;

    std::shared_ptr<Material> mat = nullptr;
};