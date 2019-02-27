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

#ifndef HIPSYCL_TYPES_HPP
#define HIPSYCL_TYPES_HPP

#include <vector>
#include <string>
#include <functional>
#include <thread>
#include <memory>
#include <exception>
#include <mutex>
#include <shared_mutex>

namespace cl {
namespace sycl {

#ifndef CL_SYCL_NO_STD_VECTOR
template<class T, class Alloc = std::allocator<T>>
using vector_class = std::vector<T, Alloc>;
#endif

#ifndef CL_SYCL_NO_STD_STRING
using string_class = std::string;
#endif

#ifndef CL_SYCL_NO_STD_FUNCTION
template<class Func>
using function_class = std::function<Func>;
#endif

#ifndef CL_SYCL_NO_STD_MUTEX
using mutex_class = std::mutex;
#endif

#ifndef CL_SYCL_NO_STD_SHARED_MUTEX
using shared_mutex_class = std::shared_timed_mutex;
#endif

#ifndef CL_SYCL_NO_STD_UNIQUE_PTR
template<class T>
using unique_ptr_class = std::unique_ptr<T>;
#endif

#ifndef CL_SYCL_NO_STD_SHARED_PTR
template<class T>
using shared_ptr_class = std::shared_ptr<T>;
#endif

#ifndef CL_SYCL_NO_STD_WEAK_PTR
template<class T>
using weak_ptr_class = std::weak_ptr<T>;
#endif

#ifndef CL_SYCL_NO_HASH
template<class T>
using hash_class = std::hash<T>;
#endif

using exception_ptr_class = std::exception_ptr;


using exception_ptr = exception_ptr_class;
using exception_list = vector_class<exception_ptr>;
using async_handler = function_class<void(cl::sycl::exception_list)>;

// \todo Better use uint32_t etc
using cl_uchar = unsigned char;
using cl_ushort = unsigned short;
using cl_uint = unsigned;
using cl_ulong = unsigned long long;

using cl_char = char;
using cl_short = short;
using cl_int = int;
using cl_long = long long;

using cl_float = float;
using cl_double = double;
// ToDo: Proper half type
using cl_half = cl_ushort;

}
}

// Only pull typedefs into global namespace if the OpenCL headers
// defining them haven't yet been pulled in
#ifndef CL_TARGET_OPENCL_VERSION
using cl::sycl::cl_uchar;
using cl::sycl::cl_ushort;
using cl::sycl::cl_uint;
using cl::sycl::cl_ulong;

using cl::sycl::cl_char;
using cl::sycl::cl_short;
using cl::sycl::cl_int;
using cl::sycl::cl_long;

using cl::sycl::cl_float;
using cl::sycl::cl_double;
using cl::sycl::cl_half;
#endif

#endif
