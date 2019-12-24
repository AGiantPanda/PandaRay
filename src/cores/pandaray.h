#pragma once

#ifndef __PANDARAY__
#define __PANDARAY__

#include <iostream>
#include <fstream>
#include <limits>
#include <vector>
#include <memory>
#include <string>
#include <functional>
#include <random>
#include <ctime>
#include <thread>

static float Infinity = std::numeric_limits<float>::infinity();
static float Epsilon = std::numeric_limits<float>::epsilon() * 0.1;

static float Pi = 3.14159265358979323846;
static float InvPi = 0.31830988618379067154;
static float Inv2Pi = 0.15915494309189533577;

#include "../cores/vec3.h"
#include "../cores/ray.h"

// some maths
inline float random_double()
{
	static std::uniform_real_distribution<float> distribution(0.0, 1.0);
	static std::mt19937 generator;
	static std::function<float()> rand_generator = std::bind(distribution, generator);
	return rand_generator();
}

inline Vec3f random_in_unit_sphere() {
	Vec3f p;
	do {
		p = 2.0*Vec3f(random_double(), random_double(), random_double()) - Vec3f(1, 1, 1);
	} while (p.squared_length() >= 1.0);
	return p;
}

inline Vec3f random_in_unit_disk()
{
	Vec3f p;

	do
	{
		p = 2.0 * Vec3f(random_double(), random_double(), 0) - Vec3f(1, 1, 0);
	} while (dot(p, p) >= 1.0);

	return p;
}

#include "../cores/camera.h"

// #include "../cores/shape.h"
// #include "../cores/material.h"

// Global Forward Declaration
class Shape;
class Material;
struct hit_record;

#endif