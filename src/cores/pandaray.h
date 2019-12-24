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

inline double random_double()
{
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	static std::function<double()> rand_generator = std::bind(distribution, generator);
	return rand_generator();
}

static float Infinity = std::numeric_limits<float>::infinity();
static float Epsilon = std::numeric_limits<float>::epsilon() * 0.1;

static float Pi = 3.14159265358979323846;
static float InvPi = 0.31830988618379067154;
static float Inv2Pi = 0.15915494309189533577;

#include "../cores/vec3.h"
#include "../cores/ray.h"
#include "../cores/camera.h"

#include "../cores/shape.h"
#include "../shapes/shapelist.h"
#include "../shapes/sphere.h"

#include "../cores/material.h"
#include "../materials/lambertian.h"
#include "../materials/metal.h"
#include "../materials/dielectric.h"

#endif