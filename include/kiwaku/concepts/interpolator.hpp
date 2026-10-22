//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kiwaku/detail/detail.hpp>

namespace kwk::__
{
  // A strategy "interpolates" once it provides both a down (sample request) and an up (blend)
  // stage. Selection strategies (`none`/`nearest`) expose neither and exact-sample one element.
  template<typename T> inline constexpr bool has_down_up_v = requires { T::_is_interpolating; };

  // A strategy can consume a fractional position when it either interpolates (blends, `linear`) or
  // rounds the position onto integer grid points (`select` : `nearest`/`floor`/`ceil`/`round`).
  // detection uses the same marker-constant trick as `has_down_up_v` (`_is_interpolating`), since a
  // static member function template cannot be named in a `requires`-expression.
  // `none` does neither, so it only accepts integral indices.
  template<typename T>
  inline constexpr bool fractional_capable_v
    = requires { T::_is_interpolating; } || requires { T::_selects; };
}
