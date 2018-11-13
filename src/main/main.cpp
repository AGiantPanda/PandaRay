#include "./cores/pandaray.h"

#include "../cores/vec3.h"
#include "../cores/ray.h"
#include "../cores/camera.h"
#include "../shapes/sphere.h"
#include "../shapes/shapelist.h"

using namespace std;

Vec3f bgcolor(const Ray& r) {
    float t = (r.Direction().y() + 1.0) / 2.0;
    return float(1.0 - t)*Vec3f(1.0, 1.0, 1.0) + t*Vec3f(0.5, 0.7, 1.0);
}

Vec3f color(const Ray &r, Shape_List &world) {
    hit_record rec;
    if(world.IntersectRec(r, rec)) {
        return 0.5 * Vec3f(rec.normal.x()+1, rec.normal.y()+1, rec.normal.z()+1);
    } else {
        return bgcolor(r);
    }
}

int main(int argc, char const *argv[])
{
    std::ofstream output;
    output.open("output.ppm");

    int n_sample = 128;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> dis(0.0, 1.0);

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
                col += color(r, world);
            }
            col /= float(n_sample);
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
            output << ir << " " << ig << " " << ib << "\n";
        }
    }

    output.close();
    return 0;
}