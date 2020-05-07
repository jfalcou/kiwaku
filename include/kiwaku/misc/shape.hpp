//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_MISC_SHAPE_HPP_INCLUDED
#define KIWAKU_MISC_SHAPE_HPP_INCLUDED

#include <kiwaku/detail/options/options.hpp>
#include <kiwaku/misc/slicers.hpp>
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
  template<std::size_t Dimensions>
  struct shape : private std::array<std::ptrdiff_t,Dimensions>
  {
    using storage_type  = std::array<std::ptrdiff_t,Dimensions>;
    using stride_type   = unit_stride<Dimensions>;

    //==============================================================================================
    // NTTP Indirect interface
    //==============================================================================================
    using shape_type = shape<Dimensions>;
    using option_tag = detail::shape_tag;
    static constexpr bool is_dynamic = false;

    //==============================================================================================
    // Dependent types
    //==============================================================================================
    using reference       = typename storage_type::reference;
    using const_reference = typename storage_type::const_reference;

    static constexpr std::size_t    static_size   = Dimensions;
    static constexpr std::ptrdiff_t static_count  = Dimensions;

    //==============================================================================================
    // Default Constructor
    //==============================================================================================
    constexpr shape() noexcept
    {
      if constexpr(static_size > 1ULL)  for(auto& e : *this) e = 1;
      if constexpr(static_size > 0ULL)  (*this)[0] = 0;
    }

    //==============================================================================================
    // Construct from some amount of integral values
    //==============================================================================================
    template<typename T>
    constexpr explicit shape(T s) noexcept
              requires ((std::is_convertible_v<T,std::ptrdiff_t>))
            : storage_type{ static_cast<std::ptrdiff_t>(s) }
    {
      if constexpr(static_size > 1 )
      {
        for(std::size_t i = 1;i<static_size;++i)
          (*this)[i] = 1;
      }
    }

    template<typename... T>
    constexpr explicit shape(T... s) noexcept
    requires ((std::is_convertible_v<T,std::ptrdiff_t> && ...) && sizeof...(T)<=static_size)
            : storage_type{ static_cast<std::ptrdiff_t>(s)... }
    {
      if constexpr(sizeof...(T) < static_size)
      {
        for(std::size_t i = sizeof...(T);i<static_size;++i)
          (*this)[i] = 1;
      }
    }

    //==============================================================================================
    // Constructs from a shape with less dimensions.
    // Small shape are allowed implicitly in large one and are completed with 1s.
    //==============================================================================================
    template< std::size_t OtherDimensions>
    constexpr shape( shape<OtherDimensions> const& other ) noexcept
              requires( OtherDimensions <= static_size )
    {
      constexpr auto dz = std::min(OtherDimensions,static_size);

      std::size_t i = 0;
      for(; i < dz;++i)          (*this)[i] = other[i];
      for(; i < static_size;++i) (*this)[i] = 1;
    }

    //==============================================================================================
    // Constructs from a shape with more dimensions.
    // Large shape are allowed only explicitly in small ones as we need to convert data.
    //==============================================================================================
    template<std::size_t OtherDimensions>
    explicit constexpr  shape( shape<OtherDimensions> const& other ) noexcept
                        requires( OtherDimensions > static_size )
    {
      constexpr auto dz = std::min(OtherDimensions,static_size);

      std::size_t i = 0;
      for(; i < dz;++i)               (*this)[i] = other[i];
      for(; i < OtherDimensions;++i)  storage_type::back() *= other[i];
    }

    //==============================================================================================
    // Sequence interface
    //==============================================================================================
    static constexpr std::size_t    size()  noexcept { return static_size;  }
    static constexpr std::ptrdiff_t count() noexcept { return static_count; }

    using storage_type::data;
    using storage_type::begin;
    using storage_type::end;
    using storage_type::rbegin;
    using storage_type::rend;
    using storage_type::front;
    using storage_type::back;

    constexpr reference operator[](std::ptrdiff_t i) noexcept requires(static_size!=0)
    {
      return storage_type::operator[](i);
    }

    constexpr const_reference operator[](std::ptrdiff_t i) const noexcept requires(static_size!=0)
    {
      return storage_type::operator[](i);
    }

    void swap( shape& other ) noexcept
    {
      storage_type::swap( static_cast<storage_type&>(other) );
    }

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
        for(int i=static_count-1;i>=0;--i)
        {
          if( (*this)[i] != 1) return i+1;
        }
        return 0;
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
        for(int i=0;i<static_count;i++) n *= (*this)[i];
        return n;
      }
    }

    //==============================================================================================
    // Convert shape to the equivalent unit stride
    //==============================================================================================
    constexpr auto as_stride() const { return stride_type(*this); }

    //==============================================================================================
    // Storage access
    //==============================================================================================
    storage_type const& storage() const noexcept { return static_cast<storage_type const&>(*this); }

    //==============================================================================================
    // Comparisons
    //==============================================================================================
    template<std::size_t Dimensions2>
    constexpr bool operator==( shape<Dimensions2> const& other) const noexcept
    {
      return compare(other, [](auto a, auto b) { return a == b; },[](auto a) { return a != 1; } );
    }

    template<std::size_t Dimensions2>
    constexpr bool operator!=( shape<Dimensions2> const& other) const noexcept
    {
      return compare( other , [](auto a, auto b) { return a != b; },[](auto a) { return a != 1; } );
    }

    //==============================================================================================
    // Check if current shape contains (maybe strictly) all the extent of another one
    //==============================================================================================
    template<std::size_t Dimensions2>
    constexpr bool contains( shape<Dimensions2> const& other) const noexcept
    {
      return compare( other , [](auto a, auto b) { return a>=b;}, [](auto) { return false; } );
    }

    template<std::size_t Dimensions2>
    constexpr bool strictly_contains( shape<Dimensions2> const& other) const noexcept
    {
      return compare( other , [](auto a, auto b) { return a>b;}, [](auto) { return false; } );
    }

    //==============================================================================================
    // I/O
    //==============================================================================================
    friend std::ostream& operator<<(std::ostream& os, shape const& s)
    {
      os << "[";
      for(auto e : s) os << " " << e;
      os << " ]";
      return os;
    }

    private:
    template<std::size_t Dimensions2, typename Comp, typename Check>
    constexpr bool compare( shape<Dimensions2> const& other
                          , Comp const& comp, Check const& check
                          ) const noexcept
    {
      if constexpr(Dimensions == Dimensions2)
      {
        return comp(storage(),other.storage());
      }
      else
      {
        constexpr auto d = std::min(Dimensions, Dimensions2);

        for(std::size_t i=0;i<d;++i)
          if( !comp((*this)[i], other[i]) ) return false;

        if constexpr(Dimensions < Dimensions2)
        {
          for(std::size_t i=Dimensions;i<Dimensions2;++i)
            if( other[i] !=1 ) return false;
        }
        else if constexpr(Dimensions > Dimensions2)
        {
          for(std::size_t i=Dimensions2;i<Dimensions;++i)
            if( check((*this)[i]) ) return false;
        }
      }

      return true;
    }
  };

  //================================================================================================
  // Deduction guides
  //================================================================================================
  template<typename... T> shape(T... s) -> shape<sizeof...(T)>;
}

//==================================================================================================
// Shape supports structured bindings.
//==================================================================================================
namespace kwk
{
  template<std::size_t I, std::size_t Dimensions>
  constexpr auto get(shape<Dimensions> const& s) noexcept { return s[I]; }

  template<std::size_t I, std::size_t Dimensions>
  auto& get(shape<Dimensions>& s) noexcept { return s[I]; }
}

namespace std
{
  template<std::size_t I, std::size_t Dim>
  struct tuple_element<I, kwk::shape<Dim>>
  {
    using type = std::ptrdiff_t;
  };

  template<std::size_t Dim>
  struct tuple_size<kwk::shape<Dim>> : std::integral_constant<std::size_t,Dim>
  {
  };
}

#endif
