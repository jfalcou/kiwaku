//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kiwaku/detail/ct_helpers.hpp>
//#include <kiwaku/misc/slicers.hpp>
#include <kiwaku/misc/stride.hpp>
#include <utility>
#include <cstddef>
#include <iosfwd>
#include <array>

namespace kwk
{
  //================================================================================================
  // Shape type
  //================================================================================================
  template<auto Shaper> struct shape
  {
    using                           size_map      = decltype(Shaper.size_map());
    static constexpr std::ptrdiff_t static_size   = Shaper.size();
    static constexpr std::ptrdiff_t storage_size  = static_size - size_map::size;

    using storage_type  = std::array<std::ptrdiff_t,storage_size>;
    using stride_type   = unit_stride<static_size>;

    //==============================================================================================
    // NTTP Indirect interface
    //==============================================================================================
    static constexpr bool is_dynamic        = storage_size >= 1;
    static constexpr bool is_fully_dynamic  = storage_size == static_size;
    static constexpr bool is_fully_static   = storage_size == 0;

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
    requires  (   (std::is_convertible_v<T,std::ptrdiff_t> && ...)
              &&  (sizeof...(T) <= static_size)
              &&  is_fully_dynamic
              )
            : storage_{ static_cast<std::ptrdiff_t>(s)... }
    {
      if constexpr(sizeof...(T) < static_size)
      {
        for(std::size_t i = sizeof...(T);i<static_size;++i)
          storage_[i] = 1;
      }
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
        return std::integral_constant<std::ptrdiff_t,Shaper.at(I)>{};
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
          return std::max( { (this->get<I>() == 1 ? 0 : 1+I)... }) ;
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
        std::ptrdiff_t n{1};

        detail::constexpr_for<static_size>
        ( [&]<std::ptrdiff_t I>(std::integral_constant<std::ptrdiff_t,I> const&)
          {
            n *= this->get<I>();
          }
        );

        return n;
      }
    }

    //==============================================================================================
    // Convert shape to the equivalent unit stride
    //==============================================================================================
    constexpr auto as_stride() const { return stride(*this); }

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
    // Check if current shape contains (maybe strictly) all the extent of another one
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

namespace std
{
  template<std::size_t I, auto Shaper>
  struct tuple_element<I, kwk::shape<Shaper>>
  {
    using type = std::ptrdiff_t;
  };

  template<auto Shaper>
  struct  tuple_size<kwk::shape<Shaper>>
        : std::integral_constant<std::size_t,kwk::shape<Shaper>::static_size>
  {
  };
}
