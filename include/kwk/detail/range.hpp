//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/detail/kumi.hpp>
#include <kwk/utility/fixed.hpp>
#include <type_traits>
#include <cstddef>
#include <array>

namespace kwk::detail
{
  //================================================================================================
  // Find the static_size of static array like types
  //================================================================================================
  template<typename T>
  struct array_traits : std::false_type
  {
    using value_type            = T;
    static constexpr auto sizes = kumi::tuple{};
    static constexpr auto data(auto&& a) noexcept { return &KWK_FWD(a); }
  };

  template<typename T, std::size_t N>
  struct array_traits<std::array<T, N>> : std::true_type
  {
    using value_type            = typename array_traits<T>::value_type;
    static constexpr auto sizes = kumi::push_back(array_traits<T>::sizes, fixed<clamp<N>()>);
    static constexpr auto data(auto&& a) noexcept { return array_traits<T>::data(KWK_FWD(a)[0]); }
  };

  template<typename T, std::size_t N>
  struct array_traits<T[N]> : std::true_type
  {
    using value_type            = typename array_traits<T>::value_type;
    static constexpr auto sizes = kumi::push_front(array_traits<T>::sizes, fixed<clamp<N>()>);
    static constexpr auto data(auto&& a) noexcept { return array_traits<T>::data(KWK_FWD(a)[0]); }
  };
}
