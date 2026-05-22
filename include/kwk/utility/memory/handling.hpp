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
  requires(std::is_implicit_lifetime_v<T>)
  KWK_TRIVIAL T* start_lifetime_as(void* p) noexcept
  {
    auto q = reinterpret_cast<T*>(p);
    std::memmove(q, p, sizeof(T));
    return q;
  }

  template<typename T>
  requires(std::is_implicit_lifetime_v<T>)
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
