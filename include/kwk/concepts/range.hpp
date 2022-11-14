//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/detail/range.hpp>
#include <type_traits>
#include <concepts>
#include <iterator>

namespace kwk::concepts
{
  /// Concept for types supporting calls to standard begin/end/size interface.
  template<typename Range>
  concept range = requires(Range&& r)
  {
    { std::begin(r) };
    { std::end(r)   };
    { std::size(r)  };
  };

  /// Concept for Range types providing access to a contiguous block of data
  template<typename Range>
  concept contiguous_range = range<Range> && requires(Range&& r)
  {
    { std::data(r)  };
  };

  /// Concept for types behaving as an actual pointer type.
  template<typename T>
  concept pointer = std::is_pointer_v<std::remove_cvref_t<T>>;

  /// Concept for types exposing a compile-time size value
  template<typename T>
  concept has_static_size = detail::array_traits<std::remove_cvref_t<T>>::value;

  /// Concept for Contiguous Range exposing a compile-time size value
  template<typename T>
  concept contiguous_static_range = contiguous_range<T> && has_static_size<T>;
}
