//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

namespace kwk
{
  //====================================================================================================================
  // Finds the qualified underlying type of an access to the given range type
  //====================================================================================================================
  template<typename T> struct container_member : kumi::container_type<T>
  {
  };

  template<typename T> struct container_member<T const>
  {
    using type = typename container_member<T>::type const;
  };

  template<typename T> struct container_member<T const&>
  {
    using type = typename container_member<T>::type const;
  };

  template<typename T> using container_member_t = typename container_member<T>::type;

  //====================================================================================================================
  // Computes the shape of any range like object based on the type of it's elements
  //====================================================================================================================
  template<typename T> struct container_shape
  {
    static constexpr auto value = kwk::shape{};//kwk::shape_descriptor{};
  };

  template<kwk::concepts::range T> struct container_shape<T>
  {
    static constexpr auto value = add_rank(container_shape<kumi::container_type_t<T>>::value, kumi::container_size_v<T>);
  };

  template<typename T> inline constexpr auto container_shape_v = container_shape<T>::value;

  template<typename T> constexpr auto* container_base_address(T&& val)
  {
    if constexpr (kumi::concepts::static_container<T>) return container_base_address(KWK_FWD(val)[0]);
    else if constexpr (concepts::range<T>) return std::data(KWK_FWD(val));
    else if constexpr (concepts::pointer<T>) return KWK_FWD(val);
    else return &val;
  }

  template<typename T>
  using container_base_t = std::remove_pointer_t<decltype(container_base_address(std::declval<T>()))>;
}
