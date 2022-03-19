//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kwk/detail/raberu.hpp>
#include <kwk/options/source/ptr_source.hpp>
#include <kwk/options/source/array_source.hpp>
#include <kwk/options/source/range_source.hpp>
#include <kwk/concept/range.hpp>
#include <array>

namespace kwk
{
#if !defined(KWK_USE_DOXYGEN)
  struct data_source : rbr::as_keyword<data_source>
  {
    // Options passthrough
    constexpr auto operator=(rbr::concepts::option auto const& o) const noexcept { return o; }

    template<concepts::contiguous_static_range Array>
    constexpr auto operator=( Array&& a) const noexcept
    {
      using a_t = std::remove_reference_t<Array>;
      return array_source < detail::value_type_of<a_t>
                          , detail::static_size_v<a_t>
                          >{std::data(KWK_FWD(a))};
    }

    // ContiguousRange with .data()
    constexpr auto operator=(concepts::contiguous_range auto&& r) const noexcept
    {
      return range_source{std::data(KWK_FWD(r)), std::size(KWK_FWD(r))};
    }

    // Raw pointers
    constexpr auto operator=(concepts::pointer auto&& p) const noexcept
    {
      return ptr_source{KWK_FWD(p)};
    }

    // Display
    template<typename Src> std::ostream& show(std::ostream& os, Src src) const
    {
      return os << "Source: " << src.as_span().data()
                              << " (" << rbr::detail::type_name<Src>() << ") "
                              << " - shape: " << src.default_shape();
    }
  };
#endif

  /**
    @ingroup  options
    @brief    Data source setting for kwk::view
  **/
  inline constexpr data_source source = {};
}
