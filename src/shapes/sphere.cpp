#include "sphere.h"

Sphere::Sphere(Vec3f cc, float rr, const std::shared_ptr<Material> &mm) {
    center = cc;
    radius = rr;
    mat = mm;
}

Sphere::Sphere(float p0, float p1, float p2, float rr, const std::shared_ptr<Material> &mm) {
    center[0] = p0;
    center[1] = p1;
    center[2] = p2;
    radius = rr;
    mat = mm;
}

bool Sphere::Intersect(const Ray &ray, float &tHit) const {
    Vec3f co = ray.Origin() - center;
    float a = dot(ray.Direction(), ray.Direction());
    float b = 2.0 * dot(ray.Direction(), co);
    float c = dot(co, co) - radius * radius;
    float discriminant = b*b - 4*a*c;

    if (discriminant < ray.tMin) {
        return false;
    } else {
        float min = (-b - sqrt(discriminant)) / (2.0 * a);
        float max = (-b + sqrt(discriminant)) / (2.0 * a);

        if (min >= ray.tMin && min < ray.tMax) {
            tHit = min;
            return true;
        } else if (max >= ray.tMin && max < ray.tMax) {
            tHit = max;
            return true;
        } else {
            return false;
        }
    }
}

bool Sphere::IntersectRec(const Ray &ray, hit_record &hit) const {
    bool res = Intersect(ray, hit.t);
    if (res == false) {
        return false;
    } else {
        hit.p = ray(hit.t);
        hit.normal = unit_vector(hit.p - center);
        hit.mat_ptr = mat;
        return true;
    }
}