/*
 * This file is part of hipSYCL, a SYCL implementation based on CUDA/HIP
 *
 * Copyright (c) 2018 Aksel Alpay
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <cassert>
#include <iostream>

#include <CL/sycl.hpp>


using data_type = int;

const size_t local_size = 256;
const size_t global_size = 1024;

int main()
{
  cl::sycl::queue q;

  std::vector<data_type> input;
  for(std::size_t i = 0; i < global_size; ++i)
    input.push_back(static_cast<data_type>(i));

  {
    cl::sycl::buffer<data_type> input_buffer{input.data(), input.size()};

    q.submit([&](cl::sycl::handler& cgh)
    {
      auto access_input =
          input_buffer.get_access<cl::sycl::access::mode::read_write>(cgh);

      auto scratch = cl::sycl::accessor<
          data_type, 1,
          cl::sycl::access::mode::read_write,
          cl::sycl::access::target::local>{local_size, cgh};

      cl::sycl::nd_range<1> execution_range{global_size, local_size};
      cgh.parallel_for<class reduction>(execution_range,
                     [=] (cl::sycl::nd_item<1> this_item)
      {
        const size_t lid = this_item.get_local(0);

        scratch[lid] = access_input[this_item.get_global()];

        this_item.barrier();

        for(size_t i = local_size/2; i > 0; i /= 2)
        {
          if(lid < i)
            scratch[lid] += scratch[lid + i];
          this_item.barrier();
        }

        if(lid == 0)
          access_input[this_item.get_global()] = scratch[0];
      });
    });
  }

  std::cout << "Computed local sums:" << std::endl;
  for(std::size_t i = 0; i < global_size/local_size; ++i)
    std::cout << input[i * local_size] << std::endl;

  std::cout << "Expected results:" << std::endl;
  for(std::size_t i = 0; i < global_size/local_size; ++i)
  {
    std::size_t sum = 0;
    for(std::size_t j = 0; j < local_size; ++j)
      sum += i * local_size + j;
    std::cout << sum << std::endl;
  }
}
