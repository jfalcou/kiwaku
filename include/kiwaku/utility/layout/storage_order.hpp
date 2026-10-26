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
    @ingroup layout-utility
    @brief Storage Order descriptor

    A compile-time structural object holding a function pointer to a dimension mapping rule.
    It is dimension-agnostic and evaluates the permutation when combined with a shape to produce a
  stride.
  **/
  //====================================================================================================================
  struct storage_order_descriptor
  {
    using generator_t                    = int (*)(int, int);
    generator_t generator                = nullptr;

    consteval storage_order_descriptor() = default;

    /**
      @brief Construct a storage order from a stateless lambda.
      @param f A stateless function taking (index, ndim) and returning the dimension mapping.
    **/
    template<typename Func>
      requires(std::is_empty_v<Func>)
    consteval storage_order_descriptor(Func f)
        : generator(static_cast<generator_t>(f))
    {
    }

    /**
      @brief Functional interface

      Computes the order of physical storage for a given dimension.

      @param i    Dimension index
      @param ndim Number of dimensions
      @return The dimension index where to find the actual size.
    **/
    constexpr int  operator()(int i, int ndim) const { return generator(i, ndim); }

    constexpr bool operator==(storage_order_descriptor const&) const noexcept = default;
  };

  // Canonical dimension-mapping descriptors. Both the `row_major_order` / `column_major_order`
  // presets and the `<<` printer below are derived from these *same* constants, so their function
  // pointers compare equal by construction (two behaviourally-identical but separately-written
  // stateless lambdas would NOT have equal converted function pointers, so the printer compares
  // against these shared values rather than re-declaring lambdas).
  inline constexpr storage_order_descriptor __row_major_desc    = [](int i, int) { return i; };
  inline constexpr storage_order_descriptor __column_major_desc = [](int i, int n)
  { return n - 1 - i; };

  //====================================================================================================================
  /**
    @brief Wrapper for compile-time storage order definition
  **/
  //====================================================================================================================
  template<storage_order_descriptor O> struct storage_order_option
  {
    //==================================================================================================================
    // Shape is a field over itself
    //==================================================================================================================
    using element_type    = storage_order_option;
    using type            = storage_order_option;
    using identifier_type = __::storage_order_id;
    using label_type      = kumi::str;

    template<typename Self> constexpr auto&& operator()(this Self&& self, identifier_type const&)
    {
      using storage_order_type = std::conditional_t<std::is_const_v<std::remove_reference_t<Self>>,
                                                    storage_order_option const,
                                                    storage_order_option>;
      return std::forward_like<Self>(static_cast<storage_order_type&>(self));
    }

    static constexpr label_type               label() { return kumi::str {"Storage Order"}; }

    static constexpr storage_order_descriptor order = O;

    consteval operator storage_order_descriptor() const { return O; }

    consteval storage_order_option() {}

    consteval storage_order_option(auto) {}

    template<storage_order_descriptor P>
    constexpr bool operator==(storage_order_option<P> const&) const noexcept
    {
      return O == P;
    }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                         storage_order_option const&) noexcept
    {
      if constexpr(O == __row_major_desc) return os << "Storage Order : Row Major";
      else if constexpr(O == __column_major_desc) return os << "Storage Order : Column Major";
      else return os << "Storage Order : Custom";
    }
  };

  template<typename SO> storage_order_option(SO) -> storage_order_option<SO {}>;

  /// @brief Row-major storage order pre-define descriptor
  inline constexpr storage_order_option<__row_major_desc>    row_major_order {};

  /// @brief Column-major storage order pre-define descriptor
  inline constexpr storage_order_option<__column_major_desc> column_major_order {};
}
