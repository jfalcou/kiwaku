//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_MISC_INDEXER_HPP_INCLUDED
#define KIWAKU_MISC_INDEXER_HPP_INCLUDED

#include <type_traits>
#include <cstddef>
#include <cassert>

namespace kwk
{
  struct unit_indexer { std::ptrdiff_t begin, end;        };
  struct indexer      { std::ptrdiff_t begin, step, end;  };
  struct along        { along(std::ptrdiff_t d) : dim(d) {} std::ptrdiff_t dim; };

  template<typename Shape>
  constexpr auto reshape( unit_indexer const& ui, std::ptrdiff_t i, Shape const& s ) noexcept
  {
    auto const distance = ui.end - ui.begin;
    assert( (distance<= s[i]) && "[kwk::unit_indexer] Out of range reindexing" );

    return distance;
  }

  template<typename Shape>
  constexpr auto reshape( indexer const& id, std::ptrdiff_t i, Shape const& s ) noexcept
  {
    auto const distance = id.end - id.begin;
    assert( (distance<= s[i]) && "[kwk::indexer] Out of range reindexing" );

    return distance/id.step + 1;
  }

  template<typename Shape>
  constexpr auto reshape( along const& i, std::ptrdiff_t, Shape const& s ) noexcept
  {
    assert( (i.dim < Shape::static_count) && "[kwk::indexer] Out of range reindexing" );
    return s[i.dim];
  }

  struct complete_indexer
  {
    constexpr auto operator()(std::ptrdiff_t b, std::ptrdiff_t e) const noexcept
    {
      return unit_indexer{b,e};
    }

    constexpr auto operator()(std::ptrdiff_t b, std::ptrdiff_t s, std::ptrdiff_t e) const noexcept
    {
      return indexer{b,s,e};
    }
  };

  template<typename Shape>
  constexpr auto reshape( complete_indexer const&, std::ptrdiff_t i, Shape const& s ) noexcept
  {
    return s[i];
  }

  template<typename Indexer, typename Shape>
  constexpr auto reshape( Indexer const& n, std::ptrdiff_t i, Shape const& s ) noexcept
  {
    assert( (n < s[i]) && "[kwk::unit_indexer] Out of range reindexing" );
    return 1;
  }

  inline constexpr complete_indexer  _    = {};
  inline constexpr std::ptrdiff_t begin_  = -1;
  inline constexpr std::ptrdiff_t end_    = -1;
}

#endif
