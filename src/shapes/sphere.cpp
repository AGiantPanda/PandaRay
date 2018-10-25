#include "sphere.h"

Sphere::Sphere(Vec3f cc, float rr) {
    center = cc;
    radius = rr;
}

Sphere::Sphere(float p0, float p1, float p2, float rr) {
    center[0] = p0;
    center[1] = p1;
    center[2] = p2;
    radius = rr;
}

bool Sphere::Intersect(const Ray &ray, float &tHit) const {
    Vec3f co = ray.Origin() - center;
    const float episilon = 1e-5;
    float a = dot(ray.Direction(), ray.Direction());
    float b = 2.0 * dot(ray.Direction(), co);
    float c = dot(co, co) - radius * radius;
    float discriminant = b*b - 4*a*c;

    if (discriminant < 0.0 -  episilon) {
        return false;
    } else {
        float min = (-b - sqrt(discriminant)) / (2.0 * a);
        float max = (-b + sqrt(discriminant)) / (2.0 * a);

        if (min >= 0.0 - episilon) {
            tHit = min;
            return true;
        } else if (max >= 0.0 - episilon) {
            tHit = max;
            return true;
        } else {
            return false;
        }
    }
}
