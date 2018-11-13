#pragma once

#include "../cores/pandaray.h"
#include "../cores/vec3.h"

class Ray {
public:
    Ray() : tMin(-Epsilon), tMax(Infinity), time(0.0f) {}
    Ray(const Vec3f a, const Vec3f b, float tmin = -Epsilon, 
        float tmax = Infinity, float time = 0.0f)
        : o(a), tMin(tmin), tMax(tmax), time(time) {
            d = unit_vector(b);
        }

    Vec3f Origin() const { return o; }
    Vec3f Direction() const { return d; }
    Vec3f Point_At_Parameter(float t) const { return o + t*d; }
    Vec3f operator()(float t) const { return o + t*d; }
    
    Vec3f o;
    Vec3f d;
    float tMin;
    float tMax;
    float time;
};