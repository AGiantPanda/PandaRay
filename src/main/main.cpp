#include "../cores/pandaray.h"

#include "../cores/vec3.h"
#include "../cores/ray.h"
#include "../cores/camera.h"
#include "../shapes/sphere.h"
#include "../shapes/shapelist.h"
#include "../materials/lambertian.h"
#include "../materials/metal.h"
#include "../materials/dielectric.h"

using namespace std;

Vec3f color(const Ray &r, Shape_List &world, int depth) {
    hit_record rec;
    if(world.IntersectRec(r, rec)) {
        Ray scattered;
        Vec3f attenuation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
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

Vec3f gammaCorrection(Vec3f &color, float gamma)
{
	float gamma_exp = 1 / gamma;
	color[0] = powf(color[0], gamma_exp);
	color[1] = powf(color[1], gamma_exp);
	color[2] = powf(color[2], gamma_exp);

	return color;
}

void genRandomWorld(Shape_List &world, int num)
{
	world.Add(make_unique<Sphere>(0.0, -1000.0, 0.0, 1000, make_shared<Lambertian>(Vec3f(0.5, 0.5, 0.5))));

	for (int i = -num; i < num; i++)
	{
		for (int j = -num; j < num; j++)
		{
			float ranMat = random_double();
			Vec3f ranCenter(i + 0.9*random_double(), 0.2, j + 0.9*random_double());
			float ranRadius = random_double() * 0.1 + 0.2;
			if ((ranCenter - Vec3f(4, 0.2, 0)).length() > 0.9)
			{
				if (ranMat < 0.6)
				{
					// diffuse
					world.Add(make_unique<Sphere>(ranCenter, ranRadius, make_shared<Lambertian>(Vec3f(
						random_double()*random_double(), 
						random_double()*random_double(), 
						random_double()*random_double()))));
				}
				else if (ranMat < 0.85)
				{
					// metal
					world.Add(make_unique<Sphere>(ranCenter, ranRadius, make_shared<Metal>(Vec3f(
						0.5*(1 + random_double()),
						0.5*(1 + random_double()),
						0.5*(1 + random_double())),
						0.5*random_double())));
				}
				else
				{
					// dielectric
					world.Add(make_unique<Sphere>(ranCenter, ranRadius, make_shared<Dielectric>(1.5)));
				}
			}
		}
	}

	world.Add(make_unique<Sphere>(0.0, 1.0, 0.0, 1, make_shared<Dielectric>(1.5)));
	world.Add(make_unique<Sphere>(-4.0, 1.0, 0.0, 1, make_shared<Lambertian>(Vec3f(0.4, 0.2, 0.1))));
	world.Add(make_unique<Sphere>(4.0, 1.0, 0.0, 1, make_shared<Metal>(Vec3f(0.7, 0.6, 0.5), 0.0)));
}

int main(int argc, char const *argv[])
{
	string filename = "output";

    int n_sample = 256;

    // image resolutions
    int nx = 800;
    int ny = 400;
    int totalPixel = nx * ny;
	float aspectRatio = float(nx) / float(ny);

	filename = filename + "_" + to_string(nx) + "x" + to_string(ny) + ".ppm";
	cout << "target output: " << filename << endl;
	cout << "resolution: " << to_string(nx) << "x" << to_string(ny) << endl;

    ofstream output;
    output.open(filename);
    output << "P3\n" << nx << " " << ny << "\n255\n";

	// world settings
	Vec3f lookFrom(8, 2, 2);
	Vec3f lookAt(0, 0, 0);
	float dist_to_focus = (lookFrom - lookAt).length();
	float aperture = 1.0;
    Camera cam(lookFrom, lookAt, Vec3f(0, 1, 0), 40, aspectRatio, aperture, dist_to_focus);

    Shape_List world;
	genRandomWorld(world, 11);
 //   world.Add(make_unique<Sphere>(0.0, 0.0, -1.0, 0.5, make_shared<Lambertian>(Vec3f(0.1, 0.2, 0.5))));
 //   world.Add(make_unique<Sphere>(0, -100.5, -1.0, 100, make_shared<Lambertian>(Vec3f(0.8, 0.8, 0.0))));
 //   world.Add(make_unique<Sphere>(1, 0, -1, 0.5, make_shared<Metal>(Vec3f(0.8, 0.6, 0.2), 0.0)));
	//world.Add(make_unique<Sphere>(-1, 0, -1, 0.5, make_shared<Dielectric>(1.5)));

	// begin rendering
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
                col += color(r, world, 0);
            }
            col /= float(n_sample);
			col = gammaCorrection(col, 2.2);
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