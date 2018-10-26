#pragma once

#include "../cores/pandaray.h"
#include "../cores/vec3.h"
#include "../cores/ray.h"
#include "../cores/shape.h"

class Sphere : public Shape
{
private:

public:
    Sphere() = default;
    Sphere(Vec3f cc, float rr);
    Sphere(float p0, float p1, float p2, float rr);
    
    inline Vec3f Center() const { return center; }
    inline float Radius() const { return radius; }
    virtual bool Intersect(const Ray &ray, float &tHit) const;
    virtual bool IntersectRec(const Ray &ray, hit_record &hit) const;
    
    float radius;
    Vec3f center;
};