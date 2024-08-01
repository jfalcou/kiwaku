//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kwk/detail/assert.hpp>
#include <concepts>
#include <cstdint>
#include <limits>

namespace kwk
{
  namespace __
  {
    template<auto N> consteval auto from_size()
    {
      if      constexpr (N>=(1ULL<<32)) return std::uint64_t{};
      else if constexpr (N>=(1ULL<<16)) return std::uint32_t{};
      else if constexpr (N>=(1ULL<<8 )) return std::uint16_t{};
      else                              return std::uint8_t {};
    }

    template<std::integral T> consteval auto from_type()
    {
      if      constexpr(std::unsigned_integral<T>) return T{};
      else if constexpr(std::signed_integral<T>)
      {
        if      constexpr(sizeof(T) == 1) return std::uint8_t{};
        else if constexpr(sizeof(T) == 2) return std::uint16_t{};
        else if constexpr(sizeof(T) == 4) return std::uint32_t{};
        else                              return std::uint64_t{};
      }
    }
  }

  //================================================================================================
  //! @ingroup utility
  //! @brief Type representing a number of elements with a safe API.
  //!
  //! kwk::size_t stores an unsigned integral in order to represent a number of element that can be
  //! safely used in comparisons and arithmetic contexts.
  //!
  //! @tparam T Internal std::unsigned_integral type used for storage
  //================================================================================================
  template<std::unsigned_integral T>
  struct size_t
  {
    using kwk_size_type  = void
;
    /// @brief Size value storage type.
    using value_type     = T;

    /// @brief Maximum representable value
    static constexpr auto max() noexcept { return std::numeric_limits<T>::max(); }

    //! @brief Default constructor
    //!
    //! Constructs a kwk::size_t of value 0
    constexpr size_t() : value_(T{0}) {}

    //! @brief Constructor from integral value
    //!
    //! Constructs a kwk::size_t from an integral value compatible with the current upper bound.
    //!
    //! If `v` is greater than the current upper bound, the behavior is undefined.
    //!
    //! @param v Integral value to store
    explicit constexpr size_t(std::convertible_to<T> auto v)
    {
      KIWAKU_ASSERT(static_cast<T>(v) <= max(), "Size " << +v << " is out of [0," << +max() << "]");
      value_ = static_cast<T>(v);
    }

    //! @brief Conversion constructor
    //!
    //! Constructs a kwk::size_t from another one with a compatible upper bound.
    //! Does not participate in overload resolution if `other.max() > max()`.
    //! @param other kwk::size_t to copy
    template<std::unsigned_integral U>
    constexpr size_t(size_t<U> other) : size_t(other.value())
    {}

    //! @brief Assign a kwk::size_t value to another
    //!
    //! Assign an kwk::size_t value within the current upper bound to another one.
    //! Does not participate in overload resolution if `other.max() > max()`.
    //!
    //! @param v kwk::size_t value to assign.
    template<typename U>
    constexpr size_t& operator=(size_t<U> other)
    {
      value_ = other.value();
      return *this;
    }

    //! @brief Assign an integral value to a kwk::size_t
    //!
    //! Assign an integral value within the current upper bound to a kwk::size_t.
    //!
    //! If `v` is greater than the current upper bound, the behavior is undefined.
    //!
    //! @param v Integral value to assign.
    constexpr size_t& operator=(std::convertible_to<T> auto v)
    {
      KIWAKU_ASSERT(v <= max(), "Size " << +v << " is out of [0," << +max() << "]");
      value_ = static_cast<T>(v);
      return *this;
    }

    /// @brief Equality comparion between kwk::size_t
    template<typename U>
    friend constexpr auto operator==(size_t a, size_t<U> b) noexcept
    {
      return a.value() == b.value();
    }

    /// @brief Comparions between kwk::size_t
    template<typename U>
    friend constexpr auto operator<=>(size_t a, size_t<U> b) noexcept
    {
      return a.value() <=> b.value();
    }

    constexpr auto operator+() const noexcept
    {
      // Promote to the closest bigger type
      using type = decltype(__::from_size<std::numeric_limits<T>::max()*2>());
      return size_t<type>{value_};
    }

    friend constexpr auto operator+(size_t<T> a, size_t<T> b) noexcept
    {
      return size_t<T>{a.value()+b.value()};
    }

    /// @brief Stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, size_t s) { return os << +s.value_; }

    /// @brief Access to the stored size value
    constexpr auto value() const noexcept { return value_; }

    //private:
    T value_;
  };

  /// @brief Deduction guide for kwk::size_t
  template<std::integral T>
  size_t(T v) -> size_t<decltype(__::from_type<T>())>;

  /// @brief Type alias for 64 bits kwk::size_t.
  using size64_t        = size_t<std::uint64_t>;
  /// @brief Type alias for 32 bits kwk::size_t.
  using size32_t        = size_t<std::uint32_t>;
  /// @brief Type alias for 16 bits kwk::size_t.
  using size16_t        = size_t<std::uint16_t>;
  /// @brief Type alias for 8 bits kwk::size_t.
  using size8_t         = size_t<std::uint8_t>;
  /// @brief Type alias for the default kwk::size_t type.
  using default_size_t  = size64_t;

  //! @brief Type alias for a kwk::size_t type with a given upper-bound
  //!
  //! Evaluates to an instance of kwk::size_t with the best type able to store value with
  //! the specified upper bound.
  template<std::ptrdiff_t MaxSize>
  using size_for = size_t<decltype(__::from_size<MaxSize>())>;

  //! @brief Type alias for a kwk::size_t type compatible with a given integral type
  template<std::integral T>
  using size_from = size_t<decltype(__::from_type<T>())>;
}
