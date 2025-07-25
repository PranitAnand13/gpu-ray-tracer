#include <CL/sycl.hpp>
#include <iostream>
#include <vector>

#include "constants.h"
#include "Vec3(sycl).h"
#include "ray(sycl).h"
#include "sphere(sycl).h"
#include "render.h"

int main() {
    constexpr int image_width = 400;
    constexpr int image_height = 225;

    std::vector<Vec3> framebuffer(image_width * image_height);

    {
        std::vector<Sphere> spheres;

        // Red center Lambertian sphere
        spheres.emplace_back(Vec3(0, 0, -1.2), 0.5, LAMBERTIAN, Vec3(0.7, 0.3, 0.3));

        // Yellow ground Lambertian sphere
        spheres.emplace_back(Vec3(0, -100.5, -1), 100, LAMBERTIAN, Vec3(0.8, 0.8, 0.0));

        // Shiny metal sphere
        spheres.emplace_back(Vec3(1, 0, -1), 0.5, METAL, Vec3(0.8, 0.6, 0.2));

        // Glass-like dielectric sphere (using fuzz to encode refractive index)
        spheres.emplace_back(Vec3(-1, 0, -1), 0.5, DIELECTRIC, Vec3(1.0, 1.0, 1.0), 0.0,1.5);
        spheres.emplace_back(Vec3(-1, 0, -1), 0.4, DIELECTRIC, Vec3(1.0, 1.0, 1.0), 0.0,1.0/1.5);
        sycl::buffer<Sphere, 1> sphere_buf(spheres.data(), sycl::range<1>(spheres.size()));

        sycl::queue q;
        render<image_width, image_height>(q, framebuffer.data(), sphere_buf, static_cast<int>(spheres.size()));
        q.wait();
    }

    // Output to PPM
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int j = image_height - 1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {
            Vec3 pixel = framebuffer[j * image_width + i];
            int ir = static_cast<int>(255.999f * pixel.x());
            int ig = static_cast<int>(255.999f * pixel.y());
            int ib = static_cast<int>(255.999f * pixel.z());
            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }

    return 0;
}
