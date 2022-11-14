//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/detail/raberu.hpp>
#include <kwk/concepts/allocator.hpp>

namespace kwk::detail
{
  struct allocator_ : rbr::as_keyword<allocator_>
  {
    template<typename T>
    static constexpr bool check() { return concepts::allocator<std::remove_cvref_t<T>>; }

    using rbr::as_keyword<allocator_>::operator=;

    template<typename A> std::ostream& show(std::ostream& os, A) const
    {
      return os << "Allocation via: " << rbr::detail::type_name<A>();
    }
  };
}

namespace kwk
{
  //================================================================================================
  //! @ingroup  settings
  //! @brief    Allocator setting for kwk::table
  //================================================================================================
  inline constexpr detail::allocator_ allocator = {};
}
