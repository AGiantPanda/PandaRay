#include <iostream>
#include <fstream>

#include "../cores/vec3.h"
#include "../cores/ray.h"

#include "../shapes/sphere.h"

using namespace std;

Vec3<float> bgcolor(const Ray& r) {
    float t = (r.Direction().y() + 1.0) / 2.0;
    return float(1.0 - t)*Vec3<float>(1.0, 1.0, 1.0) + t*Vec3<float>(0.5, 0.7, 1.0);
}

int main(int argc, char const *argv[])
{
    std::ofstream output;
    output.open("output.ppm");

    int nx = 800;
    int ny = 600;
    output << "P3\n" << nx << " " << ny << "\n255\n";

    Vec3<float> lower_left_corner(-8.0, -6.0, -1.0);
    Vec3<float> horizontal(16.0, 0.0, 0.0);
    Vec3<float> vertical(0.0, 12.0, 0.0);
    Vec3<float> origin(0.0, 0.0, 0.0);

    Sphere s(0.0, 0.0, -10.0, 8.0);
    float t = 0.0;

    for (int j = ny-1; j>=0; j--) {
        for (int i = 0; i < nx; i++) {
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);
            Ray r(origin, lower_left_corner+u*horizontal+v*vertical);
            // Vec3<float> col(float(i) / float(nx), float(j) / float(ny), 0.2);
            Vec3<float> col = bgcolor(r);
            if (s.Intersect(r, t)) {
                if (t > 9) cout << t << endl;
                float c = (t - 2.0) / 4.0;
                col = Vec3f(c, 0.0, 0.0);
            }
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
            output << ir << " " << ig << " " << ib << "\n";
        }
    }

    output.close();
    return 0;
}