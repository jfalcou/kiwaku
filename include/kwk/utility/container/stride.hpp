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
  template<auto... S>
  struct stride  : __::prefilled_t<S...>::type
  {
    using parent = typename __::prefilled_t<S...>::type;

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
    using stored_value_type = stride<S...>;
    using keyword_type      = __::strides_;

    constexpr auto operator()(keyword_type const&) const noexcept { return *this; }

    //==============================================================================================
    //! @brief Constructs a default @ref kwk::stride equals to [1 1 ... 1]
    //==============================================================================================
    constexpr stride() : parent(1) {}

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
      kumi::for_each_index( [&](auto i, auto) { os << " " << get<i>(s); }, s);
      return os << " ]";
    }

    /// Number of dimensions
    static constexpr auto order() noexcept { return static_order; }

    using parent::swap;
    /// Swap @ref kwk::stride contents
    friend constexpr void swap( stride& x, stride& y ) noexcept { x.swap(y); }

    /// Equality comparison operator
    template<auto... S2>
    constexpr bool operator==(stride<S2...> const& other) const noexcept
    requires(sizeof...(S2) == static_order)
    {
      return kumi::to_tuple(*this) == kumi::to_tuple(other);
    }

    /// Indexing interface
    template<std::convertible_to<std::ptrdiff_t>... Is>
    KWK_PURE KWK_FORCEINLINE constexpr
    auto linearize(Is... is) const noexcept
    requires( sizeof...(Is) == static_order )
    {
      return [=, *this]<int...i>(std::integer_sequence<int, i...>)
      {
          return (0 + ... + (get<i>(*this) * is));
      }(std::make_integer_sequence<int, static_order>{});
    }

    // Access to base type for internal implementation
    KWK_TRIVIAL auto const& __base() const  noexcept { return static_cast<parent const&>(*this);  }
    KWK_TRIVIAL auto &      __base()        noexcept { return static_cast<parent&>(*this);        }
  };

  /// Deduction guide for @ref kwk::stride
  template<typename... T>
  stride(T...) -> stride<to_descriptor(T{})...>;

  /// Converts a @ref kwk::shape into its corresponding @ref kwk::stride, keeping as much static
  /// informations as possible.
  template<auto... D>
  KWK_CONST constexpr
  auto as_stride(shape<D...> const s) noexcept
  {
    if constexpr(sizeof...(D) == 1) return stride{s.template axis<0>() = fixed<1>};
    else
    {
      auto const d = kumi::fold_left( [](auto a, auto m){ return push_front(a, m * front(a)); }
                                    , s
                                    , kumi::tuple{fixed<1>}
                                    );

      return [&]<std::size_t... I>(std::index_sequence<I...>, auto t)
      {
        //void* pp = with_strides( (s.template axis<I>() =  get<I>(t))...);;
        return with_strides( (s.template axis<I>() =  get<I>(t))...);
      }(std::make_index_sequence<sizeof...(D)>{}, pop_front(d));
    }
  }

  //================================================================================================
  //! @brief Generates a kwk::stride from a list of stride value or joker
  //! @param  ds        Variadic pack of sizes
  //================================================================================================
  template<int..., concepts::extent... D>
  constexpr auto with_strides(D... d) noexcept
  {
    return stride<to_descriptor(D{})...>{d...};
  }

  template<int..., kumi::product_type D>
  constexpr auto with_strides(D d) noexcept
  {
    return kumi::apply([](auto... s) { return with_strides(s...); }, d);
  }
}

// Tuple interface adaptation
template<auto... D>
struct  std::tuple_size<kwk::stride<D...>>
      : std::integral_constant<std::int32_t,sizeof...(D)>
{};

template<std::size_t N, auto... D>
struct  std::tuple_element<N, kwk::stride<D...>>
: std::tuple_element<N, typename kwk::stride<D...>::parent>
{};

#if !defined(_MSC_VER)
#pragma GCC diagnostic pop
#endif
