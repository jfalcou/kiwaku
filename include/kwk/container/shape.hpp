//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#pragma once

#include <kwk/container/slicers/axis.hpp>
#include <kwk/container/stride.hpp>
#include <kwk/options/fixed.hpp>
#include <kwk/detail/ct_helpers.hpp>
#include <kwk/detail/assert.hpp>
#include <kwk/detail/shaper.hpp>
#include <kwk/detail/kumi.hpp>
#include <utility>
#include <cstddef>
#include <ostream>
#include <array>

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
  //! - defining the layout of the kwk::shape and manually intializing it. The description of the
  //!   kwk::shape layout is done with the kwk::extent object or one of the pre-defined layouts.
  //!
  //!   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
  //!   kwk::shape< kwk::extent[5] >      s1;             // Order 1 shape with static size
  //!   kwk::shape< kwk::_2D >            s2(n, m);       // Order 2 shape with dynamic sizes
  //!   kwk::shape< kwk::extent[2][2]() > s3( _[2] = n);  // Order 3 shape with mixed sizes
  //!   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //!
  //! For most usage, kwk::shape is to be used as a whole. Acces to individual size along a given
  //! dimension is possible via a tuple-like access API based on compile-time indexes.
  //!
  //! @tparam Shaper An instance of an shape descriptor
  //================================================================================================
  template<auto Shaper> struct shape
  {
    using size_map      = decltype(Shaper.size_map());

    /// @ref glossary-order of the eve::shape
    static constexpr std::ptrdiff_t static_order = Shaper.size();

    /// Type of dimensions' size
    using size_type     = typename decltype(Shaper)::size_type;

    struct empty_storage {};
    static constexpr std::ptrdiff_t storage_size  = static_order - size_map::size;
    using storage_t     = std::conditional_t< (storage_size!=0)
                                            , std::array<size_type,storage_size>
                                            , empty_storage
                                            >;

    /// Associated kwk::stride type
    using stride_type   = unit_stride<size_type, static_order>;

    /// Indicates that the shape has at least one dimension specified at runtime
    static constexpr bool is_dynamic        = storage_size >= 1;

    /// Indicates that the shape's dimensions are all specified at runtime
    static constexpr bool is_fully_dynamic  = storage_size == static_order;

    /// Indicates that the shape's dimensions are all specified compile-time
    static constexpr bool is_fully_static   = storage_size == 0;

    //==============================================================================================
    // shape is its self option keyword
    //==============================================================================================
    using stored_value_type = shape<Shaper>;
    using keyword_type      = size_;

    KWK_FORCEINLINE constexpr auto operator()(keyword_type const&) const noexcept { return *this; }

    //==============================================================================================
    //! @brief Default constructor
    //!
    //! A default-constructed kwk::shape contains 0 element on its innermost dimension.
    //! All other dimensions are set to 1.
    //==============================================================================================
    KWK_FORCEINLINE constexpr shape() noexcept
    {
      if constexpr(storage_size > 1ULL)  for(auto& e : storage_) e = 1;
      if constexpr(storage_size > 0ULL)  storage_[0] = 0;
    }

    //==============================================================================================
    //! @brief Constructor from set of integral values
    //!
    //! Initializes current kwk::shape with a variadic list of integral values.
    //!
    //! This constructor will not take part in overload resolution if the number of values exceed
    //! shape's order, if the shape is not fully dynamic or if any value is not convertible
    //! to kwk::shape::size_type.
    //!
    //! @param  s Variadic list of dimensions' size
    //==============================================================================================
    template<typename... T>
    constexpr shape(T... s) noexcept
    requires  (   (std::is_convertible_v<T,size_type> && ...)
              &&  (sizeof...(T) <= static_order)
              &&  is_fully_dynamic
              )
            : storage_{ static_cast<size_type>(s)... }
    {
      if constexpr(sizeof...(T) < static_order)
        for(std::size_t i = sizeof...(T);i<static_order;++i)
          storage_[i] = 1;
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
      // Default fillings
      detail::constexpr_for<static_order>
      ( [&]<std::ptrdiff_t I>(std::integral_constant<std::ptrdiff_t,I> const&)
        {
          if constexpr(!size_map::contains(I))
          {
            if constexpr(I==0)  storage_[size_map::template locate<static_order>(I)] = 0;
            else                storage_[size_map::template locate<static_order>(I)] = 1;
          }
        }
      );

      // Fill the proper axis value with the corresponding size
      auto const fill = [&](auto e)
      {
        KIWAKU_ASSERT ( !size_map::contains(e.dims)
                      , "[kwk::shape] Semi-dynamic construction overwrite static shape"
                      );

        storage_[size_map::template locate<static_order>(e.dims)] = static_cast<size_type>(e.size);
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
          constexpr auto idx = size_map::template locate<static_order>(I);
          if constexpr( idx < storage_size ) storage_[idx] = other.template get<I>();
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
          storage_[I] = other.template get<I>();
        }
      );

      for(std::size_t i = dz; i < static_order;++i) storage_[i] = 1;
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
    constexpr explicit  shape( shape<OtherShaper> const& other ) noexcept
                        requires( OtherShaper.size() > static_order && is_fully_dynamic)
    {
      constexpr auto dz = std::min(OtherShaper.size(),static_order);

      detail::constexpr_for<dz>
      ( [&]<std::ptrdiff_t I>(std::integral_constant<std::ptrdiff_t,I> const&)
        {
          storage_[I] = other.template get<I>();
        }
      );

      detail::constexpr_for<shape<OtherShaper>::static_order - dz>
      ( [&]<std::ptrdiff_t I>(std::integral_constant<std::ptrdiff_t,I> const&)
        {
          storage_.back() *= other.template get<dz+I>();
        }
      );
    }

    /// Number of dimensions
    static constexpr auto order() noexcept { return static_order; }

    /// Assignment operators
    template<auto OtherShaper>
    constexpr shape& operator=( shape<OtherShaper> const& other ) noexcept
    requires( OtherShaper.size() < static_order || Shaper.is_compatible(OtherShaper) )
    {
      shape that(other);
      swap(that);
      return *this;
    }

    //==============================================================================================
    // Element access
    //==============================================================================================
    template<std::size_t I> constexpr auto get() const noexcept
    {
      if constexpr(size_map::contains(I)) return Shaper.at(I);
      else return storage_[size_map::template locate<static_order>(I)];
    }

    /// Swap shape's contents
    void swap( shape& other ) noexcept
    {
      storage_.swap( other.storage_ );
    }

/*
    //==============================================================================================
    // Reshaping interface using slicers
    //==============================================================================================
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
    /**
      @brief Number of non-trivial dimensions

      Computes the number of non-trivial dimensions, i.e dimension with size equals to 1 and that
      doesn't participate to the shape's extent.
    **/
    //==============================================================================================
    constexpr int nbdims() const noexcept
    {
      if constexpr(static_order == 0)  return 0;
      else  return [&]<std::size_t...I>( std::index_sequence<I...> const&)
      {
        size_type m = this->get<0>() == 1 ? 0 : 1;
        ((m = std::max(m, size_type(this->get<I>() == 1 ? 0 : 1+I))),...);
        return m;
      }(std::make_index_sequence<static_order>());
    }

    //==============================================================================================
    /**
      @brief Number of elements

      Computes the number of elements storable in current kwk::shape, i.e the product of all
      dimensions' size.
    **/
    //==============================================================================================
    constexpr auto numel() const noexcept
    {
      if constexpr(static_order == 0) return 0;
      else return [&]<std::size_t...I>( std::index_sequence<I...> const&)
          {
            return (size_type{1} * ... * this->get<I>());
          }(std::make_index_sequence<static_order>());
    }

    /// Conversion to kwk::stride
    constexpr auto as_stride() const requires(static_order > 0) { return stride_type(*this); }

    //==============================================================================================
    // Comparisons
    //==============================================================================================
    template<auto Shaper2>
    constexpr bool operator==( shape<Shaper2> const& other) const noexcept
    {
      return compare( other , [](auto a, auto b) { return a == b; }, [](auto a) { return a == 1; } );
    }

    template<auto Shaper2>
    constexpr bool operator!=( shape<Shaper2> const& other) const noexcept
    {
      return compare( other , [](auto a, auto b) { return a != b; },[](auto a) { return a == 1; } );
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

    storage_t storage_;

    private:

    template<auto Shaper2, typename Comp, typename Check>
    constexpr bool compare( shape<Shaper2> const& other
                          , Comp const& comp, Check const& check
                          ) const noexcept
    {
      constexpr auto other_size = shape<Shaper2>::static_order;

      if constexpr( static_order == other_size )
      {
        bool result = true;
        return [&]<std::size_t... I>(std::index_sequence<I...> const&)
        {
          return (result && ... && comp(this->get<I>(),other.template get<I>()) ) ;
        }(std::make_index_sequence<static_order>());
      }
      else
      {
        constexpr auto d = std::min(static_order, other_size);

        // Compute equality over common slice of shape
        bool result = true;
        [&]<std::size_t... I>(std::index_sequence<I...> const&)
        {
          result = (result && ... && comp(this->get<I>(),other.template get<I>()) );
        }(std::make_index_sequence<d>());

        // Check that we have 1s everywhere in the other parts
        if constexpr( static_order < other_size )
        {
          constexpr auto sz = other_size - static_order;
          return [&]<std::size_t... I>(std::index_sequence<I...> const&)
          {
            return (result && ... && (other.template get<static_order+I>() == 1));
          }(std::make_index_sequence<sz>());
        }
        else if constexpr( static_order > other_size )
        {
          constexpr auto sz = static_order - other_size;
          return [&]<std::size_t... I>(std::index_sequence<I...> const&)
          {
            return (result && ... && check(this->template get<other_size+I>()));
          }(std::make_index_sequence<sz>());
        }
      }
    }
  };
}

//==================================================================================================
// Shape supports structured bindings.
//==================================================================================================
namespace kwk
{
  template<std::size_t I, auto Shaper>
  constexpr auto get(shape<Shaper> const& s) noexcept { return s.template get<I>(); }
}

namespace kumi
{
  template<auto Shaper>
  struct is_product_type<kwk::shape<Shaper>> : std::true_type {};
}

namespace std
{
  template<std::size_t I, auto Shaper>
  struct tuple_element<I, kwk::shape<Shaper>>
  {
    using type = typename kwk::shape<Shaper>::size_type;
  };

  template<auto Shaper>
  struct  tuple_size<kwk::shape<Shaper>>
        : std::integral_constant<std::size_t,kwk::shape<Shaper>::static_order>
  {
  };
}

namespace kwk
{
  //================================================================================================
  // Deduction guides
  //================================================================================================
  template<std::integral... T> shape(T... s) -> shape< _nD<sizeof...(T)> >;

  //================================================================================================
  // Shape generators
  //================================================================================================
  namespace detail
  {
    // Convert a sequence of dimensions to the proper shaper type
    template<typename SizeType,typename ... Ds>
    constexpr auto as_shape(Ds... ds) noexcept
    {
      return kumi::fold_right( []<typename T>(auto a, T)
                              {
                                if constexpr(std::integral<T>) return a(); else return a[T::value];
                              }
                            , kumi::tuple{ds...}
                            , detail::shaper<SizeType>{}
                            );
    }
  }

  //================================================================================================
  //! @brief Generates a kwk::shape from a list of sizes
  //!
  //!
  //! @tparam SizeType  Integral type used to store sizes. If unspecified, `std::ptrdiff_t` is used.
  //! @param  ds        Variadic pack of sizes
  //================================================================================================
  template<typename SizeType, typename... Ds>
  constexpr auto of_size(Ds... ds) noexcept
  {
    // Compute the necessary constructor parameters
    std::size_t i = -1;
    auto  v = kumi::fold_right
              ( [&]<typename T>(auto acc, T m)
                {
                  i++;
                  if constexpr(std::integral<T>)
                    return push_back(acc, detail::axis{i,static_cast<std::ptrdiff_t>(m)});
                  else
                    return acc;
                }
              , kumi::tuple{ds...}
              , kumi::tuple<>{}
              );

    // Build the shape
    return kumi::apply( [](auto... v)
                        {
                          return  kwk::shape<detail::as_shape<SizeType>(Ds{}...)>(v...);
                        }
                      , v
                      );
  }

  template<typename... Ds> constexpr auto of_size( Ds... ds) noexcept
  {
    using type_t = typename detail::largest_type<detail::to_int_t<Ds>...>::type;
    return of_size<type_t,Ds...>(ds...);
  }
}
