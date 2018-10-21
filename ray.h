#pragma once

#include "vec3.h"

template<class T>
class ray {
private:
    vec3<T> A;
    vec3<T> B;

public:
    ray(const vec3<T>& a, const vec3<T>& b) { A = a; B = b; };
    vec3<T> origin() const { return A; }
    vec3<T> direction() const { return B; }
    vec3<T> point_at_parameter(T t) const { return A + t*B; }
};