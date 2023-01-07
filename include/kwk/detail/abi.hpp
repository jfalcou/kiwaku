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
#  define KWK_FORCEINLINE inline
#else
#  if defined(_MSC_VER)
#    define KWK_FORCEINLINE __forceinline
#  elif defined(__GNUC__) && __GNUC__ > 3
#    define KWK_FORCEINLINE inline __attribute__((__always_inline__))
#  else
#    define KWK_FORCEINLINE inline
#  endif
#endif

// CONST attribute
#if defined(_MSC_VER)
#  define KWK_CONST
#else
#  define KWK_CONST [[gnu::const]]
#endif

// PURE attribute
#if defined(_MSC_VER)
#  define KWK_PURE
#else
#  define KWK_PURE [[gnu::pure]]
#endif
