//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <kwk/detail/abi.hpp>
#include <kwk/container/settings/size.hpp>
#include <kwk/concepts/container.hpp>
#include <kwk/container/view.hpp>

namespace kwk
{
  constexpr auto reshape(concepts::container auto&& c, auto&& sz)
  requires requires { kwk::size = KWK_FWD(sz); }
  {
    return view{ rbr::merge( rbr::settings(size = KWK_FWD(sz)), KWK_FWD(c).settings()) };
  }
}
