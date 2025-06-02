#pragma once

/***************************************************************************
 *
 *  Copyright (C) 2016 Codeplay Software Limited
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  For your convenience, a copy of the License has been included in this
 *  repository.
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  Codeplay's ComputeCpp SDK
 *
 *  reduction.cpp
 *
 *  Description:
 *    Example of a reduction operation in SYCL.
 *
 **************************************************************************/

// https://www.intel.com/content/www/us/en/docs/oneapi/optimization-guide-gpu/2023-0/considerations-for-selecting-work-group-size.html
// The maximum work-group size can be queried by the call device::get_info<cl::sycl::info::device::max_work_group_size>().

#include <sycl/sycl.hpp>

#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>
#include <kwk/concepts/container.hpp>

template <typename T>
class sycl_reduction;

/* Implements a reduction of an STL vector using SYCL.
 * The input vector is not modified. */
template <concepts::container In, typename Func>
T sycl_reduce(::sycl::queue& q, In const& arg_vector, Func func, auto init)
{
  T retVal;
  {

    std::size_t in_len = arg_vector.numel();

    /* Output device and platform information. */
    auto device = q.get_device();
    auto deviceName = device.get_info<::sycl::info::device::name>();
    std::cout << " Device Name: " << deviceName << std::endl;
    auto platformName =  device.get_platform().get_info<::sycl::info::platform::name>();
    std::cout << " Platform Name " << platformName << std::endl;

    /* The buffer is used to initialise the data on the device, but we don't
     * want to copy back and trash it. buffer::set_final_data() tells the
     * SYCL runtime where to put the data when the buffer is destroyed; nullptr
     * indicates not to copy back. The vector's length is used as the global
     * work size (unless that is too large). */
    ::sycl::buffer<int, 1> buf_input(arg_vector.data(), ::sycl::range<1>(in_len));
    buf_input.set_final_data(nullptr);

    std::size_t local = std::min( in_len
                                , device.get_info<::sycl::info::device::max_work_group_size>()
                                );

    {
      /* Each iteration of the do loop applies one level of reduction until
       * the input is of length 1 (i.e. the reduction is complete). */
      do {
        auto f = [in_len, local, &buf_input](::sycl::handler& h) mutable
        {
          ::sycl::nd_range<1> range { ::sycl::range<1>{std::max(in_len, local)}
                                    , ::sycl::range<1>{std::min(in_len, local)}
                                    };

          /* Two accessors are used: one to the buffer that is being reduced,
           * and a second to local memory, used to store intermediate data. */
          auto aI = buf_input.template get_access<::sycl::access::mode::read_write>(h);
          
          ::sycl::local_accessor<T, 1> scratch(::sycl::range<1>(local), h);

          /* The parallel_for invocation chosen is the variant with an nd_item
           * parameter, since the code requires barriers for correctness. */
          h.parallel_for<sycl_reduction<T>>(
              range, [aI, scratch, local, in_len](::sycl::nd_item<1> id) {
                std::size_t globalid = id.get_global_id(0);
                std::size_t localid = id.get_local_id(0);

                /* All threads collectively read from global memory into local.
                 * The barrier ensures all threads' IO is resolved before
                 * execution continues (strictly speaking, all threads within
                 * a single work-group - there is no co-ordination between
                 * work-groups, only work-items). */
                if (globalid < in_len) {
                  scratch[localid] = aI[globalid];
                }
                id.barrier(::sycl::access::fence_space::local_space);

                /* Apply the reduction operation between the current local
                 * id and the one on the other half of the vector. */
                if (globalid < in_len) {
                  int min = (in_len < local) ? in_len : local;
                  for (size_t offset = min / 2; offset > 0; offset /= 2) {
                    if (localid < offset) {
                      scratch[localid] = func(scratch[localid], scratch[localid + offset]);
                    }
                    id.barrier(::sycl::access::fence_space::local_space);
                  }
                  /* The final result will be stored in local id 0. */
                  if (localid == 0) {
                    aI[id.get_group(0)] = scratch[localid];
                  }
                }
              });
        };
        q.submit(f);
        /* At this point, you could queue::wait_and_throw() to ensure that
         * errors are caught quickly. However, this would likely impact
         * performance negatively. */
        in_len = in_len / local;
      } while (in_len > 1);
    }

    {
      /* It is always sensible to wrap host accessors in their own scope as
       * kernels using the buffers they access are blocked for the in_len
       * of the accessor's lifetime. */
      ::sycl::host_accessor hI(buf_input);

      retVal = hI[0];
    }
  }
  return retVal;
}