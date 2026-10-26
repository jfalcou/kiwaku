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
#include <kiwaku/memory/utils.hpp>

namespace kwk
{
  //====================================================================================================================
  //! @brief Copy every cell of `in` into the corresponding cell of `out`.
  //!
  //! Iterates the source's n-dimensional shape and assigns each element across, using the position
  //! operator on both collections (`out[idxs...] = in[idxs...]`).
  //====================================================================================================================
  struct copy_t
  {
    template<kwk::concepts::collection Out, kwk::concepts::collection In>
    constexpr void operator()(Out& o, In const& in) const noexcept
    {
      kwk::__::for_each([ & ](auto... idxs) { o[ idxs... ] = in[ idxs... ]; }, in->shape());
    }
  } inline constexpr copy {};
}
