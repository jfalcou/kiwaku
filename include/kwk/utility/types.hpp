//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kwk/detail/assert.hpp>
#include <cstdint>
#include <limits>

namespace kwk
{
  template<std::signed_integral T, T MaxValue = std::numeric_limits<T>::max()>
  struct size_t
  {
    using value_type        = T;
    using kwk_size_type  = void;

    static constexpr auto max() { return MaxValue; }

    constexpr size_t() : value_(T{}) {}

    template<typename U, U Max>
    constexpr size_t(size_t<U,Max> other) requires(Max<=max())
              : value_(static_cast<T>(other.value()))
    {}

    explicit constexpr size_t(std::convertible_to<T> auto v) : value_(static_cast<T>(v))
    {
      KIWAKU_ASSERT ( value_ >= T{0}  , "size_t " << +value_ << " is not positive.");
      KIWAKU_ASSERT ( value_ <= max() , "size_t " << +value_ << " is over its expected maximum size_t: "
                                                  << +max() << "."
                    );
    }

    template<typename U, U Max>
    friend constexpr auto operator<=>(size_t a, size_t<U,Max> b) noexcept
    {
      return a.value() <=> b.value();
    }

    template<typename U, U Max>
    constexpr size_t& operator=(size_t<U,Max> other) requires(Max<=MaxValue)
    {
      value_ = other.value();
      return *this;
    }

    constexpr size_t& operator=(std::convertible_to<T> auto other)
    {
      KIWAKU_ASSERT ( other >= T{0} , "size_t " << +other << " is not positive.");
      KIWAKU_ASSERT ( other <= max(), "size_t " << +other << " is over its expected maximum size_t: "
                                                << +max() << "."
                    );

      value_ = static_cast<T>(other);
      return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, size_t s) { return os << s.value_; }

    constexpr auto value() const noexcept { return value_; }

    private:
    T value_;
  };

  namespace __
  {
    template<auto N> consteval auto from_size()
    {
      if      constexpr (N>=(1LL<<31))  return size_t<std::int64_t,std::int64_t(N)>{};
      else if constexpr (N>=(1LL<<15))  return size_t<std::int32_t,std::int32_t(N)>{};
      else if constexpr (N>=(1LL<<7 ))  return size_t<std::int16_t,std::int16_t(N)>{};
      else                              return size_t<std::int8_t ,std::int8_t (N)>{};
    }

    template<std::integral T> consteval auto from_type()
    {
      if      constexpr(std::signed_integral<T>  ) return T{};
      else if constexpr(std::unsigned_integral<T>)
      {
        if      constexpr(sizeof(T) == 1) return std::int16_t{};
        else if constexpr(sizeof(T) == 2) return std::int32_t{};
        else                              return std::int64_t{};
      }
    }
  }

  template<std::integral T>
  size_t(T v) -> size_t<decltype(__::from_type<T>())>;

  using size64        = size_t<std::int64_t>;
  using size32        = size_t<std::int32_t>;
  using size16        = size_t<std::int16_t>;
  using size8         = size_t<std::int8_t>;
  using default_size  = size64;

  template<std::ptrdiff_t MaxSize>
  using size_for = decltype(__::from_size<MaxSize>());

  template<typename T>
  using size_from = size_t<decltype(__::from_type<T>())>;
}
