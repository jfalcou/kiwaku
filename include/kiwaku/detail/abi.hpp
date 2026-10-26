//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

// Ensure correct C++ standard version
#if defined(_MSC_VER)
#if _MSVC_LANG < 202302L
#error "Kiwaku C++ version error"
#include "Kiwaku requires C++23 or higher. Use /std:c++23 or higher to enable C++23 features."
#endif
#else
#if __cplusplus < 202302L
#error "Kiwaku C++ version error"
#include "Kiwaku requires C++23 or higher. Use -std=c++23 or higher to enable C++23 features."
#endif
#endif

// Faster than std::forward
#define KWK_FWD(...) static_cast<decltype(__VA_ARGS__)&&>(__VA_ARGS__)

// Force empty base optimiation (MSVC related)
#if defined(_MSC_VER)
#define KWK_STRUCT_ABI __declspec(empty_bases)
#else
#define KWK_STRUCT_ABI
#endif

// Allow an empty data member to share storage (C++20 [[no_unique_address]]) so that static
// "parameters" such as a static layout, an empty scheme or an empty accessor do not inflate the
// footprint of a container. NOTE: removed - not portable (MSVC). The collection now reaches the
// same result portably via the `properties` internal EBO base that inherits these components.
// #define KWK_NO_UNIQUE_ADDRESS [[no_unique_address]]

// Clang-CL does not define __GNUC__ https://github.com/llvm/llvm-project/issues/53259
#if defined(KWK_DEBUG)
#define KWK_ABI
#elif defined(__EDG__) || defined(__EDG_VERSION__) || defined(__CUDACC__) || defined(__NVCC__)
#define KWK_ABI inline
#elif defined(__GNUC__) || defined(__clang__)
#define KWK_ABI [[using gnu: always_inline, flatten, artificial]] inline
#elif defined(_MSC_VER)
#define KWK_ABI [[using msvc: forceinline, flatten]] inline
#else
#define KWK_ABI inline
#endif

// Functions in namespace detail should not be forceinline and have a different abi
#define KWK_HIDDEN_ABI inline

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

// (std::)unreachable
#if defined(__GNUC__)
#define KWK_UNREACHABLE() __builtin_unreachable()
#elif defined(_MSC_VER)
#define KWK_UNREACHABLE() __assume(false)
#else
#define KWK_UNREACHABLE() std::unreachable()
#endif
