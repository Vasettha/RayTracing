#include "main.h"

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"



color ray_color(const ray& r, const hittable& world)
{
    hit_record rec;

    if (world.hit(r, interval(0, infinity), rec)) return 0.5 * (rec.normal + color(1, 1, 1));

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

int main() {

    // Image
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int image_height = image_width / aspect_ratio;
    if (image_height == 0) image_height = 1;

    // Camera
    double focal_length = 1.0;
    double viewport_height = 2.0;
    double viewport_width = viewport_height * (double(image_width)/image_height);
    point3 camera_center(0, 0, 0);

    // World

    hittable_list world;

    world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100));

    // Vector u and v (in terms of screen)
    vec3 viewport_u(viewport_width, 0, 0);
    vec3 viewport_v(0, -viewport_height, 0);

    vec3 pixel_delta_u = viewport_u / image_width;
    vec3 pixel_delta_v = viewport_v / image_height;

    vec3 viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    vec3 pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j)
    {
        // Logging
        std::clog << "\rScanlines Remaining: " << image_height - j - 1 << ' ' << std::flush;

        for (int i = 0; i < image_width; ++i)
        {
            vec3 pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            vec3 ray_direction = pixel_center - camera_center;

            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r, world);

            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.                         " << std::flush;
}