//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kiwaku/detail/detail.hpp>
#include <kiwaku/concepts.hpp>

namespace kwk
{
  //====================================================================================================================
  //! @brief Fill every cell of a collection with a value.
  //!
  //! Iterates the collection's n-dimensional shape and assigns `value` through the collection's
  //! position operator, so any collection (uniform flat space, tiled/nested space, product space via
  //! tuple-assignment in the acyclic leaf case) is filled uniformly.
  //====================================================================================================================
  struct fill_t
  {
    template<kwk::concepts::collection InOut, typename T>
    constexpr void operator()(InOut& io, T const& value) const noexcept
    {
      kwk::__::for_each([&](auto... idxs) { io[idxs...] = value; }, io->shape());
    }
  } inline constexpr fill{};
}
