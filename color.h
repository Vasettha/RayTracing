#pragma once

#include "vec3.h"

using color = vec3;

inline double linear_to_gamma(double linear_component)
{
	if (linear_component > 0) return std::sqrt(linear_component);
	return 0;
}

void write_color(std::ostream& out, const color& pixel_color)
{
	double r = pixel_color.x();
	double g = pixel_color.y();
	double b = pixel_color.z();

	// Linear to gamma
	r = linear_to_gamma(r);
	g = linear_to_gamma(g);
	b = linear_to_gamma(b);

	static const interval intensity(0.0000, 0.9999);

	int rbyte = static_cast<int>(256 * intensity.clamp(r));
	int gbyte = static_cast<int>(256 * intensity.clamp(g));
	int bbyte = static_cast<int>(256 * intensity.clamp(b));

	out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}