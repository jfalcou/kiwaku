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
  template<typename T>
  concept has_trivial_ctor =
      std::is_trivially_default_constructible_v<T> || std::is_trivially_copy_constructible_v<T> ||
      std::is_trivially_move_constructible_v<T>;

  template<typename T>
  concept implicit_lifetime_type =
      std::is_scalar_v<T> || std::is_array_v<T> ||
      (std::is_class_v<T> ? (std::is_aggregate_v<T> ||
                             (std::is_trivially_destructible_v<T> && has_trivial_ctor<T>))
                          : false);

  template<typename T>
    requires(implicit_lifetime_type<T>)
  KWK_ABI T* start_lifetime_as(void* p) noexcept
  {
    auto q = reinterpret_cast<T*>(p);
    std::memmove(q, p, sizeof(T));
    return q;
  }

  template<typename T>
    requires(implicit_lifetime_type<T>)
  KWK_ABI const T* start_lifetime_as(void const* p) noexcept
  {
    auto q = reinterpret_cast<T const*>(p);
    auto r = reinterpret_cast<T*>(const_cast<void*>(p));
    std::memmove(r, p, sizeof(T));
    return q;
  }

  template<typename T, typename... Args> constexpr T* construct_at(T* location, Args&&... args)
  {
    return ::new(location) T(KWK_FWD(args)...);
  }

  template<typename T> constexpr void destroy_at(T* p) { p->~T(); }

  struct realign_t
  {
    constexpr auto operator()(auto acc, kumi::tuple<std::size_t, std::size_t> zip) const noexcept
    {
      auto [ size, align ] = zip;
      return ((acc + align - 1) & ~(align - 1)) + size;
    }
  } inline constexpr realign {};
}
