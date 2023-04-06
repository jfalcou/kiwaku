//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/concepts/axis.hpp>
#include <kwk/concepts/values.hpp>
#include <kwk/detail/kumi.hpp>
#include <kwk/detail/sequence/combo.hpp>
#include <kwk/settings/axis.hpp>
#include <kwk/utility/fixed.hpp>
#include <utility>
#include <algorithm>

namespace kwk::__
{
  //================================================================================================
  // Converts a sequence of value/joker/axis into a shape/stride descriptor type
  //================================================================================================
  template<typename SizeType, typename... Args>
  KWK_FORCEINLINE constexpr auto as_descriptor(Args... args)
  {
    if constexpr(sizeof...(Args) == 0) return combo<SizeType>{};
    else
    {
      auto const convert = []<typename M, typename I>(M, I)
      {
        constexpr auto sz = sizeof...(Args) - 1;

        // integral : runtime indexed axis
        if constexpr(std::integral<M> || is_joker_v<M>) return along<sz - I::value>;
        // static integral : compile-time indexed axis
        else if constexpr(concepts::static_constant<M>) return along<sz - I::value>[M::value];
        // named axis
        else if constexpr(rbr::concepts::option<M>)
        {
          using key_t   = typename M::keyword_type;
          using value_t = typename M::stored_value_type;

        // name : runtime named axis
          if constexpr(std::integral<value_t> || is_joker_v<value_t>) return key_t{};
          // static integral : compile-time indexed axis
          else if constexpr(concepts::static_constant<value_t>)       return key_t{}[value_t::value];
        }
      };

      return [&]<std::size_t... N>(std::index_sequence<N...>)
      {
        return make_combo<SizeType>((convert(args, kumi::index<N>) * ...));
      }(std::index_sequence_for<Args...>{});
    }
  }

  template<typename SizeType, typename A>
  KWK_FORCEINLINE constexpr auto as_descriptor(A const&)
  requires requires(A) { A::descriptor; }
  {
    return A::descriptor;
  }

  // Helper for MSVC
  template<typename... T> constexpr auto descriptor_from()
  {
    return as_descriptor<typename largest_integral<T...>::type>(T{}...);
  }

  //================================================================================================
  // Converts a value/joker/axis into its storable representation
  //================================================================================================
  template<typename Arg, typename Desc, typename Index>
  KWK_FORCEINLINE constexpr auto as_axis(Arg arg, Desc d, Index) noexcept
  {
    // integral : runtime indexed axis
    if constexpr(std::integral<Arg>)                  return arg;
    else if constexpr(is_joker_v<Arg>)                return 0;
    // static integral : compile-time indexed axis
    else if constexpr(concepts::static_constant<Arg>) return fixed<Arg::value>;
    // named axis
    else if constexpr(rbr::concepts::option<Arg>)
    {
      using value_t = typename Arg::stored_value_type;

      // name : runtime named axis
      if constexpr(std::integral<value_t>)                        return arg.contents;
      else if constexpr(is_joker_v<value_t>)                      return 0;
      // static integral : compile-time indexed axis
      else if constexpr(kwk::concepts::static_constant<value_t>)  return fixed<value_t::value>;
    }
  }

  //================================================================================================
  // Helpers for require clauses of make_extent
  //================================================================================================
  // Check no axis is duplicated
  template<typename T> inline constexpr bool duplicate_axis = false;

  template<typename T, template<class...> class Holder, typename... Axis>
  inline constexpr bool duplicate_axis<Holder<T,Axis...>>
                      = !kwk::__::all_unique<typename Axis::axis_kind...>;

  // Check we dont make a nD axis sequence with too much indexed axis
  template<typename T,std::int32_t Size> inline constexpr bool has_valid_index = false;

  template<typename T, template<class...> class Holder, typename... Axis,std::int32_t Size>
  inline constexpr bool has_valid_index<Holder<T,Axis...>,Size> = []()
  {
    auto max_index = std::max({Axis::index()...});
    return (max_index == -1) || max_index < Size;
  }();

  //================================================================================================
  // Constructs a type described by a sequence of axis and fill it with their values
  //================================================================================================
  template< template<auto> class Wrapper
          , typename SizeType
          , typename... Args
          >
  constexpr auto make_extent(Args... args)
  requires( !duplicate_axis<decltype(as_descriptor<SizeType>(args...))>
          && (has_valid_index<decltype(as_descriptor<SizeType>(args...)),sizeof...(Args)>)
          )
  {
    return [&]<std::size_t... N>(std::index_sequence<N...>)
    {
      constexpr std::int32_t sz = sizeof...(Args) - 1;
      return  Wrapper<as_descriptor<SizeType>(Args{}...)>
              { as_axis ( args
                        , get<N>(as_descriptor<SizeType>(Args{}...))
                        , kumi::index<sz-N>
                        )...
              };
    }(std::index_sequence_for<Args...>{});
  }

  //================================================================================================
  // If an error occurs here:
  //  - you tried to build a shape/stride with duplicate axis name or index.
  //    e.g: of_shape(width = 4, width = 90);
  //  - you tried to build a shape/stride with an index greater than the order of the shape/stride
  //    e.g: of_shape(along<5> = 90);
  //================================================================================================
  template< template<auto> class, typename SizeType, typename... Args>
  constexpr auto make_extent(Args... args)
  requires( duplicate_axis<decltype(as_descriptor<SizeType>(args...))>
          || !has_valid_index<decltype(as_descriptor<SizeType>(args...)),sizeof...(Args)>
          ) = delete;
}
