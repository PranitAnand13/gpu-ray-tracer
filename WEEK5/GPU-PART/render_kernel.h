#ifndef RENDER_KERNEL_H
#define RENDER_KERNEL_H

#include <CL/sycl.hpp>
#include "Vec3(sycl).h"
#include "ray(sycl).h"
#include "sphere(sycl).h"
#include "ray_color.h"
#include "constants.h"

// Define the render kernel template
template<int width, int height>
class render_kernel {
public:
    render_kernel(sycl::accessor<Vec3, 1, sycl::access::mode::write> frame_ptr,
                  sycl::accessor<Sphere, 1, sycl::access::mode::read> spheres,
                  int num_spheres)
        : m_frame_ptr(frame_ptr), m_spheres(spheres), m_num_spheres(num_spheres) {}

    void operator()(sycl::nd_item<2> item) const {
        int i = item.get_global_id(1);
        int j = item.get_global_id(0);
        if (i >= height || j >= width) return;

        int global_id = item.get_global_linear_id();
        uint32_t seed = global_id * 9781 + 1;

        Vec3 color(0, 0, 0);
        constexpr int samples_per_pixel = 100;

        for (int s = 0; s < samples_per_pixel; ++s) {
            float u = (j + random_float(seed)) / (width - 1);
            float v = (i + random_float(seed)) / (height - 1);

            Vec3 origin(0, 0, 0);
            Vec3 horizontal(4.0, 0.0, 0.0);
            Vec3 vertical(0.0, 2.25, 0.0);
            Vec3 lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, 1);

            ray r(origin, lower_left_corner + horizontal * u + vertical * v - origin);
            color = color + ray_color(r, m_spheres.get_pointer(), m_num_spheres, 50, seed);
        }

        color = color/float(samples_per_pixel);
        color = Vec3(sycl::sqrt(color.x()), sycl::sqrt(color.y()), sycl::sqrt(color.z()));
        m_frame_ptr[i * width + j] = color;
    }

private:
    sycl::accessor<Vec3, 1, sycl::access::mode::write> m_frame_ptr;
    sycl::accessor<Sphere, 1, sycl::access::mode::read> m_spheres;
    int m_num_spheres;
};

#endif
