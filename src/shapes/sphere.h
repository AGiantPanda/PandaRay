#pragma once

#include "../cores/shape.h"

class Sphere : public Shape
{
private:

public:
    Sphere(Vec3f cc, float rr, const std::shared_ptr<Material> &mm);
    Sphere(float p0, float p1, float p2, float rr, const std::shared_ptr<Material> &mm);
    
    inline Vec3f Center() const { return center; }
    inline float Radius() const { return radius; }
    virtual bool Intersect(const Ray &ray, float &tHit) const;
    virtual bool IntersectRec(const Ray &ray, hit_record &hit) const;
    
    float radius;
    Vec3f center;
};