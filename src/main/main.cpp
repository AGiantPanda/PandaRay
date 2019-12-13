#include "../cores/pandaray.h"

#include "../cores/vec3.h"
#include "../cores/ray.h"
#include "../cores/camera.h"
#include "../shapes/sphere.h"
#include "../shapes/shapelist.h"
#include "../materials/lambertian.h"
#include "../materials/metal.h"

using namespace std;

Vec3f color(const Ray &r, Shape_List &world, int depth) {
    hit_record rec;
    if(world.IntersectRec(r, rec)) {
        Ray scattered;
        Vec3f attenuation;
        if (depth < 25 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * color(scattered, world, depth+1);
        } else {
            return Vec3f(0, 0, 0);
        }
    } else {
        // background color
        float t = (r.Direction().y() + 1.0) / 2.0;
        return float(1.0 - t)*Vec3f(1.0, 1.0, 1.0) + t*Vec3f(0.5, 0.7, 1.0);
    }
}

int main(int argc, char const *argv[])
{
	string filename = "output";

    int n_sample = 128;

    // image resolutions
    int nx = 800;
    int ny = 400;
    int totalPixel = nx * ny;

	filename = filename + "_" + to_string(nx) + "x" + to_string(ny) + ".ppm";
	cout << "target output: " << filename << endl;
	cout << "resolution: " << to_string(nx) << "x" << to_string(ny) << endl;

    ofstream output;
    output.open(filename);
    output << "P3\n" << nx << " " << ny << "\n255\n";

    Camera cam;

    Shape_List world;
    world.Add(make_unique<Sphere>(0.0, 0.0, -1.0, 0.5, make_shared<Lambertian>(Vec3f(0.8, 0.3, 0.3))));
    world.Add(make_unique<Sphere>(0, -100.5, -1.0, 100, make_shared<Lambertian>(Vec3f(0.8, 0.8, 0.0))));
    world.Add(make_unique<Sphere>(1, 0, -1, 0.5, make_shared<Metal>(Vec3f(0.8, 0.6, 0.2), 0.1)));
    world.Add(make_unique<Sphere>(-1, 0, -1, 0.5, make_shared<Metal>(Vec3f(0.8, 0.8, 0.8), 1.0)));
    
    clock_t begin = clock();
    cout << "rendering start..." << endl;

    int pixCount = 0;
    for (int j = ny-1; j>=0; j--) {
        for (int i = 0; i < nx; i++) {
            Vec3f col(0.0, 0.0, 0.0);
            for (int s = 0; s < n_sample; s++) {
                float u = float(i+random_double()) / float(nx);
                float v = float(j+random_double()) / float(ny);
                Ray r = cam.GetRay(u, v);
				r.tMin = 0.001;
                col += color(r, world, 0);
            }
            col /= float(n_sample);
            col = Vec3f(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
            output << ir << " " << ig << " " << ib << "\n";
            pixCount++;
            cout << "\rrendering progress: " << int(pixCount * 100 / totalPixel) << "%, " << pixCount << "/" << totalPixel << flush;
        }
    }

    cout << endl;

    output.close();

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "redering finished, time: " << elapsed_secs << endl;
    return 0;
}