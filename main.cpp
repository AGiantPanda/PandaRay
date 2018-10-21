#include <iostream>
#include <fstream>

#include "vec3.h"
#include "ray.h"

using namespace std;

vec3<float> color(const ray<float>& r) {
    float t = (r.direction().y() + 6.0) / 12.0;
    return float(1.0 - t)*vec3<float>(1.0, 1.0, 1.0) + t*vec3<float>(0.5, 0.7, 1.0);
}

int main(int argc, char const *argv[])
{
    std::ofstream output;
    output.open("output.ppm");

    int nx = 800;
    int ny = 600;
    output << "P3\n" << nx << " " << ny << "\n255\n";

    vec3<float> lower_left_corner(-8.0, -6.0, -1.0);
    vec3<float> horizontal(16.0, 0.0, 0.0);
    vec3<float> vertical(0.0, 12.0, 0.0);
    vec3<float> origin(0.0, 0.0, 0.0);

    for (int j = ny-1; j>=0; j--) {
        for (int i = 0; i < nx; i++) {
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);
            ray<float> r(origin, lower_left_corner+u*horizontal+v*vertical);
            // vec3<float> col(float(i) / float(nx), float(j) / float(ny), 0.2);
            vec3<float> col = color(r);
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
            output << ir << " " << ig << " " << ib << "\n";
        }
    }

    output.close();
    return 0;
}