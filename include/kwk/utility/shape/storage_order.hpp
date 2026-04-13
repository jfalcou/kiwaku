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
  /**
    @ingroup shape-utility
    @brief Storage Order descriptor

    A compile-time structural object holding a function pointer to a dimension mapping rule.
    It is dimension-agnostic and evaluates the permutation when combined with a shape to produce a stride.
  **/
  //====================================================================================================================
  struct storage_order_descriptor
  {
    using generator_t = int (*)(int, int);
    generator_t generator = nullptr;

    consteval storage_order_descriptor() = default;

    /**
      @brief Construct a storage order from a stateless lambda.
      @param f A staetless function taking (index, ndim) and returning the dimension mapping.
    **/
    template<typename Func>
    requires(std::is_empty_v<Func>)
    consteval storage_order_descriptor(Func f) : generator(static_cast<generator_t>(f))
    {
    }

    /**
      @brief Functional interface

      Computes the order of physical storage for a given dimension.

      @param i    Dimension index
      @param ndim Number of dimensions
      @return The dimension index where to find the actual size.
    **/
    constexpr int operator()(int i, int ndim) const { return generator(i, ndim); }

    constexpr bool operator==(storage_order_descriptor const&) const noexcept = default;
  };

  //====================================================================================================================
  /**
    @brief Wrapper for compile-time storage order definition
  **/
  //====================================================================================================================
  template<storage_order_descriptor O> struct storage_order_t
  {
    static constexpr storage_order_descriptor descriptor = O;

    consteval operator storage_order_descriptor() const { return O; }

    constexpr storage_order_t() {}

    constexpr storage_order_t(auto) {}

    template<storage_order_descriptor P> constexpr bool operator==(storage_order_t<P> const&) const noexcept
    {
      return O == P;
    }
  };

  template<typename SO> storage_order_t(SO) -> storage_order_t<SO{}>;

  /// @brief Row-major storage order pre-define descriptor
  inline constexpr storage_order_t row_major_order = [](int i, int) { return i; };

  /// @brief Column-major storage order pre-define descriptor
  inline constexpr storage_order_t column_major_order = [](int i, int n) { return n - 1 - i; };
}
