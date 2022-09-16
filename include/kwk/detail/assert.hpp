//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : EVE Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#if defined(KIWAKU_ASSERT)

// user/predefined

#elif defined(KIWAKU_DISABLE_ASSERTS) || defined(NDEBUG)

#define KIWAKU_ASSERT(cond, ...) ((void)0)
#define KIWAKU_VERIFY(cond, ...) ((void)(cond))

#elif defined(BOOST_ENABLE_ASSERT_HANDLER)

#include <boost/assert.hpp>

#define KIWAKU_ASSERT   BOOST_ASSERT_MSG
#define KIWAKU_VERIFY   BOOST_VERIFY_MSG

#else

#include <cstdlib>
#include <iostream>

#define KIWAKU_ASSERT(cond, ...)                                                                    \
  do                                                                                                \
  {                                                                                                 \
    if(!(cond))                                                                                     \
    {                                                                                               \
      std::cerr << "[KIWAKU] Assertion '" << #cond << "' failed in "                                \
                << __FILE__ << ":" << __LINE__                                                      \
                << " - " << __VA_ARGS__ << std::endl;                                               \
      std::abort();                                                                                 \
    }                                                                                               \
  } while(0) /**/

#define KIWAKU_VERIFY(cond, ...) KIWAKU_ASSERT(cond, __VA_ARGS__)

#endif
