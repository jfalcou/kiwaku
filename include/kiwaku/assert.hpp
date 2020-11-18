//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#undef KIWAKU_ASSERT
#undef KIWAKU_VERIFY

#if defined(KIWAKU_DISABLE_ASSERTS) || defined(NDEBUG)

#define KIWAKU_ASSERT(cond, ...) ((void)0)
#define KIWAKU_VERIFY(cond, ...) ((void)(cond))

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
