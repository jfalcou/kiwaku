//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/concepts/extent.hpp>
#include <kwk/detail/abi.hpp>
#include <kwk/detail/kumi.hpp>
#include <kwk/detail/raberu.hpp>
#include <kwk/detail/sequence/extent_builder.hpp>
#include <kwk/detail/sequence/prefilled.hpp>
#include <kwk/settings/extent.hpp>
#include <kwk/utility/fixed.hpp>
#include <kwk/utility/joker.hpp>
#include <kwk/utility/slicer.hpp>
#include <cstddef>

#if !defined(_MSC_VER)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#endif

namespace kwk::__ { struct size_; }

namespace kwk
{
  template<auto Shape> struct shape;

  template<std::int32_t N, auto Desc>
  constexpr auto compress(shape<Desc> const& s) noexcept;

  //================================================================================================
  //! @brief Generates a kwk::shape from a list of sizes
  //! @tparam SizeType  Integral type used to store sizes. If unspecified, the most fitting integral
  //!                   type is used.
  //! @param  ds        Variadic pack of sizes
  //================================================================================================
  template<std::integral SizeType, int..., concepts::extent<SizeType>... Ds>
  KWK_CONST constexpr auto of_size(Ds... ds) noexcept
  {
    return __::make_extent<kwk::shape,SizeType>(ds...);
  }

  /// @overload
  template<int..., typename... Ds>
  KWK_CONST constexpr   auto of_size( Ds... ds) noexcept
          ->  decltype(of_size<typename __::largest_integral<Ds...>::type>(ds...)  )
  {
    return of_size<typename __::largest_integral<Ds...>::type>(ds...);
  }

  /// @overload
  template<int..., kumi::product_type Ds>
  KWK_CONST constexpr auto of_size( Ds ds) noexcept
  {
    return kumi::apply([](auto... s) { return of_size(s...); }, ds);
  }

  /// @overload
  template<std::integral SizeType,int..., kumi::product_type Ds>
  KWK_CONST constexpr auto of_size( Ds ds) noexcept
  {
    return kumi::apply([](auto... s) { return of_size<SizeType>(s...); }, ds);
  }

  //================================================================================================
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
  //! - defining the layout of the kwk::shape and manually initializing it. The description of the
  //!   kwk::shape layout is done with the kwk::extent object, one of the pre-defined layouts or.
  //!   by combining axis descriptors.
  //!
  //!   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
  //!   kwk::shape< kwk::extent[5] >          s1;                   // Order 1 shape with static size
  //!   kwk::shape< kwk::_2D >                s2(n, m);             // Order 2 shape with dynamic sizes
  //!   kwk::shape< kwk::width * kwk::height> s3(n,kwk::fixed<3>);  // Order 2 shape with mixed sizes
  //!   kwk::shape< kwk::extent[2]()[3]>      s4( _, n, _);         // Order 3 shape with mixed sizes
  //!   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //!
  //! @tparam Shape An instance of a shape descriptor
  //================================================================================================
  template<auto Shape>
  struct shape : kwk::__::prefilled<Shape>
  {
    using parent = kwk::__::prefilled<Shape>;

    /// Compile-time shape descriptor
    static constexpr auto descriptor = Shape;

    /// Compile-time value for @ref glossary-order
    static constexpr std::int32_t static_order = parent::static_size;

    /// Type of dimensions' size
    using size_type = typename parent::value_type;

    /// Indicates that the shape has at least one dimension specified at runtime
    static constexpr bool is_dynamic        = parent::is_dynamic;

    /// Indicates that the shape's dimensions are all specified at runtime
    static constexpr bool is_fully_dynamic  = parent::dynamic_size == static_order;

    /// Indicates that the shape's dimensions are all specified compile-time
    static constexpr bool is_fully_static   = parent::is_fully_static;

    // shape is its self option keyword
    using stored_value_type = shape<Shape>;
    using keyword_type      = __::size_;

    KWK_FORCEINLINE constexpr auto operator()(keyword_type const&) const noexcept { return *this; }

    //==============================================================================================
    //! @brief Constructs a default @ref kwk::shape equals to [0 0 ... 0]
    //==============================================================================================
    constexpr shape() noexcept = default;

    //==============================================================================================
    //! @brief Constructor from set of dimensions
    //!
    //! Constructs a kwk::shape with a variadic list of integral-convertible  dimension values.
    //! Values can either be any integral value or any fixed integral value.
    //!
    //! Passing a runtime dimension size where a static size is provided is undefined behavior if
    //! both values are not equal.
    //!
    //! This constructor will not take part in overload resolution if the number of values exceed
    //! shape's order or if any value is neither convertible to kwk::shape::size_type.
    //!
    //! @param  values Variadic list of dimensions' values
    //==============================================================================================
    constexpr shape(std::convertible_to<size_type> auto... values) noexcept
    requires( sizeof...(values) <= static_order )
    {
      this->fill(values...);
    }

    //==============================================================================================
    //! @brief Construct from a subset of runtime dimension values specifiers
    //!
    //! This constructor takes a variadic list of arguments specifying the size specified for a
    //! given runtime dimension. Those sizes are passed by using an axis descriptor, an integral
    //! convertible type or kwk::_.
    //!
    //! If you pass kwk::_ as a dimension's value, it means that the shape will be using the
    //! default value for this dimension (i.e. 0).
    //!
    //! Passing a dimension to overwrite a compile-time dimensions is undefined behavior.
    //!
    //! This constructor will not take part in overload resolution if its parameters doesn't
    //! conform to the shape descriptor specified or deduced.
    //!
    //! @groupheader{Example}
    //! @include docs/shape/mixed.cpp
    //!
    //! @param dims  Variadic list of dimension/size association
    //==============================================================================================
    template<concepts::extent<size_type>... A>
    constexpr shape(A const... dims) noexcept
    requires( !(std::convertible_to<A,size_type> && ...)
            && make_combo<size_type>(Shape).is_equivalent(__::as_descriptor<size_type>(A{}...))
            )
    {
      [=, this]<std::size_t... N>(std::index_sequence<N...>)
      {
        this->fill(__::as_axis(dims,get<N>(Shape),kumi::index<static_order-N-1>)...);
      }(std::make_index_sequence<sizeof...(A)>{});
    }

    //==============================================================================================
    //! @brief Constructs from a shape with compatible layout
    //!
    //! Constructs a kwk::shape from another one with compatible layout but potentially different
    //! size_type.
    //!
    //! Two kwk::shapes have compatible layout if :
    //!   + All their axis match
    //!   + They have the same @ref glossary-order
    //!   + Each axis of the constructed @ref kwk::shape can hold its equivalent from the source
    //!     shape, i.e it's runtime specified or, if it's compile-time specified, has the same
    //!     value than its source.
    //!
    //! This constructor does not participate in overload resolution if the shapes have a
    //! non-compatible layout.
    //!
    //! @groupheader{Example}
    //! @include docs/shape/compatible.cpp
    //!
    //! @param other  Source kwk::shape to copy
    //==============================================================================================
    template<auto OtherShape>
    constexpr shape( shape<OtherShape> const& other ) noexcept
    requires(   make_combo<size_type>(Shape).is_equivalent(OtherShape)
            &&  make_combo<size_type>(Shape).is_compatible(OtherShape)
            )
    {
      kumi::apply([this](auto const... vals){ this->fill(vals...); }, other);
    }

    //==============================================================================================
    //! @brief Constructs from a shape with a lower order
    //!
    //! Constructs a kwk::shape from a shape with lower orders, filling the missing sizes with 1.
    //! Lower order axis must be equivalent.
    //!
    //! @groupheader{Example}
    //! @include docs/shape/odd_sized.cpp
    //!
    //! @param other  Shape to copy
    //==============================================================================================
    template<auto Other>
    constexpr explicit shape( shape<Other> const& other ) noexcept
              requires(shape<Other>::static_order < static_order)
    {
      kumi::apply([this](auto const... vals){ this->fill(vals...); }, other);
    }

    //==============================================================================================
    //! @brief Constructs from a shape with a higher order
    //!
    //! Constructs a kwk::shape from a shape with higher orders, compacting the extra-dimensions
    //! into the last.
    //!
    //! If you require compile-time shape descriptor to be updated, consider using @ref compress.
    //!
    //! This constructor does not participate in overload resolution of the shape is not fully
    //! specified at runtime.
    //!
    //! @groupheader{Example}
    //! @include docs/shape/odd_sized.cpp
    //!
    //! @param o  Shape to copy
    //==============================================================================================
    template<auto Other>
    constexpr explicit  shape( shape<Other> const& o ) noexcept
                        requires( shape<Other>::static_order > static_order && is_fully_dynamic)
            : shape( compress<static_order>(o) )
    {}

    //==============================================================================================
    /// Assignment operator
    //==============================================================================================
    template<auto Other>
    requires std::constructible_from<shape, shape<Other>>
    constexpr shape& operator=( shape<Other> const& other ) & noexcept
    {
      shape that(other);
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
    template<auto Other>
    requires(!std::constructible_from<shape, shape<Other>>)
    constexpr shape& operator=( shape<Other> const& other ) & noexcept = delete;

    /// Swap shapes' contents
    friend void swap( shape& x, shape& y ) noexcept { x.swap(y); }
    using parent::swap;

    /// Equality comparison operator
    KWK_PURE friend constexpr bool operator==( shape const & a, shape const & b ) noexcept
    {
        return a.storage() == b.storage();
    }

    template<auto S2>
    KWK_PURE friend constexpr bool operator==(shape const & a, shape< S2 > const & b ) noexcept
    requires(  compress<std::min(Shape.size(), S2.size())>(Shape)
              .is_equivalent(compress<std::min(Shape.size(), S2.size())>(S2))
            )
    {
      constexpr auto sz = std::min(Shape.size(), S2.size());
      bool result = true;
      kumi::for_each ( [&](auto aa, auto bb) { result = result && (aa == bb); }
                            , compress<sz>(a)
                            , compress<sz>(b)
                            );
      return result;
    }

    template<auto S2>
    friend constexpr bool operator==(shape const& a, shape<S2> const& b) noexcept
    requires(  !compress<std::min(Shape.size(), S2.size())>(Shape)
              .is_equivalent(compress<std::min(Shape.size(), S2.size())>(S2))
            ) = delete;

    //==============================================================================================
    //! @brief Check if a position fits into current shape volume
    //!
    //! Check if all coordinates of a given position fits inside a shape volume
    //!
    //! @param p  List of coordinates to check
    //! @return `true` if all extent of other fits exactly inside current shape. `false` otherwise.
    //!
    //==============================================================================================
    template<std::integral... Coords>
    KWK_PURE constexpr bool contains(Coords... p) const noexcept
    requires(static_order == sizeof...(Coords))
    {
      return kumi::apply( [&](auto... m) { return ((p < m) && ... && true); }, *this);
    }

    template<std::integral... Coords>
    constexpr bool contains(Coords...) const noexcept
    requires(static_order != sizeof...(Coords)) = delete;

    /// Number of dimensions
    static constexpr auto order() noexcept { return static_order; }

    //==============================================================================================
    //! @brief Number of non-trivial dimensions
    //!
    //! Computes the number of non-trivial dimensions, i.e dimension with size equals to 1 and that
    //! doesn't participate to the shape's extent.
    //==============================================================================================
    constexpr std::int32_t nbdims() const noexcept
    {
      if constexpr(static_order == 0)  return 0;
      else
      {
        auto first_non_trivial = kumi::locate(kumi::to_tuple(*this), [](auto m) { return m != 1; });
        return static_order - first_non_trivial;
      }
    }

    //==============================================================================================
    //! @brief Number of elements
    //!
    //! Computes the number of elements storable in current kwk::shape, i.e the product of all
    //! dimensions' size.
    //==============================================================================================
    constexpr std::int32_t numel() const noexcept
    {
      if constexpr(static_order == 0) return 0;
      else return kumi::fold_left([](auto a, auto b){ return a*b; }, *this, std::int32_t{1});
    }

    //==============================================================================================
    //! @brief Check if a shape fit current's shape constraints on size and dimension
    //! @param ref Shape to use as a reference extent
    //! @return `true` if both shape has the same order and if each statically set dimension of
    //!         `ref` is matched with an equal value in the current shape.
    //==============================================================================================
    template<auto R>
    KWK_PURE constexpr bool fit_constraints(shape<R> const& ref) const noexcept
    {
      if      constexpr(shape<R>::is_fully_dynamic                  ) return true;
      else if constexpr(shape<R>::static_order != static_order      ) return false;
      else if constexpr(shape<R>::is_fully_static && is_fully_static) return R.is_compatible(Shape);
      else
      {
        return [&]<std::size_t... N>(std::index_sequence<N...>)
        {
          auto check = []<typename A>(A a,auto b) { return std::integral<A> || a == b; };

          return (true && ... && check(get<N>(ref), get<N>(*this)) );
        }(std::make_index_sequence<static_order>{});
      }
    }

    template<typename T, typename... E>
    constexpr bool is_similar( __::combo<T,E...> const& o) const noexcept
    {
      return descriptor.is_similar(o);
    }

    template<auto S2>
    constexpr bool is_similar( shape<S2> const& ) const noexcept
    {
      return descriptor.is_similar(S2);
    }

    /// Stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, shape const& s)
    {
      os << "[";
      kumi::for_each_index( [&](auto i, auto) { os << " " << get<i>(s); }, s);
      return os << " ]";
    }

    //==============================================================================================
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
    //==============================================================================================
    // has to be defined inline due to (Apple) Clang deficiencies
    // https://github.com/llvm/llvm-project/issues/58952
    // but also after of_size
    //==============================================================================================
    template<typename... Slicers>
    constexpr auto operator()(Slicers const&... s ) const noexcept
    requires( sizeof...(Slicers) <= static_order )
    {
      auto  shd     = compress<sizeof...(s)>(*this);
      auto  sliced  = kumi::map_index ( [&](auto i, auto m) { return reshape(shd,m,i); }
                                      , kumi::tie(s...)
                                      );

      return kumi::apply( [](auto... v) { return of_size(v...); }, sliced );
    }
  };

  /// Deduction guide for @ref kwk::shape
  template<typename... T>
  shape(T...) -> shape< __::descriptor_from<T...>() >;

  //================================================================================================
  //! @brief Compress a kwk::shape to a given order
  //! @tparam N  Expected @ref glossary-order of the generated shape.
  //! @param  s  Original shape to compress
  //! @return A new kwk::shape instance which order has been set to N by compressing dimensions.
  //================================================================================================
  template<std::int32_t N, auto Desc>
  constexpr auto compress(shape<Desc> const& s) noexcept
  {
    using old_t = typename shape<Desc>::parent;
    using new_t = typename shape<compress<N>(Desc)>::parent;

    shape<compress<N>(Desc)> that;
    static_cast<new_t&>(that) = compress<N>(static_cast<old_t const&>(s));
    return that;
  }
}

// Tuple interface adaptation
template<auto Desc>
struct  std::tuple_size<kwk::shape<Desc>>
      : std::integral_constant<std::int32_t,kwk::shape<Desc>::static_order>
{};

template<std::size_t N, auto Desc>
struct  std::tuple_element<N, kwk::shape<Desc>>
{
  using type = typename kwk::shape<Desc>::size_type;
};

#if !defined(_MSC_VER)
#pragma GCC diagnostic pop
#endif
