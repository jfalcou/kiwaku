//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kwk/detail/raberu.hpp>
#include <kwk/detail/kumi.hpp>
#include <kwk/detail/ct_helpers.hpp>
#include <kwk/options/fixed.hpp>

namespace kwk
{
#if !defined(KWK_USE_DOXYGEN)
  struct base_index_;

  template<std::ptrdiff_t Base> struct bases_
  {
    using stored_value_type = bases_<Base>;
    using keyword_type      = base_index_;

    constexpr auto operator()(keyword_type const&) const noexcept { return *this; }

    template<std::size_t Order> constexpr auto as_position() const noexcept
    {
      return kumi::generate<Order>(Base);
    }
  };

  struct base_index_ : rbr::as_keyword<base_index_>
  {
    template<std::ptrdiff_t Base>
    constexpr auto operator=(bases_<Base> const& s) const noexcept { return s; }

    // Display
    template<std::ptrdiff_t N>
    std::ostream& show(std::ostream& os, bases_<N> const&) const
    {
      return os << "Base Index: " << N;
    }
  };
#endif

  /**
    @ingroup  options
    @brief    Keyword for accessing base index options
  **/
  inline constexpr auto base_index = base_index_{};

  /**
    @ingroup  options
    @brief    Settings describing the base index for all dimensions of a container
  **/
  template<std::ptrdiff_t N>    inline constexpr auto bases   = bases_<N>{};
}
