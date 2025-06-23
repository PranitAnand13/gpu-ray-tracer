#ifndef RENDER_H
#define RENDER_H

#include <CL/sycl.hpp>
#include <vector>

#include "Vec3(sycl).h"
#include "constants.h"
#include "render_kernel.h"
#include "sphere(sycl).h"

template<int width, int height>
void render(sycl::queue& q, Vec3* framebuffer, sycl::buffer<Sphere, 1>& sphere_buf, int sphere_count)
{
    // Create SYCL buffer for output framebuffer
    sycl::buffer<Vec3, 1> frame_buf(framebuffer, sycl::range<1>(width * height));

    q.submit([&](sycl::handler& h) {
        auto frame_ptr = frame_buf.get_access<sycl::access::mode::write>(h);
        auto sphere_ptr = sphere_buf.get_access<sycl::access::mode::read>(h);

        auto global = sycl::range<2>(width, height);
        auto local = sycl::range<2>(constants::TileX, constants::TileY);
        auto nd_range = sycl::nd_range<2>(global, local);

        render_kernel<width, height> kernel(frame_ptr, sphere_ptr, sphere_count);
        h.parallel_for(nd_range, kernel);
    });  // correct: closes q.submit
}       // correct: closes render


#endif

