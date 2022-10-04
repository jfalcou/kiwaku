//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : EVE Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <kwk/detail/prefilled.hpp>
#include <kwk/detail/pp_helpers.hpp>
#include <kwk/detail/extent_builder.hpp>
#include <kwk/settings/extent.hpp>
#include <kwk/utility/joker.hpp>
#include <kwk/utility/fixed.hpp>
#include <kwk/utility/slicer.hpp>
#include <cstddef>

namespace kwk
{
  struct size_;
  template<auto Shape> struct shape;

  template<std::size_t N, auto Desc>
  constexpr auto compress(shape<Desc> const& s) noexcept;

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
  //!   <br/>
  //!
  //! - defining the layout of the kwk::shape and manually initializing it. The description of the
  //!   kwk::shape layout is done with the kwk::extent object or one of the pre-defined layouts.
  //!
  //!   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
  //!   kwk::shape< kwk::extent[5] >      s1;             // Order 1 shape with static size
  //!   kwk::shape< kwk::_2D >            s2(n, m);       // Order 2 shape with dynamic sizes
  //!   kwk::shape< kwk::extent[2][2]( )> s3( _[2] = n);  // Order 3 shape with mixed sizes
  //!   kwk::shape< kwk::extent[2]( )[3]> s4( _, n, _);   // Order 3 shape with mixed sizes
  //!   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //!
  //! @tparam Shaper An instance of a [shape descriptor](@ref eve::extent)
  //================================================================================================
  template<auto Shape>
  struct shape : kwk::detail::prefilled<Shape>
  {
    using parent = kwk::detail::prefilled<Shape>;

    /// Compile-time value for @ref glossary-order
    static constexpr std::ptrdiff_t static_order = parent::static_size;

    /// Type of dimensions' size
    using size_type = typename parent::value_type;

    /// Associated kwk::stride type
    //using stride_type   = unit_stride<size_type, static_order>;

    /// Indicates that the shape has at least one dimension specified at runtime
    static constexpr bool is_dynamic        = parent::is_dynamic;

    /// Indicates that the shape's dimensions are all specified at runtime
    static constexpr bool is_fully_dynamic  = parent::dynamic_size == static_order;

    /// Indicates that the shape's dimensions are all specified compile-time
    static constexpr bool is_fully_static   = parent::is_fully_static;

    // shape is its self option keyword
    using stored_value_type = shape<Shape>;
    using keyword_type      = size_;

    KWK_FORCEINLINE constexpr auto operator()(keyword_type const&) const noexcept { return *this; }

    //==============================================================================================
    //! @brief Constructs a default @ref kwk::shape equals to [0 1 ... 1]
    //==============================================================================================
    shape() : parent([](auto i, auto) -> size_type { return i > 0;}) {}

    //==============================================================================================
    //! @brief Constructor from set of dimensions
    //!
    //! Constructs a kwk::shape with a variadic list of dimension values.
    //! Values can either be any integral value, any fixed integral value or kwk::_.
    //!
    //! If you pass kwk::_ as a dimension's value, it means that the shape will be using the
    //! default value for this dimension, i.e 0 for the first dimension, 1 for others or the fixed
    //! size if it is provided.
    //!
    //! Passing a runtime dimension size where a static size is provided is undefined behavior if
    //! both values are not equal.
    //!
    //! This constructor will not take part in overload resolution if the number of values exceed
    //! shape's order or if any value is neither convertible to kwk::shape::size_type nor kwk::_.
    //!
    //! @param  s Variadic list of dimensions' values
    //==============================================================================================
    constexpr shape(concepts::extent<size_type> auto... vals) noexcept
    requires( sizeof...(vals) <= static_order )
    : shape{}
    {
      parent::fill(vals...);
    }

    //==============================================================================================
    //! @brief Construct from a subset of runtime dimension values
    //!
    //! This constructor takes a variadic list of arguments specifying the size specified for a
    //! given runtime dimension. Those sizes are passed by using `kwk::_[i]` as a dimension value.
    //!
    //! Passing a dimension specifier to overwrite one of the compile-time dimensions is undefined
    //! behavior.
    //!
    //! This constructor will not take part in overload resolution if the shape is fully static.
    //!
    //! @groupheader{Example}
    //! @godbolt{docs/shape/mixed.cpp}
    //!
    //! @param s  Variadic list of dimension/size association
    //==============================================================================================
    constexpr shape(std::same_as<axis> auto... dims) noexcept
    requires( (sizeof...(dims) <= static_order) && !is_fully_static )
    : shape{}
    {
      auto& st = parent::storage();

      // Fill the proper axis value with the corresponding size
      kumi::for_each( [&](auto e)
                      {
                        KIWAKU_ASSERT ( parent::contains(e.dims)
                                      , "[kwk] - Semi-dynamic construction overwrite static shape"
                                      );

                        st[parent::location[e.dims]] = static_cast<size_type>(e.size);
                      }
                    , kumi::tie(dims...)
                    );
    }

    //==============================================================================================
    //! @brief Constructs from a shape with compatible layout
    //!
    //! Constructs a kwk::shape from another one with compatible layout but potentially different
    //! size_type.
    //!
    //! Two kwk::shapes have compatible layout if they have the same @ref glossary-order and if each
    //! dimension of the constructed @ref kwk::shape can hold its equivalent from the source shape,
    //! i.e it's runtime specified or, if it's compile-time specified, has the same value than its
    //! source.
    //!
    //! This constructor doesn not participate in overload resolution if shape's orders are not equal.
    //!
    //! @groupheader{Example}
    //! @godbolt{docs/shape/compatible.cpp}
    //!
    //! @param other  Source kwk::shape to copy
    //==============================================================================================
    template<auto OtherShape>
    constexpr shape( shape<OtherShape> const& other ) noexcept
    requires(shape<OtherShape>::static_size == static_order && Shape.is_compatible(OtherShape))
    {
      auto& v = parent::storage();
      kumi::for_each_index( [&]<typename I>(I, auto const& m)
                            {
                              constexpr auto i = I::value;
                              if constexpr(parent::contains(i)) v[parent::location[i]] = m;
                              else
                              {
                                KIWAKU_ASSERT( m == get<i>(Shape)
                                            , "[kwk] - Static/Dynamic mismatch in constructor"
                                            );
                              }
                            }
                          , other
                          );
    }

    //==============================================================================================
    //! @brief Constructs from a shape with a lower order
    //!
    //! Constructs a kwk::shape from a shape with higher orders, filling the missing sizes with 1
    //!
    //! This constructor does not participate in overload resolution of the shape is not fully
    //! specified at runtime.
    //!
    //! @groupheader{Example}
    //! @godbolt{docs/shape/odd_sized.cpp}
    //!
    //! @param other  Shape to copy
    //==============================================================================================
    template<auto OtherShaper>
    constexpr explicit shape( shape<OtherShaper> const& other ) noexcept
              requires( shape<OtherShaper>::static_order < static_order && is_fully_dynamic)
    {
      constexpr auto dz = std::min(shape<OtherShaper>::static_order,static_order);
      for(std::size_t i = 0;  i < dz;++i)           parent::storage()[i] = other[i];
      for(std::size_t i = dz; i < static_order;++i) parent::storage()[i] = 1;
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
    //! @godbolt{docs/shape/odd_sized.cpp}
    //!
    //! @param other  Shape to copy
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
    requires( shape<Other>::static_order < static_order || Shape.is_compatible(Other) )
    constexpr shape& operator=( shape<Other> const& other ) noexcept
    {
      shape that(other);
      swap(that);
      return *this;
    }

    /// Swap shapes' contents
    friend void swap( shape& x, shape& y ) noexcept { x.swap(y); }
    using parent::swap;

    /// Equality comparison operator
    template<auto Shaper2>
    constexpr bool operator==( shape<Shaper2> const& other) const noexcept
    {
      return compare( other
                    , [](size_type a, size_type b) { return a == b; }
                    , [](auto a) { return a == 1; }
                    );
    }

    /// Inequality comparison operator
    template<auto Shaper2>
    constexpr bool operator!=( shape<Shaper2> const& other) const noexcept
    {
      return compare( other
                    , [](size_type a, size_type b) { return a != b; }
                    , [](auto a) { return a == 1; }
                    );
    }

    //==============================================================================================
    // Check if current shape contains (maybe strictly) all the shape of another one
    //==============================================================================================
    template<auto Shaper2>
    constexpr bool contains( shape<Shaper2> const& other) const noexcept
    {
      return compare( other , [](auto a, auto b) { return a>=b;}, [](auto) { return true; } );
    }

    template<auto Shaper2>
    constexpr bool strictly_contains( shape<Shaper2> const& other) const noexcept
    {
      return compare( other , [](auto a, auto b) { return a>b;}, [](auto) { return true; } );
    }

    /// Number of dimensions
    static constexpr auto order() noexcept { return static_order; }

    //==============================================================================================
    //! @brief Number of non-trivial dimensions
    //!
    //! Computes the number of non-trivial dimensions, i.e dimension with size equals to 1 and that
    //! doesn't participate to the shape's extent.
    //==============================================================================================
    constexpr std::ptrdiff_t nbdims() const noexcept
    {
      if constexpr(static_order == 0)  return 0;
      else
      {
        std::ptrdiff_t r = get<0>(*this) == 1 ? 0 : 1;
        kumi::for_each_index( [&r](std::ptrdiff_t i, auto m) { r = std::max(r, m == 1 ? 0 : i+1); }
                            , *this
                            );
        return r;
      }
    }

    //==============================================================================================
    //! @brief Number of elements
    //!
    //! Computes the number of elements storable in current kwk::shape, i.e the product of all
    //! dimensions' size.
    //==============================================================================================
    constexpr std::ptrdiff_t numel() const noexcept
    {
      if constexpr(static_order == 0) return 0LL;
      else return kumi::fold_left([](auto a, auto b){ return a*b; }, *this, std::ptrdiff_t{1});
    }

    /// Stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, shape const& s)
    {
      os << "[";
      kumi::for_each( [&](auto e) { os << " " << +e; }, s);
      return os << " ]";
    }

    /// Slicing interface - TODO DOC
    template<typename... Slicers>
    inline constexpr auto operator()(Slicers const&... s) const noexcept
    requires( sizeof...(Slicers) <= static_order);

    protected:
    template<auto S2, typename Comp, typename Check>
    constexpr bool compare( shape<S2> const& o, Comp const& op, Check const& check) const noexcept
    {
      constexpr auto o_size = shape<S2>::static_order;

      if constexpr( static_order == o_size )
      {
        return [&]<std::size_t... I>(std::index_sequence<I...> const&)
        {
          return (true && ... && op(get<I>(*this),get<I>(o)) ) ;
        }(std::make_index_sequence<static_order>());
      }
      else
      {
        constexpr auto d = std::min(static_order, o_size);

        // Compute equality over common slice of shape
        bool result = true;
        [&]<std::size_t... I>(std::index_sequence<I...> const&)
        {
          result = (result && ... && op(get<I>(*this),get<I>(o)) );
        }(std::make_index_sequence<d>());

        // Check that we have 1s everywhere in the other parts
        if constexpr( static_order < o_size )
        {
          constexpr auto sz = o_size - static_order;
          return [&]<std::size_t... I>(std::index_sequence<I...> const&)
          {
            return (result && ... && (get<static_order+I>(o) == 1));
          }(std::make_index_sequence<sz>());
        }
        else if constexpr( static_order > o_size )
        {
          constexpr auto sz = static_order - o_size;
          return [&]<std::size_t... I>(std::index_sequence<I...> const&)
          {
            return (result && ... && check(get<o_size+I>(*this)));
          }(std::make_index_sequence<sz>());
        }
      }
    }
  };

  /// Deduction guide for @ref kwk::shape
  template<concepts::extent<std::ptrdiff_t>... T> shape(T...) -> shape< _nD<sizeof...(T)> >;

  //================================================================================================
  //! @brief Compress a kwk::shape to a given order
  //! @tparam N  Expected @ref glossary-order of the generated shape.
  //! @param  s  Original shape to compress
  //! @return A new kwk::shape instance which order has been set to N by compressing dimensions.
  //================================================================================================
  template<std::size_t N, auto Desc>
  constexpr auto compress(shape<Desc> const& s) noexcept
  {
    using old_t = typename shape<Desc>::parent;
    using new_t = typename shape<compress<N>(Desc)>::parent;

    shape<compress<N>(Desc)> that;
    static_cast<new_t&>(that) = compress<N>(static_cast<old_t const&>(s));
    return that;
  }

  //================================================================================================
  //! @brief Generates a kwk::shape from a list of sizes
  //! @tparam SizeType  Integral type used to store sizes. If unspecified, `std::ptrdiff_t` is used.
  //! @param  ds        Variadic pack of sizes
  //================================================================================================
  template<typename SizeType, int..., concepts::extent<SizeType>... Ds>
  constexpr auto of_size(Ds... ds) noexcept
  {
    return detail::make_extent<kwk::shape,SizeType>(ds...);
  }

  template<int..., concepts::extent<std::ptrdiff_t>... Ds>
  constexpr auto of_size( Ds... ds) noexcept
  {
    using type_t = typename detail::largest_type<detail::to_int_t<Ds>...>::type;
    return of_size<type_t>(ds...);
  }

  template<int..., kumi::product_type Ds>
  constexpr auto of_size( Ds ds) noexcept
  {
    return kumi::apply([](auto... s) { return of_size(s...); }, ds);
  }

  template<typename SizeType,int..., kumi::product_type Ds>
  constexpr auto of_size( Ds ds) noexcept
  {
    return kumi::apply([](auto... s) { return of_size<SizeType>(s...); }, ds);
  }

  // Implementation of slicing interface
  template<auto Shape>
  template<typename... Slicers>
  inline constexpr auto shape<Shape>::operator()(Slicers const&... s) const noexcept
  requires( sizeof...(Slicers) <= static_order)
  {
    auto  shd     = compress<sizeof...(s)>(*this);
    auto  sliced  = kumi::map_index ( [&](auto i, auto m) { return slice_dim(shd,m,i); }
                                    , kumi::tie(s...)
                                    );

    return kumi::apply( [](auto... v) { return of_size(v...); }, sliced );
  }
}

// Tuple interface adaptation
template<auto Desc>
struct  std::tuple_size<kwk::shape<Desc>>
      : std::integral_constant<std::size_t,kwk::shape<Desc>::static_order>
{};

template<std::size_t N, auto Desc>
struct  std::tuple_element<N, kwk::shape<Desc>>
{
  using type = typename kwk::shape<Desc>::size_type;
};
