//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kiwaku/detail/raberu.hpp>

namespace kwk
{
  struct data_source : rbr::any_keyword<data_source>
  {
    // Raw pointers
    template<typename Ptr> auto operator=(Ptr p) const noexcept
    requires(std::is_pointer_v<Ptr>)
    {
      return rbr::option<data_source,Ptr>{p};
    }

    // ContiguousRange with .data()
    template<typename Range> auto operator=(Range&& r) const noexcept
    requires requires(Range&& r) { r.data(); }
    {
      return rbr::option<data_source,decltype(r.data())>{r.data()};
    }

    // Display
    template<typename Ptr> std::ostream& show(std::ostream& os, Ptr ptr) const
    {
      return os << "Source: " << ptr << " (" << rbr::detail::type_name<Ptr>() << ')';
    }
  };

  inline constexpr data_source source = {};
}
