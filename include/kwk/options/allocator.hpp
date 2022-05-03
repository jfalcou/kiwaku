//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kwk/detail/raberu.hpp>
#include <kwk/concept/allocator.hpp>

namespace kwk
{
#if !defined(KWK_USE_DOXYGEN)
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
#endif

  /**
    @ingroup  options
    @brief    Allocator setting for kwk::table
  **/
  inline constexpr allocator_ allocator = {};
}
