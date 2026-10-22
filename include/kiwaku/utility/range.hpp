//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kiwaku/utility/layout/shape.hpp>

namespace kwk
{
  namespace __
  {
    // Peels one rank off a raw C-array of *any* dimension. kumi only supports one-dimensional
    // raw arrays (`container_type_t<long[10]>` yields `long`), so multi-dimensional raw arrays
    // must be peeled here. Everything else defers to kumi's container_type.
    template<typename T>            struct container_layer { using type = kumi::container_type_t<T>; };
    template<typename T, std::size_t N> struct container_layer<T[N]>       { using type = T; };
    template<typename T, std::size_t N> struct container_layer<T const[N]> { using type = T const; };
    template<typename T, std::size_t N> struct container_layer<T (&)[N]>   { using type = T; };

    template<typename T> using container_layer_t = typename container_layer<T>::type;

    // Weird requirements : to investigate
    template<typename T> extern container_layer_t<T> member_t;
    template<typename T> extern container_layer_t<T> const member_t<T const>;
    template<typename T> extern container_layer_t<T> const member_t<T const&>;
  }
  //====================================================================================================================
  // Finds the qualified underlying type of an access to the given range type
  //====================================================================================================================
  template<typename T> using container_member_t = decltype(__::member_t<T>);

  template<typename T>
  struct container_member 
  {
    using type = container_member_t<T>;
  };

  //====================================================================================================================
  // Computes the shape of any range like object based on the type of it's elements
  //====================================================================================================================
  template<typename T> struct container_shape
  {
    static constexpr auto value = kwk::shape{};
  };

  template<kwk::concepts::range T> struct container_shape<T>
  {
    static constexpr auto value =
      add_rank(container_shape<__::container_layer_t<T>>::value, kumi::container_size_v<T>);
  };

  template<typename T> inline constexpr auto container_shape_v = container_shape<T>::value;

  template<typename T> constexpr auto* container_base_address(T&& val)
  {
    if constexpr (kumi::concepts::static_container<T>) return container_base_address(KWK_FWD(val)[0]);
    else if constexpr (concepts::range<T>) return kwk::data(KWK_FWD(val));
    else if constexpr (concepts::pointer<T>) return KWK_FWD(val);
    else return &val;
  }

  template<typename T>
  using container_base_t = std::remove_pointer_t<decltype(container_base_address(std::declval<T>()))>;
}
