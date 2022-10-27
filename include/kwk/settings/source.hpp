//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/detail/abi.hpp>
#include <kwk/detail/raberu.hpp>
#include <kwk/settings/source/ptr_source.hpp>
#include <kwk/settings/source/array_source.hpp>
#include <kwk/settings/source/range_source.hpp>
#include <kwk/concepts/range.hpp>
#include <array>

namespace kwk::detail
{
  struct source_ : rbr::as_keyword<source_>
  {
    // Options passthrough
    constexpr auto operator=(rbr::concepts::option auto const& o) const noexcept { return o; }

    template<kwk::concepts::contiguous_static_range Array>
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
    template<typename Src> std::ostream& display(std::ostream& os, Src src) const
    {
      auto ptr = storage(src);
      return os << "Source: " << ptr << " (" << rbr::detail::type_name<decltype(ptr)>() << ") ";
    }
  };
}

namespace kwk
{
  //================================================================================================
  //! @ingroup  settings
  //! @brief    Data source setting for kwk::table and kwk::view
  //================================================================================================
  inline constexpr detail::source_ source = {};
}
