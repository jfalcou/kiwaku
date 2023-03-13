//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

// Faster than std::forward
#define KWK_FWD(...) static_cast<decltype(__VA_ARGS__) &&>(__VA_ARGS__)

// Force a function to be inline
#if defined(KWK_NO_FORCEINLINE)
#  define KWK_TRIVIAL            inline
#  define KWK_FORCEINLINE        inline
#  define KWK_LAMBDA_FORCEINLINE
#else
#  if defined(__GNUC__) && (__GNUC__ > 3) || defined(__clang__) // Clang-CL does not define __GNUC__ https://github.com/llvm/llvm-project/issues/53259
#    define KWK_FORCEINLINE inline __attribute__((__always_inline__))
#    define KWK_LAMBDA_FORCEINLINE __attribute__((__always_inline__))
#    define KUMI_TRIVIAL [[using gnu: always_inline, flatten, artificial]] inline
#  elif defined(_MSC_VER)
#    define KWK_FORCEINLINE         __forceinline
#    define KWK_TRIVIAL             __forceinline
#    define KWK_LAMBDA_FORCEINLINE [[msvc::forceinline]]
#  else
#    define KWK_TRIVIAL            inline
#    define KWK_FORCEINLINE        inline
#    define KWK_LAMBDA_FORCEINLINE
#  endif
#endif

// CONST attribute
#if defined(__GNUC__) || defined(__clang__)
#  define KWK_CONST [[gnu::const]]
#elif defined(_MSC_VER)
#  define KWK_CONST __declspec(noalias)
#else
#  define KWK_CONST
#endif

// PURE attribute
#if defined(__GNUC__) || defined(__clang__)
#  define KWK_PURE [[gnu::pure]]
#elif defined(_MSC_VER)
#  define KWK_PURE __declspec(noalias)
#else
#  define KWK_PURE
#endif

// (std::)unreachable
#if defined(__GNUC__)
#   define KWK_UNREACHABLE() __builtin_unreachable()
#elif defined(_MSC_VER)
#   define KWK_UNREACHABLE() __assume(false)
#else
#   define KWK_UNREACHABLE() std::unreachable()
#endif
