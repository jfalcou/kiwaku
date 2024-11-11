//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kwk/detail/assert.hpp>
#include <kwk/concepts/stream.hpp>
#include <concepts>
#include <cstdint>
#include <limits>

namespace kwk
{
  namespace _
  {
    template<auto N> consteval auto from_size()
    {
      if      constexpr(N >= (1ULL<<32))  return std::uint64_t{};
      else if constexpr(N >= (1ULL<<16))  return std::uint32_t{};
      else if constexpr(N >= (1ULL<<8 ))  return std::uint16_t{};
      else                                return std::uint8_t {};
    }

    template<std::integral T> consteval auto from_type()
    {
      if      constexpr(std::unsigned_integral<T>) return T{};
      else if constexpr(std::signed_integral<T>  )
      {
        if      constexpr(sizeof(T) == 1) return std::uint8_t {};
        else if constexpr(sizeof(T) == 2) return std::uint16_t{};
        else if constexpr(sizeof(T) == 4) return std::uint32_t{};
        else                              return std::uint64_t{};
      }
    }
  }

  //====================================================================================================================
  //! @ingroup utility
  //! @brief Type representing a number of elements.
  //!
  //! kwk::size_type stores an unsigned integral in order to represent a number of element that can be
  //! safely used in comparisons and arithmetic contexts.
  //!
  //! @tparam T Internal std::unsigned_integral type used for storage
  //====================================================================================================================
  template<std::unsigned_integral T>
  struct size_type
  {
    using kwk_size_type  = void
;
    /// @brief Size value storage type.
    using value_type     = T;

    /// @brief Maximum representable value
    static constexpr auto max() noexcept { return std::numeric_limits<T>::max(); }

    //! @brief Constructs a kwk::size_type of value 0
    constexpr size_type() : value_(T{0}) {}

    //! @brief Constructs a kwk::size_type from an integral value compatible with the current upper bound.
    //!
    //! If `v` is greater than the current upper bound, the behavior is undefined.
    //! @param v Integral value to store
    template<std::integral U>
    explicit constexpr size_type(U v)
    {
      KWK_ASSERT(static_cast<std::size_t>(v) <= max(), "Size ", +v, " is out of [0," ,+max(), "]" );
      value_ = static_cast<T>(v);
    }

    //! @brief Constructs a kwk::size_type from another one with a compatible upper bound.
    //!
    //! Does not participate in overload resolution if `std::numeric_limits<U>::max() > max()`.
    //! @param other kwk::size_type to copy
    template<std::unsigned_integral U>
    requires(max() >= std::numeric_limits<U>::max())
    constexpr size_type(size_type<U> other) : size_type(other.value())
    {}

    //! @brief Assign an kwk::size_type value within the current upper bound to another one.
    //!
    //! Does not participate in overload resolution if `std::numeric_limits<U>::max() > max()`.
    //! @param v kwk::size_type value to assign.
    template<std::unsigned_integral U>
    requires(max() >= std::numeric_limits<U>::max())
    constexpr size_type& operator=(size_type<U> other)
    {
      value_ = other.value();
      return *this;
    }

    //! @brief Assign an integral value within the current upper bound to a kwk::size_type.
    //!
    //! If `v` is greater than the current upper bound, the behavior is undefined.
    //! @param v Integral value to assign.
    constexpr size_type& operator=(std::integral auto v)
    {
      KWK_ASSERT(static_cast<std::size_t>(v) <= max(), "Size ", +v, " is out of [0,", +max(), "]");
      value_ = static_cast<T>(v);
      return *this;
    }

    /// @brief Equality comparison between kwk::size_type
    template<typename U>
    friend constexpr auto operator==(size_type a, size_type<U> b) noexcept
    {
      return a.value() == b.value();
    }

    /// @brief Comparions between kwk::size_type
    template<typename U>
    friend constexpr auto operator<=>(size_type a, size_type<U> b) noexcept
    {
      return a.value() <=> b.value();
    }

    constexpr auto operator+() const noexcept
    {
      // Promote to the closest bigger type
      if constexpr(sizeof(T) < 8) return size_type<decltype(_::from_size<std::numeric_limits<T>::max()*2ULL>())>{value_};
      else                        return *this;
    }

    friend constexpr auto operator+(size_type<T> a, size_type<T> b) noexcept
    {
      return size_type<T>{a.value() + b.value()};
    }

    /// @brief Stream insertion operator
    template<concepts::output_stream Stream>
    friend decltype(auto) operator<<(Stream& os, size_type s) { return os << +s.value_; }

    /// @brief Access to the stored size value
    constexpr auto value() const noexcept { return value_; }

    private:
    T value_;
  };

  /// @brief Deduction guide for kwk::size_type
  /// @related kwk::size_type
  template<std::integral T>
  size_type(T v) -> size_type<decltype(_::from_type<T>())>;

  /// @brief Type alias for 64 bits kwk::size_type.
  /// @related kwk::size_type
  using size64_t        = size_type<std::uint64_t>;

  /// @brief Type alias for 32 bits kwk::size_type.
  /// @related kwk::size_type
  using size32_t        = size_type<std::uint32_t>;

  /// @brief Type alias for 16 bits kwk::size_type.
  /// @related kwk::size_type
  using size16_t        = size_type<std::uint16_t>;

  /// @brief Type alias for 8 bits kwk::size_type.
  /// @related kwk::size_type
  using size8_t         = size_type<std::uint8_t>;

  /// @brief Type alias for the default kwk::size_type type.
  /// @related kwk::size_type
  using size_t          = size64_t;

  //! @brief Type alias for a kwk::size_type type with a given upper-bound
  /// @related kwk::size_type
  template<std::size_t MaxValue>
  using size_type_for = size_type<decltype(_::from_size<MaxValue>())>;

  //! @brief Type alias for a kwk::size_type type compatible with a given integral type
  /// @related kwk::size_type
  template<std::integral T>
  using size_type_from = size_type<decltype(_::from_type<T>())>;
}
