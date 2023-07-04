//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kwk/concepts/axis.hpp>
#include <kwk/concepts/extent.hpp>
#include <kwk/detail/stdfix.hpp>
#include <kwk/detail/kumi.hpp>
#include <kwk/settings/axis.hpp>
#include <kwk/settings/type.hpp>
#include <kwk/utility/fixed.hpp>

namespace kwk::__
{
  //====================================================================================================================
  // Extract information from a list of extent so building storage for shape-like entity is optimal
  //====================================================================================================================
  template<auto... D>
  constexpr auto map_descriptor()
  {
    struct map
    {
      int count = 0, pos = 0, curr = 0, stored_pos = 0;
      int index [sizeof...(D)]  = {};
      int stored[sizeof...(D)]  = {};

      constexpr map()
      {
        // Check if the current extent descriptor is an integer (ie a static size) or not
        auto check = [&]<typename T>(T)
        {
          if constexpr(std::integral<T>) index[pos] = -1;
          else
          {
            stored[stored_pos++] = pos;
            index[pos] = curr++;
            count++;
          }
          pos++;
        };

        // Check on the proper information
        auto is_present = [&]<typename T>(T)
        {
          if constexpr(kwk::concepts::axis<T>)  check(typename T::content_type{});
          else                                  check(T{});
        };

        (is_present(D),...);
      }
    };

    return map{};
  }

  //====================================================================================================================
  // Normalize extent as axis
  // TODO: Should this be a CPO ?
  //====================================================================================================================
  template<typename E> constexpr auto normalize_extent(E e)
  {
    if constexpr(kwk::concepts::axis<E>)  return e;
    else                                  return kwk::along<-1>[e];
  }

  //====================================================================================================================
  // Gather all the index of a given list of axis
  //====================================================================================================================
  template<typename N> constexpr auto all_index()
  {
    return kumi::apply( []<typename... M>(M...) { return kumi::tuple{kwk::fixed<M::index()>...}; }, N{});
  }

  //====================================================================================================================
  // Make sure all extent are turned into axis for prefilled construction
  //====================================================================================================================
  template<typename... E> constexpr auto normalize_extents(E... e)
  {
    // Computes which integral index is already used
    auto is_used        = [](auto m, auto i) { return kumi::any_of(i, [&](auto e) { return e != -1 && e == m; } ); };

    // Turn every extent into their normalized, axis like form
    auto all_extents    = kumi::tuple{normalize_extent(e)...};
    using all_extents_t = decltype(all_extents);

    // Retrieve the associated index of each axis
    constexpr auto is   = all_index<all_extents_t>();

    // Walk over all the axis and compute their integral index if they need one
    constexpr auto idx  = kumi::map ( [&, k = 0]<typename M>(M) mutable
                                      {
                                        if constexpr(!M::is_indexed || M::index() != -1) return M::index();
                                        else
                                        {
                                          while(is_used(k,is)) { k++; }
                                          return k++;
                                        }
                                      }
                                    , all_extents_t{}
                                    );

    constexpr int sz = kumi::max(idx);

    // Put the axis index in the proper place
    return kumi::map_index( [&]<typename M>(auto i, M m )
                            {
                              if constexpr(M::is_indexed) return kwk::along<sz - get<i>(idx)>[m.value];
                              else                        return m;
                            }
                          , all_extents
                          );
  }

  //====================================================================================================================
  // Force the type of a given extent
  // TODO: Should this be a CPO ?
  //====================================================================================================================
  template<typename T, typename E>
  KWK_TRIVIAL constexpr auto force_type(type_::info<E>)
  {
    return as<T>;
  }

  template<typename T, typename E>
  KWK_TRIVIAL constexpr auto force_type(E e)
  {
    if      constexpr( std::integral<E> )                   return static_cast<T>(e);
    else if constexpr( kwk::concepts::static_constant<E> )  return kwk::fixed<static_cast<T>(E::value)>;
    else if constexpr( kwk::concepts::joker<E> )            return T{};
    else if constexpr( kwk::concepts::axis<E> )             return axis_<E::identifier>{}[force_type<T>(e.value)];
    else if constexpr( rbr::concepts::option<E> )
    {
      constexpr typename E::keyword_type key = {};
      auto const new_key = force_type<T>(key);
      return (new_key = force_type<T>(e(key)));
    }
    else return e;
  }

  //====================================================================================================================
  // Type short-cut for internal representation of types in axis
  //====================================================================================================================
  template<typename T>        struct stored_type : T {};
  template<concepts::joker T> struct stored_type<T> { using type = std::int32_t; };

  template<typename T>
  using stored_t = typename stored_type<typename T::content_type>::type;
}
