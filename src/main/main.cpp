#include "../cores/pandaray.h"

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

Vec3f calcPixelColor(int w, int h, Camera& cam, Shape_List& world, int samples, int width, int height)
{
	Vec3f col(0.0, 0.0, 0.0);
	for (int s = 0; s < samples; s++) {
		float u = float(w + random_double()) / float(width);
		float v = float(h + random_double()) / float(height);
		Ray r = cam.GetRay(u, v);
		col += color(r, world, 0);
	}
	col /= float(samples);

	return col;
}

void calcPixelColorByThread(vector<int>& pic, Camera& cam, Shape_List& world, int samples, int width, int height, int ct = 1)
{

}

int main(int argc, char const *argv[])
{
	unsigned int c = std::thread::hardware_concurrency();
	std::cout << " number of threads detected: " << c << std::endl;;

	string filename = "output";

    int n_sample = 64;

    // image resolutions
    int width = 400;
    int height = 200;
    int totalPixel = width * height;
	float aspectRatio = float(width) / float(height);
	vector<Vec3f> outPicture(totalPixel);

	filename = filename + "_" + to_string(width) + "x" + to_string(height) + ".ppm";
	cout << "target output: " << filename << endl;
	cout << "resolution: " << to_string(width) << "x" << to_string(height) << endl;

    ofstream output;
    output.open(filename);
    output << "P3\n" << width << " " << height << "\n255\n";

	// world settings
	Vec3f lookFrom(0, 1, 3);
	Vec3f lookAt(0, 1, 0);
	float dist_to_focus = (lookFrom - lookAt).length();
	float aperture = 0.1;
	Camera cam(lookFrom, lookAt, Vec3f(0, 1, 0), 90, aspectRatio, aperture, dist_to_focus);

	Shape_List world;
	//genRandomWorld(world, 11);
	world.Add(make_unique<Sphere>(0.0, -100, 0.0, 100, make_shared<Lambertian>(Vec3f(0.8, 0.8, 0.0))));
	world.Add(make_unique<Sphere>(0.0, 1.0, 0.0, 1.0, make_shared<Lambertian>(Vec3f(0.1, 0.2, 0.5))));
	world.Add(make_unique<Sphere>(2, 1.0, 0.0, 1.0, make_shared<Metal>(Vec3f(0.8, 0.6, 0.2), 0.0)));
	world.Add(make_unique<Sphere>(-2, 1.0, 0.0, 1.0, make_shared<Dielectric>(1.5)));

	// begin rendering
    clock_t begin = clock();
    cout << "rendering start..." << endl;

   // int pixCount = 0;
   // for (int j = height-1; j>=0; j--) {
   //     for (int i = 0; i < width; i++) {
   //         Vec3f col = calcPixelColor(i, j, cam, world, n_sample, width, height);
			//col = gammaCorrection(col, 2.2);
   //         int ir = int(255.99 * col[0]);
   //         int ig = int(255.99 * col[1]);
   //         int ib = int(255.99 * col[2]);
   //         output << ir << " " << ig << " " << ib << "\n";
   //         pixCount++;
   //         cout << "\rrendering progress: " << int(pixCount * 100 / totalPixel) << "%, " << pixCount << "/" << totalPixel << flush;
   //     }
   // }

	// from left->right, top->down
	for (int pix = 0; pix < totalPixel; pix++)
	{
		int h = pix / width;
		int w = pix - h * width;
		Vec3f col = calcPixelColor(w, h, cam, world, n_sample, width, height);
		outPicture[pix] = col;
	}

	cout << "writing to output file...";
	for (int pix = 0; pix < totalPixel; pix++)
	{
		int ir = int(255.99 * outPicture[pix][0]);
		int ig = int(255.99 * outPicture[pix][1]);
		int ib = int(255.99 * outPicture[pix][2]);
		output << ir << " " << ig << " " << ib << "\n";
	}

    cout << "done" << endl;

    output.close();

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "redering finished, time: " << elapsed_secs << endl;
    return 0;
}