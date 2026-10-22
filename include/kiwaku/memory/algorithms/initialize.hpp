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
  template<typename T> constexpr void initialize_(void* p)
  {
    if constexpr (kwk::__::implicit_lifetime_type<T>) kwk::__::start_lifetime_as<T>(p);
    else ::new (p) T;
  }

  struct initialize_t
  {
    template<kwk::concepts::collection InOut> constexpr void operator()(InOut& io) const noexcept
    {
      using T = typename std::remove_cvref_t<InOut>::value_type;

      if constexpr (kumi::concepts::product_type<T>) kumi::for_each(*this, io);
      else kwk::__::for_each([&](auto... idxs) { initialize_<std::remove_const_t<T>>(const_cast<void*>(static_cast<const void*>(&io[idxs...]))); }, io->shape());
    }
  } inline constexpr initialize{};
}
