//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kiwaku/detail/strider.hpp>
#include <kiwaku/detail/kumi.hpp>
#include <ostream>
#include <concepts>

namespace kwk
{
  template<auto Shaper> struct shape;

  template<auto Strider>
  struct stride
  {
    //==============================================================================================
    // Sequence interface
    //==============================================================================================
    using storage_type    = detail::stride_storage<Strider>;
    using value_type      = typename storage_type::value_type;

    static constexpr std::ptrdiff_t static_size  = Strider.static_size;
    static constexpr std::ptrdiff_t size() noexcept { return static_size; }

    //==============================================================================================
    // NTTP Indirect interface
    //==============================================================================================
    using stride_type = stride<Strider>;
    using stride_map  = typename decltype(Strider)::map_type;
    static constexpr bool is_dynamic  = false;
    static constexpr bool is_unit     = stride_map::contains(0);
    static constexpr bool is_explicit = false;

    //==============================================================================================
    // Element access
    //==============================================================================================
    template<std::size_t I> constexpr auto get() const noexcept
    {
      constexpr auto i = std::min<std::ptrdiff_t>(I,static_size-1);

      if constexpr(stride_map::contains(i))
        return std::integral_constant<std::ptrdiff_t,1>{};
      else
        return storage_[stride_map::template locate<static_size>(i)];
    }

    //==============================================================================================
    // Construct from some amount of integral values
    //==============================================================================================
    template<std::convertible_to<std::ptrdiff_t>... Values>
    constexpr stride(Values... v) noexcept : storage_{}
    {
      // Filter out the non-dynamic stride values
      auto ptr = storage_.data();
      detail::for_each_args(  [&ptr]<typename T>(T sv)
                              {
                                if constexpr( !std::is_same_v<T,detail::unit_type> ) *ptr++ = sv;
                              }
                            , v...
                            );

      // Repeat last stride for when the data are missing
      if constexpr(sizeof...(Values) < static_size-1)
      {
        detail::constexpr_for<sizeof...(Values),static_size-1>
        (
          [&]<std::ptrdiff_t I>( std::integral_constant<std::ptrdiff_t,I> const&)
          {
            storage_[I] = storage_[sizeof...(Values)-1];
          }
        );
      }
    }

    //==============================================================================================
    // Construct from a shape with same dimensions
    // Only possible if stride is unit on the inner dimension
    //==============================================================================================
    template<typename Shaper>
    constexpr explicit stride(Shaper const& shp) noexcept requires(is_unit)
    {
      if constexpr(static_size > 1)
      {
        storage_[0] = shp.template get<0>();
        detail::constexpr_for<1,static_size-1>
        (
          [&]<std::ptrdiff_t I>( std::integral_constant<std::ptrdiff_t,I> const&)
          {
            storage_[I] = storage_[I-1] * shp.template get<I>();
          }
        );
      }
    }

    void swap( stride& other ) noexcept { storage_.swap( other.storage_ ); }

    //==============================================================================================
    // indexing interface
    //==============================================================================================
    template<typename... Is>
    constexpr auto  index(Is... is) const noexcept
                    requires(  (  sizeof...(Is) <= Strider.static_size)
                            && (  std::is_convertible_v<Is,std::ptrdiff_t> && ...)
                            )
    {
      return detail::linearize(std::make_index_sequence<sizeof...(Is)>(),*this,is...);
    }

    //==============================================================================================
    // I/O
    //==============================================================================================
    friend std::ostream& operator<<(std::ostream& os, stride const& s)
    {
      os << "[{ ";

      []<std::size_t... P>( std::index_sequence<P...> const&, auto& stream, auto const& c)
      {
        ((stream << c.template get<P>() << " "),...);
      }( std::make_index_sequence<static_size>{}, os, s);

      os << "}]";
      return os;
    }

    storage_type storage_;
  };

  //================================================================================================
  // Specialization for 1D unit stride
  //================================================================================================
  template<> struct stride<detail::unit_index_map<1>{}>
  {
    //==============================================================================================
    // NTTP Indirect interface
    //==============================================================================================
    using stride_type = stride<detail::unit_index_map<1>{}>;
    static constexpr bool is_dynamic  = false;
    static constexpr bool is_unit     = true;
    static constexpr bool is_explicit = false;

    //==============================================================================================
    // Sequence interface
    //==============================================================================================
    using value_type = detail::unit_type;

    static constexpr std::ptrdiff_t size() noexcept { return 1; }
    static constexpr std::ptrdiff_t static_size  = 1;

    //==============================================================================================
    // Element access
    //==============================================================================================
    template<std::size_t I> constexpr detail::unit_type get() const noexcept
    {
      return detail::unit_type{};
    }

    //==============================================================================================
    // Construct from some amount of integral values
    //==============================================================================================
    template<typename... Values> constexpr stride(Values&&...) noexcept {}

    //==============================================================================================
    // indexing interface
    //==============================================================================================
    constexpr auto index(std::ptrdiff_t is) const noexcept { return is; }

    void swap( stride& ) noexcept {}

    //==============================================================================================
    // I/O
    //==============================================================================================
    friend std::ostream& operator<<(std::ostream& os, stride<detail::unit_index_map<1>{}> const&)
    {
      return os << "[{ 1 }]";
    }
  };

  //================================================================================================
  // Deduction guide
  //================================================================================================
  template<typename... V>
  stride(V const&...) -> stride<detail::index_map<V...>{}>;

  //================================================================================================
  // Unit stride helpers
  //================================================================================================
  template<std::size_t Dimensions>
  using unit_stride = stride<detail::unit_index_map<Dimensions>{}>;

  inline constexpr detail::unit_type const _1 = {};

  //================================================================================================
  // Structured binding supports
  //================================================================================================
  template<std::size_t I, auto Strider>
  constexpr auto get(stride<Strider> const& s) noexcept
  {
    return s.template get<I>();
  }
}

//==================================================================================================
// Structured binding supports
//==================================================================================================
namespace std
{
  template<std::size_t I, auto Strider>
  struct tuple_element<I, kwk::stride<Strider>>
  {
    using type = decltype(std::declval<kwk::stride<Strider>>().template get<I>());
  };

  template<auto Strider>
  struct  tuple_size<kwk::stride<Strider>>
        : std::integral_constant<std::size_t,Strider.static_size>
  {};
}

namespace kumi
{
  template<auto Strider>
  struct is_product_type<kwk::stride<Strider>> : std::true_type {};
}
