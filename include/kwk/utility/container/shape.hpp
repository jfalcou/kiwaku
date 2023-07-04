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
#include <kwk/detail/sequence/prefilled.hpp>
#include <kwk/detail/sequence/shape_constraints.hpp>
#include <kwk/settings/extent.hpp>
#include <kwk/utility/traits/to_descriptor.hpp>
#include <kwk/utility/fixed.hpp>
#include <kwk/utility/slicer.hpp>
#include <cstddef>

namespace kwk::__ { struct size_; }

namespace kwk
{
  template<auto... D>
  struct shape;

  template<std::int32_t N, auto... D>
  constexpr auto compress(shape<D...> const&) noexcept;

  //====================================================================================================================
  //! @ingroup containers
  //! @brief  Fixed order shape with mixed size capability
  //!
  //! <hr/>
  //! **Required header**:
  //! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
  //!  #include<kwk/utility/container/shape.hpp>
  //! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //! <hr/>
  //!
  //! kwk::shape defines and optimally stores a set of integral values representing the size of a
  //! container along a fixed number of dimensions (or @ref glossary-order). Those sizes can be
  //! either specified at runtime or at compile-time. kwk::shape then provides an interface to
  //! query informations about those dimensions, sizes and compare shapes.
  //!
  //! kwk::shape can be defined in two ways:
  //!
  //! - using the kwk::of_size function. This is the main process of building a size to pass to
  //!   the constructor of containers as it leverages all the minutiae of defining the kwk::shape
  //!   type in itself.
  //!
  //!   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
  //!   kwk::shape s1 = kwk::of_size(kwk::fixed<5>);                   // Order 1 shape with static size
  //!   kwk::shape s2 = kwk::of_size(n, m);                            // Order 2 shape with dynamic sizes
  //!   kwk::shape s3 = kwk::of_size(kwk::fixed<2>,kwk::fixed<2>, n);  // Order 3 shape with mixed sizes
  //!   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //!
  //! - defining the layout of the kwk::shape using @ref glossary-extent and manually initializing it.
  //!
  //!   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
  //!   kwk::shape<5>                       s1;                   // Order 1 shape with static size
  //!   kwk::shape<kwk::_,kwk::_>           s2(n, m);             // Order 2 shape with dynamic sizes
  //!   kwk::shape<kwk::width, kwk::height> s3(n,kwk::fixed<3>);  // Order 2 shape with mixed sizes
  //!   kwk::shape<2,kwk::_,3>              s4( _, n, _);         // Order 3 shape with mixed sizes
  //!   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //!
  //! @tparam Shape An instance of a shape descriptor
  //====================================================================================================================
  template<auto... D>
  struct shape : __::prefilled_t<D...>::type
  {
    using parent        = typename __::prefilled_t<D...>::type;
    using constraint_t  = KWK_DEFAULT_SHAPE_CONSTRAINTS;

    //==================================================================================================================
    // If you end up here, you built a shape with duplicate axis names
    //==================================================================================================================
    static_assert ( !__::prefilled_t<D...>::duplicate_axis()
                  , "[KWK] Duplicate axis identifiers found when building shape."
                  );

    /// Compile-time value for @ref glossary-order
    static constexpr std::int32_t static_order  = parent::static_size;

    /// Indicates that the shape has at least one dimension specified at runtime
    static constexpr bool is_dynamic            = parent::is_dynamic;

    /// Indicates that the shape's dimensions are all specified at runtime
    static constexpr bool is_fully_dynamic      = parent::is_fully_dynamic;

    /// Indicates that the shape's dimensions are all specified compile-time
    static constexpr bool is_fully_static       = parent::is_fully_static;

    // shape is its self option keyword
    using stored_value_type = shape<D...>;
    using keyword_type      = __::size_;

    KWK_FORCEINLINE constexpr auto operator()(keyword_type const&) const noexcept { return *this; }

    //==================================================================================================================
    //! @brief Constructs a default @ref kwk::shape equals to [0 0 ... 0]
    //==================================================================================================================
    constexpr shape() noexcept = default;

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
    KWK_TRIVIAL constexpr shape(T... d) : parent(0, d...) {}

    //==================================================================================================================
    /// Copy constructor
    //==================================================================================================================
    KWK_TRIVIAL constexpr shape(shape const& d) : parent(d.__base())
    {}

    //==================================================================================================================
    /// Construct shape from another shape type
    //==================================================================================================================
    template<auto... D2>
    requires( constraint_t::is_contructible_from<parent{},typename shape<D2...>::parent{}>() )
    KWK_TRIVIAL explicit(static_order != sizeof...(D2))
    constexpr shape(shape<D2...> const& other)
    {
      constraint_t::construct(*this, other);
    }

    //==================================================================================================================
    /// Assignment operator
    //==================================================================================================================
    constexpr shape& operator=( shape const& other ) & noexcept
    {
      this->__base() = other.__base();
      return *this;
    }

    template<auto... D2>
    requires( constraint_t::is_contructible_from<parent{},typename shape<D2...>::parent{}>() )
    constexpr shape& operator=( shape<D2...> const& other ) & noexcept
    {
      constraint_t::construct(*this, other);
      return *this;
    }

    using parent::swap;
    /// Swap shapes' contents
    friend void swap( shape& x, shape& y ) noexcept { x.swap(y); }

    /// Equality comparison operator
    template<auto... D2>
    KWK_PURE friend constexpr bool operator==(shape const& a, shape<D2...> const& b ) noexcept
    {
      constexpr auto sz = std::min(static_order, shape<D2...>::static_order);
      bool result = true;
      kumi::for_each( [&](auto aa, auto bb) { result = result && (aa == bb); }
                    , compress<sz>(a), compress<sz>(b)
                    );
      return result;
    }

    //==================================================================================================================
    //! @brief Check if a position fits into current shape volume
    //!
    //! Check if all coordinates of a given position fits inside a shape volume.
    //! Does not participate in overload resolution if the order of the shape and the dimension of the coordinate
    //! do not match.
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

    //==============================================================================================
    //! @brief Check if a shape fit current's shape constraints on size and dimension
    //! @param ref Shape to use as a reference extent
    //! @return `true` if both shape has the same order and if each statically set dimension of
    //!         `ref` is matched with an equal value in the current shape.
    //==============================================================================================
    template<auto... D2>
    KWK_PURE constexpr bool fit_constraints(shape<D2...> const& ref) const noexcept
    {
      return constraint_t::fit_constraints(*this, ref);
    }

    /// Number of dimensions
    KWK_PURE static constexpr auto order() noexcept { return static_order; }

    //==================================================================================================================
    //! @brief Number of non-trivial dimensions
    //!
    //! Computes the number of non-trivial dimensions, i.e dimension with size equals to 1 and that
    //! doesn't participate to the shape's extent.
    //==================================================================================================================
    KWK_PURE constexpr std::int32_t nbdims() const noexcept
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
    KWK_PURE constexpr std::int32_t numel() const noexcept
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

      return kumi::apply( []<typename... V>(V... v) { return shape<to_descriptor(V{})...>{v...}; }, sliced );
    }

    /// Stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, shape const& s)
    {
      os << "[";
      kumi::for_each_index( [&](auto i, auto) { os << " " << get<i>(s); }, s);
      return os << " ]";
    }

    // Access to base type for internal implementation
    KWK_TRIVIAL constexpr auto const& __base() const  noexcept { return static_cast<parent const&>(*this);  }
    KWK_TRIVIAL constexpr auto &      __base()        noexcept { return static_cast<parent&>(*this);        }
  };

  /// Deduction guide for @ref kwk::shape
  template<typename... T>
  shape(T...) -> shape<to_descriptor(T{})...>;

  //====================================================================================================================
  //! @brief Generates a kwk::shape from a list of @ref glossary-extent
  //! @tparam SizeType  Integral type used to store sizes.
  //! @param  ds        Variadic pack of @ref glossary-extent
  //====================================================================================================================
  template<int..., concepts::extent... D>
  KWK_TRIVIAL constexpr auto of_size(D... d)
  {
    return shape<to_descriptor(D{})...>{d...};
  }

  /// @overload
  template<std::integral SizeType, int..., concepts::extent... D>
  KWK_TRIVIAL constexpr auto of_size(D... d)
  {
    return of_size(__::force_type<SizeType>(d)...);
  }

  //====================================================================================================================
  //! @brief Generates a kwk::shape from a tuple of @ref glossary-extent
  //! @tparam SizeType  Integral type used to store sizes.
  //! @param  t         Tuple of  @ref glossary-extent
  //====================================================================================================================
  template<int..., kumi::product_type T>
  KWK_TRIVIAL constexpr auto of_size(T const& t)
  {
    return kumi::apply( [](auto... e) {  return of_size(e...); }, t);
  }

  /// @overload
  template<typename SizeType, int...,kumi::product_type T>
  KWK_TRIVIAL constexpr auto of_size(T const& t)
  {
    return kumi::apply( [](auto... e) {  return of_size<SizeType>(e...); }, t);
  }

  //====================================================================================================================
  //! @brief Compress a kwk::shape to a given order
  //! @tparam N  Expected @ref glossary-order of the generated shape.
  //! @param  s  Original shape to compress
  //! @return A new kwk::shape instance which order has been set to N by compressing dimensions.
  //====================================================================================================================
  template<std::int32_t N, auto... D>
  KWK_CONST constexpr auto compress(shape<D...> const& s) noexcept
  requires(N>0 && N<=sizeof...(D))
  {
    constexpr std::int32_t sz = shape<D...>::static_order;
    if constexpr(N == sz) return s;
    else
    {
      auto [head,tail] = kumi::split(s, kumi::index<sz-N+1>);
      auto value = kumi::fold_right( [](auto acc, auto v) { return acc * v; }, pop_front(head), get<0>(head));
      return of_size(kumi::push_front(tail,value));
    }
  }

  using _1D = shape<_>;
  using _2D = shape<_,_>;
  using _3D = shape<_,_,_>;
  using _4D = shape<_,_,_,_>;

//  template<std::size_t N>
//  using _nD = []<std::size_t... I>(std::index_sequence<I...>)
//   {
//     return of_size(along<N-I-1>...);
//   }(std::make_index_sequence<N>{});
}

// Tuple interface adaptation
template<auto... D>
struct  std::tuple_size<kwk::shape<D...>>
      : std::integral_constant<std::int32_t,sizeof...(D)>
{};

template<std::size_t N, auto... D>
struct  std::tuple_element<N, kwk::shape<D...>>
: std::tuple_element<N, typename kwk::shape<D...>::parent>
{};
