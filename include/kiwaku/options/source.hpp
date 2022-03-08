//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kiwaku/detail/raberu.hpp>
#include <kiwaku/options/source/ptr_source.hpp>
#include <kiwaku/options/source/range_source.hpp>
#include <kiwaku/concept/range.hpp>

namespace kwk
{
#if !defined(KWK_USE_DOXYGEN)
  struct data_source : rbr::any_keyword<data_source>
  {
    // Raw pointers
    template<typename Ptr> constexpr auto operator=(Ptr* p) const noexcept
    {
      return ptr_source{p};
    }

    // ContiguousRange with .data()
    template<kwk::concepts::contiguous_range R> constexpr auto operator=(R&& r) const noexcept
    {
      return range_source{r.data(), r.end() - r.begin()};
    }

    // Display
    template<typename Src> std::ostream& show(std::ostream& os, Src src) const
    {
      return os << "Source: " << src.as_span().get()
                              << " (" << rbr::detail::type_name<Src>() << ") "
                              << " - shape: " << src.default_shape();
    }
  };
#endif

  /**
    @ingroup  options
    @brief    Data source for kwk::view
   **/
  inline constexpr data_source source = {};
}
