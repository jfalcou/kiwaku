//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <array>
#include <cstdint>
#include <type_traits>
#include <utility>

// Faster than std::forward
#define KWK_FWD(...) static_cast<decltype(__VA_ARGS__) &&>(__VA_ARGS__)

// Force a function to be inline
#if defined(KWK_NO_FORCEINLINE)
#  define KWK_FORCEINLINE inline
#else
#  if defined(_MSC_VER)
#    define KWK_FORCEINLINE __forceinline
#  elif defined(__GNUC__) && __GNUC__ > 3
#    define KWK_FORCEINLINE inline __attribute__((__always_inline__))
#  else
#    define KWK_FORCEINLINE inline
#  endif
#endif


namespace kwk::detail
{
  //================================================================================================
  // Find the static_size of static array like types
  //================================================================================================
  template<typename> struct static_size : std::false_type {};

  template<typename T, std::size_t N> struct static_size<std::array<T, N>> : std::true_type
  {
    static constexpr std::size_t size_value = N;
  };

  template<typename T, std::size_t N> struct static_size<T[N]> : std::true_type
  {
    static constexpr std::size_t size_value = N;
  };

  template<typename T>
  inline constexpr std::size_t static_size_v = static_size<std::remove_cvref_t<T>>::size_value;

  //================================================================================================
  // Find the value type of anything with a .data()
  //================================================================================================
  template<typename C>
  using value_type_of = std::remove_reference_t<decltype(*std::begin(std::declval<C&>()))>;

  //================================================================================================
  // for_each_args abstraction
  //================================================================================================
  template<typename Callable, typename... Args>
  constexpr void for_each_args(Callable c, Args&&... args) noexcept
  {
    (c(KWK_FWD(args)),...);
  }

  //================================================================================================
  // constexpr_for abstraction
  //================================================================================================
  template<std::size_t Begin, std::size_t End, typename Callable>
  constexpr auto constexpr_for(Callable c) noexcept
  {
    return  []<typename Func,std::ptrdiff_t... Idx>
            (std::integer_sequence<std::ptrdiff_t,Idx...> const&, Func f)
            {
              ((f( std::integral_constant<std::ptrdiff_t,Begin+Idx>{})),...);
              return f;
            }( std::make_integer_sequence<std::ptrdiff_t, End-Begin>(), c );
  }

  template<std::size_t Count, typename Callable>
  constexpr auto constexpr_for(Callable c) noexcept
  {
    return constexpr_for<0,Count>(c);
  }

  //================================================================================================
  // List of integral constants
  //================================================================================================
  template<auto... Bs> struct values {};

  //================================================================================================
  // Augmented static integer list for mapping known unit values
  //================================================================================================
  template<std::size_t... I> struct index_list
  {
    // How many indexes ?
    static constexpr std::size_t size = sizeof...(I);

    // is N in the set of known unit indexes ?
    static constexpr bool contains(std::size_t N) noexcept
    {
      if constexpr(sizeof...(I) > 0)
      {
        bool found[] = { (I==N)... };
        for(auto f : found) if(f) return true;
        return false;
      }
      else return false;
    }

    // Aggregation of an index into an existing index_list
    template<std::size_t J> using append = index_list<I...,J>;

    // Find the actual dynamic index of a non-unit index
    template<std::uint8_t Size> static constexpr auto locate(std::uint8_t const N) noexcept
    {
      // Count how far you need to go to find an unknown
      auto const less_than_n_count{( 0 + ... + (I < N) )};
      return N - less_than_n_count;
    }
  };

  //================================================================================================
  // Build a list of index in a type list where a target type is found
  //================================================================================================
  template<typename Target, std::size_t Index, typename Current, typename... Vs>
  struct type_map_impl;

  template<typename Target, std::size_t Index, typename Current>
  struct type_map_impl<Target, Index, Current, Target>
  {
    using type = typename Current::template append<Index>;
  };

  template<typename Target, std::size_t Index, typename Current, typename Head>
  struct type_map_impl<Target, Index, Current, Head>
  {
    using type = Current;
  };

  template<typename Target, std::size_t Index, typename Current, typename Head, typename... Tail>
  struct  type_map_impl<Target, Index  , Current, Head, Tail...>
        : type_map_impl<Target, Index+1,Current , Tail...>
  {
  };

  template<typename Target, std::size_t Index, typename Current, typename... Tail>
  struct  type_map_impl<Target, Index, Current, Target, Tail...>
        : type_map_impl<Target, Index+1,typename Current::template append<Index>,Tail...>
  {
  };

  template<typename Target, typename... Vs>
  struct type_map : type_map_impl<Target, 0, index_list<>, Vs...>
  {};
}
