//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <kwk/detail/abi.hpp>
#include <kwk/container.hpp>
#include <kwk/container/settings/label.hpp>
#include <kwk/concepts/container.hpp>
#include <kwk/container/view.hpp>

namespace kwk
{
  constexpr auto relabel(concepts::container auto&& c, auto&& l)
  requires requires { kwk::label = KWK_FWD(l); }
  {
    return view{ rbr::merge( rbr::settings(label = KWK_FWD(l)), KWK_FWD(c).settings()) };
  }
}
