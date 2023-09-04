//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once
#include <kwk/container/view.hpp>

namespace kwk 
{
auto reverse(kwk::concepts::container auto const& c)
{
  auto nst = kumi::apply([](auto... m) { return kwk::with_strides(-m...); }, c.stride());

  return kwk::view( kwk::source = c.get_data() + c.numel() - 1
                  , c.shape()
                  , nst
                  );
}
}
