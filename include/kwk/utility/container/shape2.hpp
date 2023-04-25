//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#if !defined(_MSC_VER)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#endif

#include <kwk/concepts/extent.hpp>
#include <kwk/detail/abi.hpp>
#include <kwk/detail/kumi.hpp>
#include <kwk/detail/raberu.hpp>
#include <kwk/detail/sequence/prefilled2.hpp>
#include <kwk/settings/extent.hpp>
#include <kwk/utility/traits/to_descriptor.hpp>
#include <kwk/utility/fixed.hpp>
#include <kwk/utility/joker.hpp>
#include <kwk/utility/slicer.hpp>
#include <cstddef>

namespace kwk::__ { struct size_; }

#if !defined(KWK_DEFAULT_SHAPE_CONSTRAINTS)
#define KWK_DEFAULT_SHAPE_CONSTRAINTS ::kwk::strict
#endif

namespace kwk
{
  template<auto... D>
  struct shape2;

  template<std::int32_t N, auto... D>
  constexpr auto compress(shape2<D...> const&) noexcept;

  template<auto... D>
  struct shape2 : kwk::__::prefilled2_t<D...>::type
  {
    using parent        = typename kwk::__::prefilled2_t<D...>::type;
    using constraint_t  = KWK_DEFAULT_SHAPE_CONSTRAINTS;

    //==================================================================================================================
    // If you end up here, you built a shape with duplicate axis names
    //==================================================================================================================
    static_assert ( !kwk::__::prefilled2_t<D...>::duplicate_axis()
                  , "[KWK] Duplicate axis identifiers found when building shape2."
                  );

    /// Compile-time value for @ref glossary-order
    static constexpr std::int32_t static_order  = parent::static_size;

    /// Indicates that the shape has at least one dimension specified at runtime
    static constexpr bool is_dynamic            = parent::is_dynamic;

    /// Indicates that the shape's dimensions are all specified at runtime
    static constexpr bool is_fully_dynamic      = parent::dynamic_size == static_order;

    /// Indicates that the shape's dimensions are all specified compile-time
    static constexpr bool is_fully_static       = parent::is_fully_static;

    // shape is its self option keyword
    using stored_value_type = shape2<D...>;
    using keyword_type      = __::size_;

    KWK_FORCEINLINE constexpr auto operator()(keyword_type const&) const noexcept { return *this; }

    //==================================================================================================================
    //! @brief Constructs a default @ref kwk::shape equals to [0 0 ... 0]
    //==================================================================================================================
    constexpr shape2() noexcept = default;

    //==================================================================================================================
    //! @brief Construct from a subset of @ref glossary-extent
    //!
    //! This constructor takes a variadic list of @ref glossary-extent for each shape's dimension.
    //! Note that passing a @ref glossary-extent to overwrite a compile-time @ref glossary-extent is undefined behavior.
    //!
    //! @groupheader{Example}
    //! @include docs/shape/mixed.cpp
    //!
    //! @param dims  Variadic list of @ref glossary-extent
    //==================================================================================================================
    template<concepts::extent... T>
    KWK_TRIVIAL constexpr shape2(T... d) : parent(0, d...) {}

    KWK_TRIVIAL constexpr shape2(shape2 const& d)
    : parent(d.__base())
    {
    }

    template<auto... D2>
    requires( constraint_t::is_contructible_from<shape2,shape2<D2...>>() )
    KWK_TRIVIAL constexpr shape2(shape2<D2...> const& d)
    {
      constraint_t::construct(*this, d);
    }

    //==================================================================================================================
    /// Assignment operator
    //==================================================================================================================
    template<auto... D2>
    requires std::constructible_from<shape2, shape2<D2...>>
    constexpr shape2& operator=( shape2<D2...> const& other ) & noexcept
    {
      shape2 that(other);
      swap(that);
      return *this;
    }

    //==============================================================================================
    // If an error appears here, it means you try to assign a shape to a shape with non-equivalent
    // axis description.
    //  E.g:
    //  shape<extent()()> x;
    //  x = of_size(width = 4, height = 2);
    //==============================================================================================
    template<auto... D2>
    requires(!std::constructible_from<shape2, shape2<D2...>>)
    constexpr shape2& operator=( shape2<D2...> const& other ) & noexcept = delete;

    /// Swap shapes' contents
    friend void swap( shape2& x, shape2& y ) noexcept { x.swap(y); }
    using parent::swap;

    /// Equality comparison operator
    KWK_PURE friend constexpr bool operator==( shape2 const& a, shape2 const& b ) noexcept
    {
      return a.storage_ == b.storage_;
    }

    //==================================================================================================================
    //! @brief Check if a position fits into current shape volume
    //!
    //! Check if all coordinates of a given position fits inside a shape volume
    //!
    //! @param p  List of coordinates to check
    //! @return `true` if all extent of other fits exactly inside current shape. `false` otherwise.
    //==================================================================================================================
    template<std::integral... Coords>
    KWK_PURE constexpr bool contains(Coords... p) const noexcept
    requires(static_order == sizeof...(Coords))
    {
      return kumi::apply( [&](auto... m) { return ((p < m) && ... && true); }, *this);
    }

    //==================================================================================================================
    // If you end up here, you try to check the validity of a position with non compatible
    // number of dimensions, e.g a 3D position in a 2D shape
    //==================================================================================================================
    template<std::integral... Coords>
    constexpr bool contains(Coords...) const noexcept
    requires(static_order != sizeof...(Coords)) = delete;

    //==================================================================================================================
    //! @brief Check for shape's equivalence
    //!
    //! Two shapes are equivalent if their @ref glossary-axis identifiers are the same.
    //!
    //! @param other  Shape to compare to
    //! @return A boolean value indicating if current shape and the `other` one are axis-equivalent.
    //==================================================================================================================
    template<auto... D2>
    KWK_TRIVIAL constexpr bool is_equivalent(shape2<D2...> const& other)
    {
      return parent::is_equivalent(other.__base());
    }

    /// Number of dimensions
    static constexpr auto order() noexcept { return static_order; }

    //==================================================================================================================
    //! @brief Number of non-trivial dimensions
    //!
    //! Computes the number of non-trivial dimensions, i.e dimension with size equals to 1 and that
    //! doesn't participate to the shape's extent.
    //==================================================================================================================
    constexpr std::int32_t nbdims() const noexcept
    {
      if constexpr(static_order == 0)  return 0;
      else
      {
        auto first_non_trivial = kumi::locate(kumi::to_tuple(*this), [](auto m) { return m != 1; });
        return static_order - first_non_trivial;
      }
    }

    //==================================================================================================================
    //! @brief Number of elements
    //!
    //! Computes the number of elements storable in current kwk::shape, i.e the product of all
    //! dimensions' size.
    //==================================================================================================================
    constexpr auto numel() const noexcept
    {
      if constexpr(static_order == 0) return 0;
      else return kumi::fold_left([](auto a, auto b){ return a*b; }, *this, fixed<1>);
    }

    //==================================================================================================================
    //! @brief Shape slicing interface
    //!
    //! Computes the shape of the sub-volume described by the slicers passed as parameters
    //!
    //! Does not participate in overload resolution if the number of parameters is not equal to the
    //! shape order.
    //!
    //! @param s  Variadic list of slicer instances
    //! @return   An instance of @ref kwk::shape corresponding to the shape of the selected
    //!           sub-volume
    //==================================================================================================================
    // has to be defined inline due to (Apple) Clang deficiencies
    // https://github.com/llvm/llvm-project/issues/58952
    //==================================================================================================================
    template<typename... Slicers>
    constexpr auto operator()(Slicers const&... s ) const noexcept
    requires( sizeof...(Slicers) <= static_order )
    {
      auto  shd     = compress<sizeof...(s)>(*this);
      auto  sliced  = kumi::map_index ( [&](auto i, auto m) { return reshape(shd,m,i); }
                                      , kumi::tie(s...)
                                      );

      return kumi::apply( [](auto... v) { return shape2{v...}; }, sliced );
    }

    /// Stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, shape2 const& s)
    {
      os << "[";
      kumi::for_each_index( [&](auto i, auto) { os << " " << get<i>(s); }, s);
      return os << " ]";
    }

    // Access to base type for internal implementation
    auto const& __base() const  noexcept { return static_cast<parent const&>(*this);  }
    auto &      __base()        noexcept { return static_cast<parent&>(*this);        }
  };

  /// Deduction guide for @ref kwk::shape
  template<typename... T>
  shape2(T...) -> shape2<kwk::to_descriptor(T{})...>;

  //====================================================================================================================
  //! @brief Compress a kwk::shape to a given order
  //! @tparam N  Expected @ref glossary-order of the generated shape.
  //! @param  s  Original shape to compress
  //! @return A new kwk::shape instance which order has been set to N by compressing dimensions.
  //====================================================================================================================
  template<std::int32_t N, auto... D>
  constexpr auto compress(shape2<D...> const& s) noexcept
  requires(N>0 && N<=sizeof...(D))
  {
    constexpr std::int32_t sz = sizeof...(D);
    if constexpr(N == sizeof...(D)) return s;
    else
    {
      auto [head,tail] = kumi::split(s, kumi::index<sz-N+1>);
      auto value = kumi::fold_right( [](auto acc, auto v) { return acc * v; }, pop_front(head), get<0>(head));
      return kumi::apply( [&](auto... e){ return shape2{value,e...}; }, tail);
    }
  }

  //====================================================================================================================
  //! @brief Generates a kwk::shape from a list of @ref glossary-extent
  //! @tparam SizeType  Integral type used to store sizes.
  //! @param  ds        Variadic pack of @ref glossary-extent
  //====================================================================================================================
  template<concepts::extent... D>
  KWK_TRIVIAL constexpr auto of_size2(D... d)
  {
    return shape2{kwk::__::force_type<kwk::joker>(d)...};
  }

  /// @overload
  template<typename SizeType, int...,concepts::extent... D>
  KWK_TRIVIAL constexpr auto of_size2(D... d)
  {
    return shape2{kwk::__::force_type<SizeType>(d)...};
  }

  //====================================================================================================================
  //! @brief Generates a kwk::shape from a tuple of @ref glossary-extent
  //! @tparam SizeType  Integral type used to store sizes.
  //! @param  t         Tuple of  @ref glossary-extent
  //====================================================================================================================
  template<kumi::product_type T>
  KWK_TRIVIAL constexpr auto of_size2(T const& t)
  {
    return kumi::apply( [](auto... e) {  return of_size2(e...); }, t);
  }

  /// @overload
  template<typename SizeType, int...,kumi::product_type T>
  KWK_TRIVIAL constexpr auto of_size2(T const& t)
  {
    return kumi::apply( [](auto... e) {  return of_size2<SizeType>(e...); }, t);
  }
}

// Tuple interface adaptation
template<auto... D>
struct  std::tuple_size<kwk::shape2<D...>>
      : std::integral_constant<std::int32_t,sizeof...(D)>
{};

template<std::size_t N, auto... D>
struct  std::tuple_element<N, kwk::shape2<D...>>
: std::tuple_element<N, typename kwk::shape2<D...>::parent>
{};
