#pragma once

#include <cmath>
#include <memory>
#include <limits>
#include <iostream>
#include <cstdlib>


// Constants
constexpr double infinity = std::numeric_limits<double>::infinity();
constexpr double pi = 3.1415926535897932385;


// Utility functions

inline double degree_to_radians(double degrees)
{
	return degrees * pi / 180.0;
}

inline double random_double() { return std::rand() / (RAND_MAX + 1.0); } // 0 <= x < 1
inline double random_double(double min, double max) { return min + (max - min) * random_double(); }

#include "interval.h"
#include "color.h"
#include "ray.h"
#include "vec3.h"


