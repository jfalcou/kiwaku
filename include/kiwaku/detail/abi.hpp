//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

//======================================================================================================================
// C++ version check
//======================================================================================================================
#if defined(_MSC_VER)
#if _MSVC_LANG < 202002L
#error "KWK C++ version error"
#include "KWK requires C++20 or higher. Use /std:c++20 or higher to enable C++20 features."
#endif
#else
#if __cplusplus < 202002L
#error "KWK C++ version error"
#include "KWK requires C++20 or higher. Use -std=c++20 or higher to enable C++20 features."
#endif
#endif

#define KWK_FWD(...) static_cast<decltype(__VA_ARGS__)&&>(__VA_ARGS__)

//======================================================================================================================
/**
 Frontend  detection

 Order matters, compilers can impersonate others.
   - EDG   : nvcc (cudafe++) and other EDG compilers. Defines __GNUC__ / _MSC_VER too, so it is
tested first.
   - clang : defines __GNUC__, and also _MSC_VER for clang-cl. Tested before gcc and msvc.
   - gcc   : anything else defining __GNUC__
   - msvc  : cl.exe
**/
//======================================================================================================================
#if defined(__EDG__) || defined(__EDG_VERSION__) || defined(__NVCC__)
#define KWK_FRONTEND_EDG 1
#elif defined(__clang__)
#define KWK_FRONTEND_CLANG 1
#elif defined(__GNUC__)
#define KWK_FRONTEND_GCC 1
#elif defined(_MSC_VER)
#define KWK_FRONTEND_MSVC 1
#endif

//======================================================================================================================
/**
  Device backend detection

   - NVIDIA : nvcc, clang -x cuda, and hipcc when HIP targets the NVIDIA platform.
   - AMD    : clang -x hip (hipcc on ROCm).

 The HIP platform macros are tested first: on the NVIDIA platform __CUDACC__ is also defined.
**/
//======================================================================================================================
#if defined(__HIP_PLATFORM_NVIDIA__) || defined(__HIP_PLATFORM_NVCC__)
#define KWK_DEVICE_NVIDIA 1
#elif defined(__HIP__) || defined(__HIPCC__) || defined(__HIP_PLATFORM_AMD__) ||                   \
    defined(__HIP_PLATFORM_HCC__)
#define KWK_DEVICE_AMD 1
#elif defined(__CUDACC__) || defined(__NVCC__) || defined(__CUDA__)
#define KWK_DEVICE_NVIDIA 1
#endif

#if defined(KWK_DEVICE_NVIDIA) || defined(KWK_DEVICE_AMD)
#define KWK_GPU_COMPILER 1
#define KWK_GPU          __host__ __device__
#else
#define KWK_GPU
#endif

// Are we currently generating device code?
#if (defined(KWK_DEVICE_NVIDIA) && defined(__CUDA_ARCH__)) ||                                      \
    (defined(KWK_DEVICE_AMD) && defined(__HIP_DEVICE_COMPILE__))
#define KWK_DEVICE_PASS 1
#endif

//======================================================================================================================
/**
  KWK_ABI : public functions. Attributes are fixed per frontend.

  frontend  always_inline              flatten             artificial
  --------  -------------------------  ------------------  ----------------
   EDG       __forceinline__            (not available)     (not available)
   clang     [[clang::always_inline]]   (too aggressive)    [[gnu::artificial]]
   gcc       [[gnu::always_inline]]     [[gnu::flatten]]    [[gnu::artificial]]
   msvc      [[msvc::forceinline]]      [[msvc::flatten]]   (not available)

  The device backend does not change the spelling: it is the frontend that parses it.
  Flatten has been removed from clang as it already inlines too much
**/
//======================================================================================================================
#if defined(KWK_DEBUG)
#define KWK_ABI KWK_GPU
#elif defined(KWK_DEVICE_PASS)
#define KWK_ABI KWK_GPU __forceinline__
#elif defined(KWK_FRONTEND_EDG)
#define KWK_ABI inline
#elif defined(KWK_FRONTEND_CLANG)
#define KWK_ABI [[clang::always_inline, gnu::artificial]] KWK_GPU inline
#elif defined(KWK_FRONTEND_GCC)
#define KWK_ABI [[using gnu: always_inline, flatten, artificial]] KWK_GPU inline
#elif defined(KWK_FRONTEND_MSVC)
#define KWK_ABI [[msvc::forceinline, msvc::flatten]] KWK_GPU inline
#else
#define KWK_ABI KWK_GPU inline
#endif

//======================================================================================================================
// KWK_HIDDEN_ABI : functions in namespace detail. Not force-inlined, so they keep a distinct ABI.
//======================================================================================================================
#define KWK_HIDDEN_ABI KWK_GPU inline

//======================================================================================================================
/**
  KWK_VARIABLE_ABI : namespace-scope objects.

  A namespace-scope object lives in host memory, so a kernel naming kumi::apply finds nothing under
  nvcc or hipcc. The device pass gets a copy of its own; every other pass keeps the inline variable.
**/
//======================================================================================================================
#if defined(KWK_DEVICE_PASS)
#define KWK_VARIABLE_ABI __device__
#else
#define KWK_VARIABLE_ABI inline
#endif

//======================================================================================================================
/**
  KWK_ERROR : misuse reporting.

  Device code has no exceptions: the same misuse aborts the kernel there. __builtin_trap() is the
  portable spelling that the NVIDIA (nvcc, clang) and AMD (clang) device back-ends lower to a trap.
**/
//======================================================================================================================
#if defined(KWK_DEVICE_PASS)
#define KWK_ERROR(MESSAGE) __builtin_trap()
#else
#define KWK_ERROR(MESSAGE) throw MESSAGE
#endif

//======================================================================================================================
/**
  KWK_UNREACHABLE : invokes UB.

  Device code has no exceptions: the same misuse aborts the kernel there. __builtin_trap() is the
  portable spelling that the NVIDIA (nvcc, clang) and AMD (clang) device back-ends lower to a trap.
**/
//======================================================================================================================
#if defined(__cpp_lib_unreachable)
#define KWK_UNREACHABLE() std::unreachable()
#elif defined(KWK_FRONTEND_MSVC)
#define KWK_UNREACHABLE() __assume(false)
#else
#define KWK_UNREACHABLE() __builtin_unreachable()
#endif

// Enforce EBO on MSVC : https://learn.microsoft.com/en-us/cpp/cpp/empty-bases?view=msvc-170
#if defined(KWK_FRONTEND_MSVC)
#define KWK_STRUCT_ABI __declspec(empty_bases)
#else
#define KWK_STRUCT_ABI
#endif

// Unused at this point, to remove ??
// CONST attribute
#if defined(__GNUC__) || defined(__clang__)
#define KWK_CONST [[gnu::const]]
#elif defined(_MSC_VER)
#define KWK_CONST __declspec(noalias)
#else
#define KWK_CONST
#endif

// PURE attribute
#if defined(__GNUC__) || defined(__clang__)
#define KWK_PURE [[gnu::pure]]
#elif defined(_MSC_VER)
#define KWK_PURE __declspec(noalias)
#else
#define KWK_PURE
#endif
