#pragma once

#ifndef __PANDARAY__
#define __PANDARAY__

#include <iostream>
#include <fstream>
#include <limits>
#include <vector>
#include <memory>
#include <random>


static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_real_distribution<float> dis(-0.5, 0.5);

static float Infinity = std::numeric_limits<float>::infinity();
static float Epsilon = std::numeric_limits<float>::epsilon() * 0.1;

static float Pi = 3.14159265358979323846;
static float InvPi = 0.31830988618379067154;
static float Inv2Pi = 0.15915494309189533577;

#include "../cores/vec3.h"
#include "../cores/ray.h"

// #include "../cores/shape.h"
// #include "../cores/material.h"

// Global Forward Declaration
class Shape;
class Material;
struct hit_record;



#endif