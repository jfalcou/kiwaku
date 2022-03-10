//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#pragma once

#include <kiwaku/detail/ct_helpers.hpp>
#include <type_traits>
#include <concepts>
#include <iterator>

namespace kwk::concepts
{
  template<typename Range>
  concept contiguous_range = requires(Range&& r)
  {
    { std::begin(r) };
    { std::end(r)   };
    { std::size(r)  };
    { std::data(r)  };
  };

  template<typename T>
  concept pointer = std::is_pointer_v<std::remove_cvref_t<T>>;

  template<typename T>
  concept has_static_size = detail::static_size<std::remove_cvref_t<T>>::value;

  template<typename T>
  concept contiguous_static_range = contiguous_range<T> && has_static_size<T>;
}
