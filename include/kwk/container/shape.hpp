//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/container/slicers.hpp>
#include <kwk/container/stride.hpp>
#include <kwk/detail/assert.hpp>
#include <kwk/detail/joker.hpp>
#include <kwk/detail/ct_helpers.hpp>
#include <kwk/detail/kumi.hpp>
#include <kwk/detail/prefilled_array.hpp>
#include <kwk/options/extent.hpp>
#include <kwk/options/fixed.hpp>

#include <array>
#include <cstddef>
#include <ostream>
#include <utility>

namespace kwk
{
  struct size_;

  //================================================================================================
  //! @ingroup containers
  //! @brief  Fixed order shape with mixed size capability
  //!
  //! <hr/>
  //! **Required header**:
  //! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
  //!  #include<kwk/container/shape.hpp>
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
  template<auto Shaper>
  struct shape : detail::prefilled_array<Shaper>
  {
    using parent    = detail::prefilled_array<Shaper>;

    /// Compile-time value for @ref glossary-order
    static constexpr std::ptrdiff_t static_order = parent::static_size;

    /// Type of dimensions' size
    using size_type     = typename parent::size_type;

    /// Associated kwk::stride type
    using stride_type   = unit_stride<size_type, static_order>;

    /// Indicates that the shape has at least one dimension specified at runtime
    static constexpr bool is_dynamic        = parent::storage_size >= 1;

    /// Indicates that the shape's dimensions are all specified at runtime
    static constexpr bool is_fully_dynamic  = parent::storage_size == static_order;

    /// Indicates that the shape's dimensions are all specified compile-time
    static constexpr bool is_fully_static   = parent::storage_size == 0;

    // shape is its self option keyword
    using stored_value_type = shape<Shaper>;
    using keyword_type      = size_;

    KWK_FORCEINLINE constexpr auto operator()(keyword_type const&) const noexcept { return *this; }

    /// @brief Constructs a default kwk::shape equals to [0 1 ... 1]
    KWK_FORCEINLINE constexpr shape() noexcept
    {
      if constexpr(parent::storage_size > 1ULL)  for(auto& e : storage()) e = 1;
      if constexpr(parent::storage_size > 0ULL)  storage()[0] = 0;
    }

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
    template<typename... T>
    constexpr shape(T... s) noexcept
    requires(  (sizeof...(T) <= static_order)
            && ((std::same_as<T,detail::value_joker> || std::is_convertible_v<T,size_type>) && ...)
            )
    {
      auto& st = storage();

      // Fill storage data with provided size
      [&]<std::size_t... I>(std::index_sequence<I...> const&)
      {
        [[maybe_unused]] auto proc = [&]<typename N, typename X>(X const& sz, N idx)
        {
          if constexpr( std::is_convertible_v<X,size_type> )
          {
            if constexpr(!parent::size_map::contains(N::value))
            {
              st[parent::size_map::template locate<static_order>(idx)] = sz;
            }
            else
              KIWAKU_ASSERT ( sz == this->template get<N::value>()
                            , "[kwk::shape] - Runtime/Compile-time mismatch in constructor"
                            );
          }
          else
          {
            if constexpr(!parent::size_map::contains(N::value))
            {
              if constexpr(!N::value) st[parent::size_map::template locate<static_order>(idx)] = 0;
              else                    st[parent::size_map::template locate<static_order>(idx)] = 1;
            }
          }
        };
        (proc(s, kumi::index<I>),...);
      }(std::index_sequence_for<T...>{});

      // Complete the rest with 0/1 if needed
      [&]<std::size_t... I>(std::index_sequence<I...> const&)
      {
        [[maybe_unused]] auto proc = [&]<typename N>(N)
        {
          constexpr auto M = sizeof...(T) + N::value;
          if constexpr(!parent::size_map::contains(M))
          {
            if constexpr(M==0) st[parent::size_map::template locate<static_order>(M)] = 0;
            else               st[parent::size_map::template locate<static_order>(M)] = 1;
          }
        };
        (proc(kumi::index<I>),...);
      }(std::make_index_sequence<static_order - sizeof...(T)>{});
    }

    //==============================================================================================
    //! @brief Construct from a subset of runtime dimension values
    //!
    //! This constructor takes a variadic list of arguments specifying the size specified for a
    //! given runtime dimension. Those sizes are passed by using the `kwk::_` dimension specifier.
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
    template<std::same_as<detail::axis>... Extent>
    constexpr shape(Extent... s) noexcept
    requires( (sizeof...(Extent) <= static_order) && !is_fully_static )
    {
      auto& st = storage();

      // Default fillings
      detail::constexpr_for<static_order>
      ( [&]<std::ptrdiff_t I>(std::integral_constant<std::ptrdiff_t,I> const&)
        {
          if constexpr(!parent::size_map::contains(I))
          {
            constexpr auto ii = parent::size_map::template locate<static_order>(I);
            if constexpr(I==0)  st[ii] = 0; else st[ii] = 1;
          }
        }
      );

      // Fill the proper axis value with the corresponding size
      auto const fill = [&](auto e)
      {
        KIWAKU_ASSERT ( !parent::size_map::contains(e.dims)
                      , "[kwk::shape] Semi-dynamic construction overwrite static shape"
                      );

        auto ii = parent::size_map::template locate<static_order>(e.dims);
        st[ii] = static_cast<size_type>(e.size);
      };
      (fill(s),...);
    }

    //==============================================================================================
    //! @brief Constructs from a shape with compatible layout
    //!
    //! Constructs a kwk::shape from another one with compatible layout but potentially different
    //! size_type.
    //!
    //! Two kwk::shapes have compatible layout if they have the same @ref glossary-order and if each
    //! dimension of the constructed kwk::shape can hold its equivalent from the source shape, i.e
    //! it's runtime specified or, if it's compile-time specified, has the same value than its source.
    //!
    //! This constructor doesn not participate in overload resolution if shape's orders are not equal.
    //!
    //! @groupheader{Example}
    //! @godbolt{docs/shape/compatible.cpp}
    //!
    //! @param other  Source kwk::shape to copy
    //==============================================================================================
    template<auto OtherShaper>
    constexpr shape( shape<OtherShaper> const& other ) noexcept
    requires( OtherShaper.size() == Shaper.size() && Shaper.is_compatible(OtherShaper))
    {
      detail::constexpr_for<order()>
      ( [&]<std::ptrdiff_t I>(std::integral_constant<std::ptrdiff_t,I> const&)
        {
          constexpr auto ii = parent::size_map::template locate<static_order>(I);
          if constexpr( ii < parent::storage_size ) storage()[ii] = other.template get<I>();
        }
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
    constexpr shape( shape<OtherShaper> const& other ) noexcept
              requires( OtherShaper.size() < static_order && is_fully_dynamic)
    {
      constexpr auto dz = std::min(OtherShaper.size(),static_order);

      detail::constexpr_for<dz>
      ( [&]<std::ptrdiff_t I>(std::integral_constant<std::ptrdiff_t,I> const&)
        {
          storage()[I] = other.template get<I>();
        }
      );

      for(std::size_t i = dz; i < static_order;++i) storage()[i] = 1;
    }

    //==============================================================================================
    //! @brief Constructs from a shape with a higher order
    //!
    //! Constructs a kwk::shape from a shape with higher orders, compacting the extra-dimensions into
    //! the last.
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
    constexpr explicit  shape( shape<OtherShaper> const& o ) noexcept
                        requires( OtherShaper.size() > static_order && is_fully_dynamic)
    {
      constexpr auto dz = std::min(OtherShaper.size(),static_order);
      constexpr auto oz = shape<OtherShaper>::static_order - dz;
      detail::constexpr_for<dz> ( [&]<typename I>(I)
                                { storage()[I{}] = o.template get<I::value>(); }
                                );
      detail::constexpr_for<oz> ( [&]<typename I>(I)
                                { storage().back() *= o.template get<dz+I::value>(); }
                                );
    }

    /// Number of dimensions
    static constexpr auto order() noexcept { return static_order; }

    /// Assignment operator
    template<auto OtherShaper>
    requires( OtherShaper.size() < static_order || Shaper.is_compatible(OtherShaper) )
    constexpr shape& operator=( shape<OtherShaper> const& other ) noexcept
    {
      shape that(other);
      swap(that);
      return *this;
    }

    // Elements access
    using parent::get;

    constexpr auto operator[](std::size_t i) const noexcept
    {
      KIWAKU_ASSERT ( (i>=0 && i<static_order)
                    , "[kwk::shape::operator[] Out of bounds access"
                    );
      return parent::operator[](i);
    }

    constexpr auto& operator[](std::size_t i) noexcept requires( is_dynamic && static_order>0)
    {
      KIWAKU_ASSERT ( (i>=0 && i<static_order)
                    , "[kwk::shape::operator[] Out of bounds access"
                    );

      KIWAKU_ASSERT ( !parent::size_map::contains(i)
                    , "[kwk::shape::operator[] Access overwrites of compile-time dimension"
                    );
      return parent::operator[](i);
    }

    using parent::swap;

    /// Swap shapes' contents
    friend void swap( shape& x,shape& y ) noexcept { x.swap(y); }

/*
    // Reshaping interface using slicers
    template<typename... Slices> constexpr auto operator()(Slices... slices) const noexcept
    {
      using that_t = shape<sizeof...(Slices)>;
      auto rs = [&]<std::size_t... I>(std::index_sequence<I...> const&, auto const& s)
      {
        return that_t{ detail::reshape(slices,I,s) ... };
      };

      that_t that{*this};
      that = rs(std::make_index_sequence<sizeof...(Slices)>{}, that);

      return that;
    }
*/

    //==============================================================================================
    //! @brief Number of non-trivial dimensions
    //!
    //! Computes the number of non-trivial dimensions, i.e dimension with size equals to 1 and that
    //! doesn't participate to the shape's extent.
    //==============================================================================================
    constexpr int nbdims() const noexcept
    {
      if constexpr(static_order == 0)  return 0;
      else  return [&]<std::size_t...I>( std::index_sequence<I...> const&)
      {
        size_type m = this->template get<0>() == 1 ? 0 : 1;
        ((m = std::max(m, size_type(this->template get<I>() == 1 ? 0 : 1+I))),...);
        return m;
      }(std::make_index_sequence<static_order>());
    }

    //==============================================================================================
    //! @brief Number of elements
    //!
    //! Computes the number of elements storable in current kwk::shape, i.e the product of all
    //! dimensions' size.
    //==============================================================================================
    constexpr auto numel() const noexcept
    {
      if constexpr(static_order == 0) return 0;
      else return [&]<std::size_t...I>( std::index_sequence<I...> const&)
          {
            return (size_type{1} * ... * this->template get<I>() );
          }(std::make_index_sequence<static_order>());
    }

    /// Conversion to kwk::stride
    constexpr auto as_stride() const requires(static_order > 0) { return stride_type(*this); }


    /// Equality comparison operator
    template<auto Shaper2>
    constexpr bool operator==( shape<Shaper2> const& other) const noexcept
    {
      return compare( other , [](auto a, auto b) { return a == b; }, [](auto a) { return a == 1; } );
    }

    /// Inequality comparison operator
    template<auto Shaper2>
    constexpr bool operator!=( shape<Shaper2> const& other) const noexcept
    {
      return compare( other , [](auto a, auto b) { return a != b; },[](auto a) { return a == 1; } );
    }

    using parent::as_array;

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

    /// Stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, shape const& s)
    {
      os << "[";

      detail::constexpr_for<static_order>
      ( [&]<std::ptrdiff_t I>(std::integral_constant<std::ptrdiff_t,I> const&)
        {
          os << " " << +s.template get<I>();
        }
      );

      os << " ]";
      return os;
    }

    private:
    constexpr auto&        storage() noexcept       { return parent::storage(); }
    constexpr auto const&  storage() const noexcept { return parent::storage(); }

    template<auto S2, typename Comp, typename Check>
    constexpr bool compare( shape<S2> const& o, Comp const& op, Check const& check) const noexcept
    {
      constexpr auto o_size = shape<S2>::static_order;

      if constexpr( static_order == o_size )
      {
        return [&]<std::size_t... I>(std::index_sequence<I...> const&)
        {
          return (true && ... && op(this->template get<I>(),o.template get<I>()) ) ;
        }(std::make_index_sequence<static_order>());
      }
      else
      {
        constexpr auto d = std::min(static_order, o_size);

        // Compute equality over common slice of shape
        bool result = true;
        [&]<std::size_t... I>(std::index_sequence<I...> const&)
        {
          result = (result && ... && op(this->template get<I>(),o.template get<I>()) );
        }(std::make_index_sequence<d>());

        // Check that we have 1s everywhere in the other parts
        if constexpr( static_order < o_size )
        {
          constexpr auto sz = o_size - static_order;
          return [&]<std::size_t... I>(std::index_sequence<I...> const&)
          {
            return (result && ... && (o.template get<static_order+I>() == 1));
          }(std::make_index_sequence<sz>());
        }
        else if constexpr( static_order > o_size )
        {
          constexpr auto sz = static_order - o_size;
          return [&]<std::size_t... I>(std::index_sequence<I...> const&)
          {
            return (result && ... && check(this->template get<o_size+I>()));
          }(std::make_index_sequence<sz>());
        }
      }
    }
  };
}

// Shape supports structured bindings.
template<std::size_t I, auto Shaper>
struct  std::tuple_element<I, kwk::shape<Shaper>>
      : std::tuple_element<I, typename kwk::shape<Shaper>::parent>
{};

template<auto Shaper>
struct  std::tuple_size<kwk::shape<Shaper>>
      : std::tuple_size<typename kwk::shape<Shaper>::parent>
{};

namespace kwk
{
  // Product-type interface
  template<std::size_t I, auto Shaper>
  constexpr auto get(shape<Shaper> const& s) noexcept { return s.template get<I>(); }

  template<std::size_t I, auto Shaper>
  constexpr decltype(auto) get(shape<Shaper>& s) noexcept { return s.template get<I>(); }

  // Deduction guides
  template<std::integral... T> shape(T... s) -> shape< _nD<sizeof...(T)> >;

  // Shape generators
  namespace detail
  {
    // Convert a sequence of dimensions to the proper shaper type
    template<typename SizeType,typename ... Ds>
    constexpr auto as_shape(Ds... ds) noexcept
    {
      return kumi::fold_right( []<typename T>(auto a, T)
                              {
                                if constexpr( requires{ T::value; } ) return a[T::value];
                                else                                  return a();
                              }
                            , kumi::tuple{ds...}
                            , detail::hybrid_sequence<SizeType>{}
                            );
    }
  }

  //================================================================================================
  //! @brief Generates a kwk::shape from a list of sizes
  //! @tparam SizeType  Integral type used to store sizes. If unspecified, `std::ptrdiff_t` is used.
  //! @param  ds        Variadic pack of sizes
  //================================================================================================
  template<typename SizeType, int..., typename... Ds>
  constexpr auto of_size(Ds... ds) noexcept
  {
    return kumi::apply( [](auto... v)
                        {
                          return  kwk::shape<detail::as_shape<SizeType>(Ds{}...)>(v...);
                        }
                      , kumi::tuple{ds...}
                      );
  }

  template<int..., typename... Ds> constexpr auto of_size( Ds... ds) noexcept
  {
    using type_t = typename detail::largest_type<detail::to_int_t<Ds>...>::type;
    return of_size<type_t>(ds...);
  }

  template<int..., kumi::product_type Ds> constexpr auto of_size( Ds ds) noexcept
  {
    return kumi::apply([](auto... s) { return of_size(s...); }, ds);
  }

  template<typename SizeType,int..., kumi::product_type Ds> constexpr auto of_size( Ds ds) noexcept
  {
    return kumi::apply([](auto... s) { return of_size<SizeType>(s...); }, ds);
  }
}
