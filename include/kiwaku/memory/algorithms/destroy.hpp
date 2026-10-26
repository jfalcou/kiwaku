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
#include <kiwaku/memory/algorithms/initialize.hpp>

namespace kwk
{
  struct destroy_t
  {
    template<kwk::concepts::collection InOut> constexpr void operator()(InOut& io) const noexcept
    {
      using T = typename std::remove_cvref_t<InOut>::value_type;

      if constexpr(kumi::concepts::product_type<T>) kumi::for_each(*this, io);
      else
        kwk::__::for_each(
            [ & ](auto... idxs)
            {
              if constexpr(!kwk::__::implicit_lifetime_type<T>)
                kwk::__::destroy_at(std::addressof(io[ idxs... ]));
            },
            io->shape());
    }
  } inline constexpr destroy {};
}
