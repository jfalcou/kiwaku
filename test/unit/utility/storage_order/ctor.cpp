//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/kwk.hpp>

TTS_CASE("Storage order construction")
{
  using kwk::storage_order;

  auto so1 = (storage_order = [](auto i, auto){ return i; });
  
  TTS_EQUAL(sizeof(so1), 1UL);
};
