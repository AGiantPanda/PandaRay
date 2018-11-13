#include "./cores/pandaray.h"

#include "../cores/vec3.h"
#include "../cores/ray.h"
#include "../cores/camera.h"
#include "../shapes/sphere.h"
#include "../shapes/shapelist.h"

using namespace std;

random_device rd;
mt19937 gen(rd());
uniform_real_distribution<float> dis(-0.5, 0.5);

Vec3f random_in_unit_sphere() {
    Vec3f p;
    do {
        p = 2.0*Vec3f(dis(gen), dis(gen), dis(gen));
    } while (p.squared_length() >= 1.0);
    return p;
}

Vec3f bgcolor(const Ray& r) {
    float t = (r.Direction().y() + 1.0) / 2.0;
    return float(1.0 - t)*Vec3f(1.0, 1.0, 1.0) + t*Vec3f(0.5, 0.7, 1.0);
}

Vec3f color(const Ray &r, Shape_List &world, int cur) {
    hit_record rec;
    cur += 1;
    if(cur <= 50 && world.IntersectRec(r, rec)) {
        Vec3f tar = rec.p + rec.normal + random_in_unit_sphere();
        Vec3f dir = unit_vector(tar - rec.p);
        return 0.5 * color(Ray(rec.p, dir), world, cur);
    } else {
        return bgcolor(r);
    }
}

int main(int argc, char const *argv[])
{
    std::ofstream output;
    output.open("output.ppm");

    int n_sample = 128;

    int nx = 200;
    int ny = 100;
    output << "P3\n" << nx << " " << ny << "\n255\n";

    Camera cam;

    Shape_List world;
    world.Add(make_unique<Sphere>(0.0, 0.0, -1.0, 0.5));
    world.Add(make_unique<Sphere>(0, -100.5, -1.0, 100));
    
    for (int j = ny-1; j>=0; j--) {
        for (int i = 0; i < nx; i++) {
            Vec3f col(0.0, 0.0, 0.0);
            for (int s = 0; s < n_sample; s++) {
                float u = float(i+dis(gen)) / float(nx);
                float v = float(j+dis(gen)) / float(ny);
                Ray r = cam.GetRay(u, v);
                col += color(r, world, 0);
            }
            col /= float(n_sample);
            col = Vec3f(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
            output << ir << " " << ig << " " << ib << "\n";
        }
    }

    output.close();
    return 0;
}