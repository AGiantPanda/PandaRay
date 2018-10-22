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

bool Sphere::Intersect(const Ray& ray, float *tHit) const {
    Vec3f oc = ray.Origin() - center;
    float a = dot(ray.Direction(), ray.Direction());
    float b = 2.0 * dot(oc, ray.Direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b*b - 4*a*c;

    if (discriminant < 0) {
        return false;
    } else {
        *tHit = (-b - sqrt(discriminant)) / (2.0 * a);
        return true;
    }
}