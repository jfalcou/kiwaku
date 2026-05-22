//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

namespace kwk::__
{
  template<typename T> struct has_trivial_ctor
  {
    static constexpr bool value =
      (std::is_trivially_default_constructible_v<T> || std::is_trivially_copy_constructible_v<T> ||
       std::is_trivially_move_constructible_v<T>);
  };

  template<typename T> struct is_implicit_lifetime
  {
    static constexpr bool value =
      std::is_scalar_v<T> || std::is_array_v<T> ||
      (std::is_class_v<T>
         ? (std::is_aggregate_v<T> || (std::is_trivially_destructible_v<T> && has_trivial_ctor<T>::value))
         : false);
  };

  template<typename T> inline constexpr bool is_implicit_lifetime_v = is_implicit_lifetime<T>::value;

  template<typename T>
  concept implicit_lifetime_type = is_implicit_lifetime_v<std::remove_cvref_t<T>>;

  template<typename T>
  requires(implicit_lifetime_type<T>)
  KWK_TRIVIAL T* start_lifetime_as(void* p) noexcept
  {
    auto q = reinterpret_cast<T*>(p);
    std::memmove(q, p, sizeof(T));
    return q;
  }

  template<typename T>
  requires(implicit_lifetime_type<T>)
  KWK_TRIVIAL const T* start_lifetime_as(void const* p) noexcept
  {
    auto q = reinterpret_cast<T const*>(p);
    auto r = reinterpret_cast<T*>(const_cast<void*>(p));
    std::memmove(r, p, sizeof(T));
    return q;
  }

  template<typename T, typename... Args> constexpr T* construct_at(T* location, Args&&... args)
  {
    return ::new (location) T(KWK_FWD(args)...);
  }

  template<typename T> constexpr void destroy_at(T* p)
  {
    p->~T();
  }
}
