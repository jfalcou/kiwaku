//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kiwaku/concepts/range.hpp>
#include <kiwaku/settings/generator.hpp>
#include <kiwaku/detail/abi.hpp>

namespace kwk
{
  namespace __
  {
    // A generator source consumes coordinates and returns values on demand; it owns no memory
    // region, so `source_pointer` is deliberately not provided. `shape_of` reports a single
    // dynamic dimension (the builder sizes a generator-backed space through its own options).
    //
    // The `generator_option<C>` class template itself lives in <kiwaku/settings/generator.hpp>
    // (it is the settings-level option produced by `kwk::generator = callable`). This header only
    // contributes the source-layer adapters that let the builder recognize a function-backed
    // source and route it to the `constant_blob` terminal backend.
    template<typename C> constexpr auto shape_of(generator_option<C> const&)
    {
      return kwk::shape {kwk::_};
    }
  }
}
