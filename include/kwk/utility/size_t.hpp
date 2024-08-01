//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <cstdarg>
#include <kwk/detail/assert.hpp>
#include <cstdint>
#include <limits>

namespace kwk
{
  //================================================================================================
  //! @ingroup utility
  //! @brief Type representing a number of elements with a safe API.
  //!
  //! kwk::size_t stores signed integral in order to represent a number of element that can be safely
  //! used in comparisons and arithmetic contexts.
  //!
  //! kwk::size_t can also be defined with a custom upper bound. When constructing or assigning a value
  //! to a kwk::size_t instance, the value assigned must be less or equal to this upper bound or the
  //! behavior is undefined.
  //!
  //! @tparam T Internal std::signed_integral type used for storage
  //! @tparam MaxValue Positive upper bound for kwk::size_t value.
  //================================================================================================
  template<std::signed_integral T, T MaxValue = std::numeric_limits<T>::max()>
  struct size_t
  {
    using kwk_size_type  = void
;
    /// @brief Size value storage type.
    using value_type     = T;

    /// @brief Upper bound for current size.
    static constexpr auto max() { return MaxValue; }

    //! @brief Default constructor
    //!
    //! Constructs a kwk::size_t of value 0
    constexpr size_t() : value_(T{0}) {}

    //! @brief Conversion constructor
    //!
    //! Constructs a kwk::size_t from another one with a compatible upper bound.
    //! Does not participate in overload resolution if `other.max() > max()`.
    //! @param other kwk::size_t to copy
    template<typename U, U Max>
    constexpr size_t(size_t<U,Max> other) requires(Max <= max())
              : value_(static_cast<T>(other.value()))
    {}

    //! @brief Constructor from integral value
    //!
    //! Constructs a kwk::size_t from an integral value compatible with the current upper bound.
    //!
    //! If `v` is negative or if `v` is greater than the current upper bound, the behavior is undefined.
    //!
    //! @param v Integral value to store
    explicit constexpr size_t(std::convertible_to<T> auto v) : value_(static_cast<T>(v))
    {
      KIWAKU_ASSERT ( value_ >= T{0}  , "Size " << +value_ << " is not positive.");
      KIWAKU_ASSERT ( value_ <= max() , "Size " << +value_ << " is over its expected maximum size: "
                                                  << +max() << "."
                    );
    }

    /// @brief Compare the value fo two kwk::size_t instance
    friend constexpr auto operator<=>(size_t a, size_t b) noexcept =default;

    /// @overload
    template<typename U, U Max>
    friend constexpr auto operator<=>(size_t a, size_t<U,Max> b) noexcept
    {
      return a.value() <=> b.value();
    }

    //! @brief Assign a kwk::size_t value to another
    //!
    //! Assign an kwk::size_t value within the current upper bound to another one.
    //! Does not participate in overload resolution if `other.max() > max()`.
    //!
    //! @param v kwk::size_t value to assign.
    template<typename U, U Max>
    constexpr size_t& operator=(size_t<U,Max> other) requires(Max <= max())
    {
      value_ = other.value();
      return *this;
    }

    //! @brief Assign an integral value to a kwk::size_t
    //!
    //! Assign an integral value within the current upper bound to a kwk::size_t.
    //!
    //! If `v` is negative or if `v` is greater than the current upper bound, the behavior is undefined.
    //!
    //! @param v Integral value to assign.
    constexpr size_t& operator=(std::convertible_to<T> auto v)
    {
      KIWAKU_ASSERT ( v >= T{0} , "Size " << +v << " is not positive.");
      KIWAKU_ASSERT ( v <= max(), "Size " << +va_end(ap) << " is over its expected maximum size: "
                                                << +max() << "."
                    );

      value_ = static_cast<T>(other);
      return *this;
    }

    /// @brief Stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, size_t s) { return os << s.value_; }

    /// @brief Access to the stored size value
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

  /// @brief Deduction guide for kwk::size_t
  template<std::integral T>
  size_t(T v) -> size_t<decltype(__::from_type<T>())>;

  /// @brief Type alias for 64 bits kwk::size_t.
  using size64_t        = size_t<std::int64_t>;
  /// @brief Type alias for 32 bits kwk::size_t.
  using size32_t        = size_t<std::int32_t>;
  /// @brief Type alias for 16 bits kwk::size_t.
  using size16_t        = size_t<std::int16_t>;
  /// @brief Type alias for 8 bits kwk::size_t.
  using size8_t         = size_t<std::int8_t>;
  /// @brief Type alias for the default kwk::size_t type.
  using default_size_t  = size64_t;

  //! @brief Type alias for a kwk::size_t type with a given upper-bound
  //!
  //! Evaluates to an instance of kwk::size_t with the best type able to store value with
  //! the specified upper bound.
  template<std::ptrdiff_t MaxSize>
  using size_for = decltype(__::from_size<MaxSize>());

  //! @brief Type alias for a kwk::size_t type compatible with a given integral type
  template<std::integral T>
  using size_from = size_t<decltype(__::from_type<T>())>;
}
