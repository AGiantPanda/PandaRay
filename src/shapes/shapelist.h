#pragma once

#include "../cores/pandaray.h"
#include "../cores/vec3.h"
#include "../cores/ray.h"
#include "../cores/shape.h"

class Shape_List : public Shape
{
private:

public:
    Shape_List() {}
    Shape_List(Shape **l, int n) : list(l), size(n) {}
    
    virtual bool Intersect(const Ray &ray, float &tHit) const {
        return false;
    };
    virtual bool IntersectRec(const Ray &ray, hit_record &hit) const {
        hit_record tmp;
        bool ishitted = false;
        float closest = ray.tMax;
        for(int i = 0; i < size; i++) {
            if (list[i]->IntersectRec(ray, tmp)) {
                ishitted = true;
                if (tmp.t < closest) {
                    closest = tmp.t;
                    hit = tmp;
                }
            }
        }
        return ishitted;
    };

    Shape **list;
    int size;
};