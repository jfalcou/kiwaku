//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kiwaku/components/slicers/axis.hpp>
#include <kiwaku/components/stride.hpp>
#include <kiwaku/options/fixed.hpp>
#include <kiwaku/detail/ct_helpers.hpp>
#include <kiwaku/detail/assert.hpp>
#include <kiwaku/detail/shaper.hpp>
#include <kiwaku/detail/kumi.hpp>
#include <utility>
#include <cstddef>
#include <ostream>
#include <array>

namespace kwk
{
  struct size_;

  //================================================================================================
  // Shape type
  //================================================================================================
  template<auto Shaper> struct shape
  {
    using                           size_map      = decltype(Shaper.size_map());
    static constexpr std::ptrdiff_t static_size   = Shaper.size();
    static constexpr std::ptrdiff_t storage_size  = static_size - size_map::size;

    struct empty_storage {};
    using size_type     = typename decltype(Shaper)::size_type;
    using storage_type  = std::conditional_t< (storage_size!=0)
                                            , std::array<size_type,storage_size>
                                            , empty_storage
                                            >;
    using stride_type   = unit_stride<static_size>;

    //==============================================================================================
    // NTTP Indirect interface
    //==============================================================================================
    static constexpr bool is_dynamic        = storage_size >= 1;
    static constexpr bool is_fully_dynamic  = storage_size == static_size;
    static constexpr bool is_fully_static   = storage_size == 0;

    using stored_value_type = shape<Shaper>;
    using keyword_type      = size_;

    constexpr auto operator()(keyword_type const&) const noexcept { return *this; }

    //==============================================================================================
    // Default Constructor
    //==============================================================================================
    constexpr shape() noexcept
    {
      if constexpr(storage_size > 1ULL)  for(auto& e : storage_) e = 1;
      if constexpr(storage_size > 0ULL)  storage_[0] = 0;
    }

    //==============================================================================================
    // Construct from some amount of integral values
    //==============================================================================================
    template<typename... T>
    constexpr shape(T... s) noexcept
    requires  (   (std::is_convertible_v<T,size_type> && ...)
              &&  (sizeof...(T) <= static_size)
              &&  is_fully_dynamic
              )
            : storage_{ static_cast<size_type>(s)... }
    {
      if constexpr(sizeof...(T) < static_size)
      {
        for(std::size_t i = sizeof...(T);i<static_size;++i)
          storage_[i] = 1;
      }
    }

    //==============================================================================================
    // Construct from some partial dynamic values for partially static shape
    //==============================================================================================
    template<std::same_as<detail::axis>... Extent>
    constexpr shape(Extent... s) noexcept
    requires( (sizeof...(Extent) <= static_size) && !is_fully_static )
    {
      // Fill with 1s wherever applicable
      detail::constexpr_for<static_size>
      ( [&]<std::ptrdiff_t I>(std::integral_constant<std::ptrdiff_t,I> const&)
        {
          if constexpr(!size_map::contains(I))
            storage_[size_map::template locate<static_size>(I)] = 1;
        }
      );

      // Fill the proper axis value with the corresponding size
      auto const fill = [&](auto ext)
      {
        KIWAKU_ASSERT ( !size_map::contains(ext.dims)
                      , "[kwk::shape] Semi-dynamic construction overwrite static shape"
                      );

        storage_[size_map::template locate<static_size>(ext.dims)] = static_cast<size_type>(ext.size);
      };

      (fill(s),...);
    }

    //==============================================================================================
    // Constructs from a shape with less dimensions.
    // Small shape are allowed implicitly in large one and are completed with 1s.
    //==============================================================================================
    template<auto OtherShaper>
    constexpr shape( shape<OtherShaper> const& other ) noexcept
              requires( OtherShaper.size() <= static_size && is_fully_dynamic)
    {
      constexpr auto dz = std::min(OtherShaper.size(),static_size);

      detail::constexpr_for<dz>
      ( [&]<std::ptrdiff_t I>(std::integral_constant<std::ptrdiff_t,I> const&)
        {
          storage_[I] = other.template get<I>();
        }
      );

      for(std::size_t i = dz; i < static_size;++i) storage_[i] = 1;
    }

    //==============================================================================================
    // Constructs from a shape with more dimensions.
    // Large shape are allowed only explicitly in small ones as we need to convert data.
    //==============================================================================================
    template<auto OtherShaper>
    constexpr explicit  shape( shape<OtherShaper> const& other ) noexcept
                        requires( OtherShaper.size() > static_size && is_fully_dynamic)
    {
      constexpr auto dz = std::min(OtherShaper.size(),static_size);

      detail::constexpr_for<dz>
      ( [&]<std::ptrdiff_t I>(std::integral_constant<std::ptrdiff_t,I> const&)
        {
          storage_[I] = other.template get<I>();
        }
      );

      detail::constexpr_for<shape<OtherShaper>::static_size - dz>
      ( [&]<std::ptrdiff_t I>(std::integral_constant<std::ptrdiff_t,I> const&)
        {
          storage_.back() *= other.template get<dz+I>();
        }
      );
    }

    //==============================================================================================
    // Sequence interface
    //==============================================================================================
    static constexpr std::ptrdiff_t size() noexcept { return static_size; }

    //==============================================================================================
    // Element access
    //==============================================================================================
    template<std::size_t I> constexpr auto get() const noexcept
    {
      if constexpr(size_map::contains(I))
        return std::integral_constant<size_type,Shaper.at(I)>{};
      else
        return storage_[size_map::template locate<static_size>(I)];
    }

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
    // Shape interface
    //==============================================================================================
    constexpr std::ptrdiff_t nbdims() const noexcept
    {
      if constexpr(static_size == 0)
      {
        return 0;
      }
      else
      {
        return [&]<std::size_t...I>( std::index_sequence<I...> const&)
        {
          size_type m = this->get<0>() == 1 ? 0 : 1;
          ((m = std::max(m, size_type(this->get<I>() == 1 ? 0 : 1+I))),...);
          return m;
        }(std::make_index_sequence<static_size>());
      }
    }

    constexpr std::ptrdiff_t numel() const noexcept
    {
      if constexpr(static_size == 0)
      {
        return 0;
      }
      else
      {
        return [&]<std::size_t...I>( std::index_sequence<I...> const&)
        {
          return (size_type{1} * ... * this->get<I>());
        }(std::make_index_sequence<static_size>());
      }
    }

    //==============================================================================================
    // Convert shape to the equivalent unit stride
    //==============================================================================================
    constexpr auto as_stride() const requires(static_size > 0)
    {
      return stride<detail::unit_index_map<static_size>{}>(*this);
    }

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

    //==============================================================================================
    // I/O
    //==============================================================================================
    friend std::ostream& operator<<(std::ostream& os, shape const& s)
    {
      os << "[";

      detail::constexpr_for<static_size>
      ( [&]<std::ptrdiff_t I>(std::integral_constant<std::ptrdiff_t,I> const&)
        {
          os << " " << s.template get<I>();
        }
      );

      os << " ]";
      return os;
    }

    storage_type storage_;

    private:
    template<auto Shaper2, typename Comp, typename Check>
    constexpr bool compare( shape<Shaper2> const& other
                          , Comp const& comp, Check const& check
                          ) const noexcept
    {
      constexpr auto other_size = shape<Shaper2>::static_size;

      if constexpr( static_size == other_size )
      {
        bool result = true;
        return [&]<std::size_t... I>(std::index_sequence<I...> const&)
        {
          return (result && ... && comp(this->get<I>(),other.template get<I>()) ) ;
        }(std::make_index_sequence<static_size>());
      }
      else
      {
        constexpr auto d = std::min(static_size, other_size);

        // Compute equality over common slice of shape
        bool result = true;
        [&]<std::size_t... I>(std::index_sequence<I...> const&)
        {
          result = (result && ... && comp(this->get<I>(),other.template get<I>()) );
        }(std::make_index_sequence<d>());

        // Check that we have 1s everywhere in the other parts
        if constexpr( static_size < other_size )
        {
          constexpr auto sz = other_size - static_size;
          return [&]<std::size_t... I>(std::index_sequence<I...> const&)
          {
            return (result && ... && (other.template get<static_size+I>() == 1));
          }(std::make_index_sequence<sz>());
        }
        else if constexpr( static_size > other_size )
        {
          constexpr auto sz = static_size - other_size;
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
        : std::integral_constant<std::size_t,kwk::shape<Shaper>::static_size>
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

  template<typename... T> struct foo{};
  template<typename D0, typename... Ds> constexpr auto of_size(D0 d0, Ds... ds) noexcept
  {
    using type_t = std::common_type_t<detail::to_int_t<D0>, detail::to_int_t<Ds>...>;
    return of_size<type_t, D0, Ds...>(d0, ds...);
  }
}
