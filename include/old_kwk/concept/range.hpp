//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/detail/ct_helpers.hpp>
#include <type_traits>
#include <concepts>
#include <iterator>

namespace kwk::concepts
{
  //================================================================================================
  /**
    @brief Range

    Types model kwk::concepts::range if they supports call to the range standard interface.
  **/
  //================================================================================================
  template<typename Range>
  concept range = requires(Range&& r)
  {
    { std::begin(r) };
    { std::end(r)   };
    { std::size(r)  };
  };

  //================================================================================================
  /**
    @brief Contiguous Range

    Types model kwk::concepts::contiguous_range if they supports call to `std::data` along with
    other range standard interface.
  **/
  //================================================================================================
  template<typename Range>
  concept contiguous_range = range<Range> && requires(Range&& r)
  {
    { std::data(r)  };
  };

  //================================================================================================
  /**
    @brief Pointer

    Types model kwk::concepts::pointer if their are an actual pointer type.
    This concept is used to discriminate overload sets that contains both pointers and C-style
    arrays.
  **/
  //================================================================================================
  template<typename T>
  concept pointer = std::is_pointer_v<std::remove_cvref_t<T>>;

  //================================================================================================
  /**
    @brief Static-size capability

    Types model kwk::concepts::has_static_size if they can provides a compile-time size.
  **/
  //================================================================================================
  template<typename T>
  concept has_static_size = detail::static_size<std::remove_cvref_t<T>>::value;

  //================================================================================================
  /**
    @brief Contiguous Static Range
  **/
  //================================================================================================
  template<typename T>
  concept contiguous_static_range = contiguous_range<T> && has_static_size<T>;
}
