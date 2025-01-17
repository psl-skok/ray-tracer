#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "cube.h"
#include "camera.h"
#include "material.h"


#include <iostream>
color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0,0,0);

    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth-1);
        return color(0,0,0);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

hittable_list random_scene() {
    hittable_list world;


    auto ground_material = make_shared<checker_texture>(
        make_shared<lambertian>(color(0.1, 0.2, 0.5)),
        make_shared<lambertian>(color(0.9, 0.9, 0.9))
    );
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

int main() {

    // Image

    const auto aspect_ratio = 3.0 / 2.0;
    const int image_width = 1200;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    // World
    //auto R = cos(pi/4);
    //hittable_list world;
    auto world = random_scene();

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<checker_texture>(
        make_shared<lambertian>(color(0.1, 0.2, 0.5)),
        make_shared<lambertian>(color(0.9, 0.9, 0.9))
    );
    auto material_left   = make_shared<dielectric>(1.5);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

        //New Stuff    
    auto material_metal  = make_shared<metal>(color(0.8, 0.8, 0.8), 0.0);
    auto material_metal_blue = make_shared<metal>(color(0.2, 0.4, 0.8), 0.1);
    auto material_metal_green = make_shared<metal>(color(0.1, 0.7, 0.3), 0.2);
    auto material_metal_magenta = make_shared<metal>(color(0.8, 0.1, 0.8), 0.2);

    auto material_red = make_shared<lambertian>(color(0.8, 0.1, 0.1));
    auto material_green = make_shared<lambertian>(color(0.1, 0.8, 0.1));
    auto material_blue = make_shared<lambertian>(color(0.1, 0.1, 0.8));
    auto material_yellow = make_shared<lambertian>(color(0.8, 0.8, 0.1));
    auto material_cyan = make_shared<lambertian>(color(0.1, 0.8, 0.8));
    auto material_magenta = make_shared<lambertian>(color(0.8, 0.1, 0.8));
    


    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_center));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_ground));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

        //New Stuff
    world.add(make_shared<sphere>(point3( 0.0, 4.0, -1.0),   2, material_metal));
    world.add(make_shared<sphere>(point3(-2.0, 0.5, 0.5), 0.1, material_red));
    world.add(make_shared<sphere>(point3(5.5, 3.5, -2.0), 0.1, material_green));
    world.add(make_shared<sphere>(point3(2.0, 2.2, -1.0), 0.1, material_blue));
    world.add(make_shared<sphere>(point3(2.0, 0.3, -1.5), 0.3, material_magenta));
    world.add(make_shared<sphere>(point3(0.0, 0.4, -1.0), 0.4, material_cyan));
    world.add(make_shared<sphere>(point3(-2.0, 1.5, -10.5), 3, material_metal_blue));
    world.add(make_shared<sphere>(point3(2.0, 1.2, -13.0), 1.5, material_metal_green));
    world.add(make_shared<sphere>(point3(-3.0, 1.5, -9.5), 2.5, material_metal_magenta));
    world.add(make_shared<sphere>(point3(-4.0, 1.5, -8.5), 2, material_metal_blue));
    world.add(make_shared<sphere>(point3(-5.0, 1.5, -7.5), 1.5, material_metal_magenta));

    world.add(make_shared<cube>(point3(-3.0, 3.0, -1.0), 0.5, make_shared<metal>(color(0.8, 0.8, 0.8), 0.0)));
    world.add(make_shared<cube>(point3(3.0, 3.0, -1.0), 0.5, make_shared<dielectric>(1.5)));




    // Camera
    point3 lookfrom(0, 1, 2.5);      // Camera position
    point3 lookat(0, 1, 0);      // Point the camera towards the center sphere
    vec3 vup(0, 1, 0);            // Define the up vector
    auto dist_to_focus = 1.0;     // Focus distance
    auto aperture = 0.0;          // Aperture (0.0 means no depth of field)

    camera cam(lookfrom, lookat, vup, 90, aspect_ratio, aperture, dist_to_focus);


    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}