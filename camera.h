#pragma once

#include "hittable.h"
#include "material.h"

class camera
{
public:
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int sample_per_pixel = 100;
    int max_depth = 10;

    double vfov = 90;
    point3 lookfrom = point3(0, 0, 0);
    point3 lookat = point3(0, 0, -1);
    vec3 vup = vec3(0, 1, 0);

	void render(const hittable& world)
    {
        initialize();

        // Render
        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; ++j)
        {
            // Logging
            std::clog << "\rScanlines Remaining: " << image_height - j - 1 << ' ' << std::flush;

            for (int i = 0; i < image_width; ++i)
            {
                color pixel_color(0, 0, 0);
                
                for (int sample = 0; sample < sample_per_pixel; ++sample)
                {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }

                write_color(std::cout, pixel_samples_scale * pixel_color);
            }
        }

        std::clog << "\rDone.                         " << std::flush;
    }

private:
    int image_height;
    point3 center;
    point3 pixel00_loc;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;
    double pixel_samples_scale;
    vec3 u, v, w;
    
	void initialize() 
    {
        // Image
        image_height = image_width / aspect_ratio;
        if (image_height == 0) image_height = 1;

        pixel_samples_scale = 1.0 / sample_per_pixel;

        // Camera
        center = lookfrom;
        double focal_length = (lookfrom - lookat).length();
        auto theta = degree_to_radians(vfov);
        auto h = std::tan(theta / 2);
        double viewport_height = 2 * h * focal_length;
        double viewport_width = viewport_height * (double(image_width) / image_height);

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);
        

        // Vector u and v (in terms of screen)
        vec3 viewport_u = viewport_width * u;
        vec3 viewport_v = viewport_height * -v;

        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        vec3 viewport_upper_left = center - focal_length * w - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    color ray_color(const ray& r, int max_depth, const hittable& world)
    {
        if (max_depth <= 0) return color(0, 0, 0);
        hit_record rec;

        if (world.hit(r, interval(0.001, infinity), rec))
        {
            ray scattered;
            color attenuation;

            if (rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, max_depth - 1, world);
            return color(0, 0, 0);
        }

        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }

    ray get_ray(int i, int j)
    {
        auto offset = sample_square();
        vec3 pixel_center = pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);
        vec3 ray_direction = pixel_center - center;

        return ray(center, ray_direction);
    }

    vec3 sample_square() const { return vec3(random_double() - 0.5, random_double() - 0.5, 0); }
};

