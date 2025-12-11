//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/detail/abi.hpp>
#include <kwk/utility/container/shape.hpp>
#include <kwk/detail/sequence/prefilled.hpp>
#include <cstddef>

#if !defined(_MSC_VER)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#endif

namespace kwk::__
{
  struct strides_;
}

namespace kwk
{
  template<concepts::extent... D>
  constexpr auto with_strides(D... d) noexcept;

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
  //! @tparam Strides An instance of a stride descriptor
  //================================================================================================
  template<auto... Strides>
  struct stride  : __::prefilled_t<Strides...>::type
  {
    using parent = typename __::prefilled_t<Strides...>::type;

    /// Compile-time value for @ref glossary-order
    static constexpr std::int32_t static_order = parent::static_size;

    /// Checks a @ref kwk::stride is unit - ie it's innermost value is statically known to be 1
    static constexpr auto is_unit = []()
    {
      auto d0 = kumi::back(parent::descriptors);
      if constexpr(concepts::static_axis<decltype(d0)>) return d0.value == 1; else return false;
    }();

    //==============================================================================================
    // stride is its self option keyword
    //==============================================================================================
    using stored_value_type = stride<Strides...> const&;
    using keyword_type      = __::strides_;

    // constexpr auto operator()(keyword_type const&) const noexcept { return *this; }
    constexpr stored_value_type operator()(keyword_type const&) const noexcept { return *this; }


    //==============================================================================================
    //! @brief Constructs a default @ref kwk::stride equals to [1 1 ... 1]
    //==============================================================================================
    constexpr stride() : parent() {}

    //==============================================================================================
    //! @brief Constructor from set of dimensions
    //!
    //! Constructs a kwk::stride with a variadic list of extents.
    //!
    //! Passing a runtime stride size where a static size is provided is undefined behavior if
    //! both values are not equal.
    //!
    //! @param values Variadic list of dimensions' values
    //==============================================================================================
    constexpr stride(concepts::extent auto... values) noexcept : parent(1, values...)
    {}

    /// Stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, stride const& s)
    {
      os << "[";
      kumi::for_each_index( [&](auto i, auto) { os << " " << +get<i>(s); }, s);
      return os << " ]";
    }

    /// Number of dimensions
    static constexpr auto order() noexcept { return static_order; }

    /// Equality comparison operator
    template<auto... S2>
    constexpr bool operator==(stride<S2...> const& other) const noexcept
    requires(sizeof...(S2) == static_order)
    {
      return kumi::to_tuple(*this) == kumi::to_tuple(other);
    }

    /// Indexing interface
    template<std::convertible_to<std::ptrdiff_t>... Is>
    KWK_PURE constexpr auto linearize(Is... is) const noexcept
    requires( sizeof...(Is) == static_order )
    {
      return [=, *this]<int...i>(std::integer_sequence<int, i...>) KWK_LAMBDA_FORCEINLINE
      {
          return (0 + ... + (get<i>(*this) * is));
      }(std::make_integer_sequence<int, static_order>{});
    }

    template<typename... Slicers>
    constexpr auto operator()(Slicers const&... s ) const noexcept
    requires( sizeof...(Slicers) == static_order )
    {
      auto  sliced  = kumi::map_index ( [&](auto i, auto m) { return restride(*this,m,i); }
                                      , kumi::tie(s...)
                                      );

      return kumi::apply( [](auto... v) { return with_strides(v...); }, sliced );
    }
  };

  /// Deduction guide for kwk::stride
  template<concepts::extent... T>
  stride(T...) -> stride<to_descriptor(T{})...>;

  //================================================================================================
  //! @brief Generates a kwk::stride from a list of stride value or joker
  //! @param  d        Variadic pack of sizes
  //================================================================================================
  template<concepts::extent... D>
  constexpr auto with_strides(D... d) noexcept
  {
    return stride{d...};
  }

  template<kumi::product_type D>
  constexpr auto with_strides(D d) noexcept
  {
    return kumi::apply([](auto... s) { return with_strides(s...); }, d);
  }

  /// Converts a kwk::shape into its corresponding kwk::stride, keeping as much static
  /// informations as possible.
  template<auto... D>
  KWK_PURE constexpr auto as_stride(shape<D...> s) noexcept
  {
    if constexpr(sizeof...(D) == 1) return stride{s.template axis<0>() = fixed<1>};
    else
    {
      auto const d  = kumi::fold_right ( [](auto m, auto a){ return push_front(a, m * front(a)); }
                                      , kumi::pop_front(s)
                                      , kumi::tuple{fixed<1>}
                                      );

      return [&]<std::size_t... I>(std::index_sequence<I...>, auto t)
      {
        return stride{ (s.template axis<I>() =  get<I>(t))... };
      }(std::make_index_sequence<sizeof...(D)>{}, d);
    }
  }

  template<std::int32_t N, auto... D>
  constexpr auto compress(stride<D...> const& s) noexcept
  {
    auto extracted = kumi::extract(s, kumi::index<stride<D...>::static_size-N>);
    return with_strides(extracted);
  }
}

#if !defined(KWK_DOXYGEN_INVOKED)
// Tuple interface adaptation
template<auto... D>
struct  std::tuple_size<kwk::stride<D...>>
      : std::integral_constant<std::int32_t,sizeof...(D)>
{};

template<std::size_t N, auto... D>
struct  std::tuple_element<N, kwk::stride<D...>>
: std::tuple_element<N, typename kwk::stride<D...>::parent>
{};
#endif

#if !defined(_MSC_VER)
#pragma GCC diagnostic pop
#endif
