//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : EVE Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <kwk/detail/abi.hpp>
#include <kwk/utility/container/shape.hpp>
#include <cstddef>

namespace kwk::detail
{
  struct strides_;

  template<auto Strides>
  struct stride_base
  {
    static constexpr auto desc = combo_cast<std::ptrdiff_t>(Strides);
    using type = prefilled<desc>;
  };
}

namespace kwk
{
  //================================================================================================
  //! @ingroup containers
  //! @brief  Fixed order stride with mixed size capability
  //!
  //! <hr/>
  //! **Required header**:
  //! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
  //!  #include<kwk/utility/container/stride.hpp>
  //! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //! <hr/>
  //!
  //! kwk::stride defines and optimally stores a set of integral values representing the strides
  //! used to walk through a given multi-dimensional container or view.
  //!
  //! kwk::stride can be defined in two ways:
  //!
  //! - using the kwk::with_strides function.
  //!
  //!   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
  //!   kwk::stride s1 = kwk::with_strides(kwk::fixed<5>);                   // Order 1 stride with static size
  //!   kwk::stride s2 = kwk::with_strides(n, m);                            // Order 2 stride with dynamic sizes
  //!   kwk::stride s3 = kwk::with_strides(kwk::fixed<2>,kwk::fixed<2>, n);  // Order 3 stride with mixed sizes
  //!   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //!   <br/>
  //!
  //! - defining the layout of the kwk::stride and manually initializing it. The description of the
  //!   kwk::stride layout is done with the kwk::extent object or one of the pre-defined layouts.
  //!
  //!   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
  //!   kwk::stride< kwk::extent[5] >      s1;             // Order 1 stride with static size
  //!   kwk::stride< kwk::_2D >            s2(n, m);       // Order 2 stride with dynamic sizes
  //!   kwk::stride< kwk::extent[1]( )[3]> s4( _, n, _);   // Order 3 stride with mixed sizes
  //!   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //!
  //! @tparam Strider An instance of a stride descriptor
  //================================================================================================
  template<auto Strides>
  struct stride  : detail::stride_base<Strides>::type
  {
    using parent = typename detail::stride_base<Strides>::type;

    /// Compile-time value for @ref glossary-order
    static constexpr std::ptrdiff_t static_order = parent::static_size;

    /// Type of dimensions' size
    using size_type = typename parent::value_type;

    /// Checks a @ref kwk::stride is unit - ie it's first value is statically known to be 1
    static constexpr auto is_unit = []()
    {
      auto first = get<0>(parent::descriptor);
      if constexpr(!is_joker_v<decltype(first)>) return first == 1; else return false;
    }();

    //==============================================================================================
    // stride is its self option keyword
    //==============================================================================================
    using stored_value_type = stride<Strides>;
    using keyword_type      = detail::strides_;

    constexpr auto operator()(keyword_type const&) const noexcept { return *this; }

    //==============================================================================================
    //! @brief Constructs a default @ref kwk::stride equals to [1 1 ... 1]
    //==============================================================================================
    constexpr stride() : parent([](auto, auto) { return 1;}) {}

    //==============================================================================================
    //! @brief Constructor from set of dimensions
    //!
    //! Constructs a kwk::stride with a variadic list of stride values.
    //! Values can either be any integral value, any fixed integral value or kwk::_.
    //!
    //! If you pass kwk::_ as a stride's value, it means that the stride will be using the
    //! default value for this dimension, i.e 1.
    //!
    //! Passing a runtime stride size where a static size is provided is undefined behavior if
    //! both values are not equal.
    //!
    //! This constructor will not take part in overload resolution if the number of values is not
    //! equal to the stride's order or if any value is neither convertible to std::ptrdiff_t
    //! nor kwk::_.
    //!
    //! @param  s Variadic list of dimensions' values
    //==============================================================================================
    constexpr stride(concepts::extent<size_type> auto... vals) noexcept
    requires( sizeof...(vals) == static_order )
    : stride()
    {
      parent::fill(vals...);
    }

    /// Stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, stride const& s)
    {
      os << "[";
      kumi::for_each( [&](auto e) { os << " " << +e; }, s);
      return os << " ]";
    }

    /// Number of dimensions
    static constexpr auto order() noexcept { return static_order; }

    /// Swap @ref kwk::stride contents
    friend constexpr void swap( stride& x, stride& y ) noexcept { x.swap(y); }
    using parent::swap;

    /// Equality comparison operator
    template<auto Strides2>
    constexpr bool operator==( stride<Strides2> const& other) const noexcept
    {
      return kumi::to_tuple(*this) == kumi::to_tuple(other);
    }

    /// Inequality comparison operator
    template<auto Strides2>
    constexpr bool operator!=( stride<Strides2> const& other) const noexcept
    {
      return kumi::to_tuple(*this) != kumi::to_tuple(other);
    }

    /// Indexing interface
    template<std::convertible_to<size_type>... Is>
    constexpr auto linearize(Is... is) const noexcept
    requires( sizeof...(Is) <= static_order )
    {
      return kumi::inner_product(*this, kumi::tie(is...), 0);
    }
  };

  /// Converts a @ref kwk::shape into its corresponding @ref kwk::stride, conserving as much static
  /// informations as possible.
  template<auto Shape>
  constexpr auto as_stride(shape<Shape> const& s) noexcept
  {
    constexpr auto prod = [](auto acc, auto m) { return push_back(acc, back(acc) * m); };

    // Build the perfect descriptor and the suitable values tuple
    constexpr auto d = kumi::pop_back(kumi::fold_right(prod, Shape, kumi::tuple{1}));
              auto v = kumi::pop_back(kumi::fold_right(prod, s    , kumi::tuple{1}));

    // Turn into a stride
    return kumi::apply([&](auto... e) { return stride<detail::to_combo<int>(d)>{e...}; }, v);
  }

  //================================================================================================
  //! @brief Generates a kwk::stride from a list of stride value or joker
  //! @param  ds        Variadic pack of sizes
  //================================================================================================
  template<concepts::extent<std::ptrdiff_t>... Ds>
  constexpr auto with_strides(Ds... ds) noexcept
  {
    return detail::make_extent<kwk::stride,std::ptrdiff_t>(ds...);
  }

  template<kumi::product_type Ds>
  constexpr auto with_strides( Ds ds) noexcept
  {
    return kumi::apply([](auto... s) { return with_strides(s...); }, ds);
  }
}

// Tuple interface adaptation
template<auto Desc>
struct  std::tuple_size<kwk::stride<Desc>>
      : std::integral_constant<std::size_t,kwk::stride<Desc>::static_order>
{};

template<std::size_t N, auto Desc>
struct  std::tuple_element<N, kwk::stride<Desc>>
{
  using type = typename kwk::stride<Desc>::size_type;
};
