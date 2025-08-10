#pragma once

#include <cmath>
#include <memory>
#include <limits>
#include <iostream>

#include "color.h"
#include "ray.h"
#include "vec3.h"
#include "interval.h"

// Constants
constexpr double infinity = std::numeric_limits<double>::infinity();
constexpr double pi = 3.1415926535897932385;


// Utility functions

inline double degree_to_radians(double degrees)
{
	return degrees * pi / 180.0;
}

