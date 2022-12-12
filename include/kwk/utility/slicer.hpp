//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <kwk/concepts/values.hpp>
#include <kwk/detail/kumi.hpp>
#include <kwk/utility/end.hpp>
#include <kwk/utility/fixed.hpp>
#include <kwk/utility/joker.hpp>
#include <kwk/utility/traits/extent.hpp>
#include <ostream>

#if !defined(_MSC_VER)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#endif

namespace kwk
{
  template<auto D> struct shape;

  template<typename Begin, typename Step, typename End>
  struct slicer
  {
    kumi::tuple<Begin,Step,End> storage;

    static constexpr bool has_begin = !std::same_as<Begin,joker>;
    static constexpr bool has_step  = !std::same_as<Step,joker>;
    static constexpr bool has_end   = !std::same_as<End,joker>;

    slicer(Begin b, Step s, End e) : storage{b,s,e} {}

    template<typename S>
    constexpr auto by(S s) const noexcept requires(!concepts::extremum<S>)
    {
      if constexpr(has_step)
      {
        auto stp = s*get<1>(storage);
        return slicer<Begin,decltype(stp),End>{get<0>(storage),stp,get<2>(storage)};
      }
      else
      {
        return slicer<Begin,S,End>{get<0>(storage),s,get<2>(storage)};
      }
    }

    template<auto D, std::size_t N>
    constexpr auto reshape(shape<D> const& sh, kumi::index_t<N> const&) const noexcept
    {
      auto count = sh.template extent<N>();

      auto fix_extremum = [&]<typename T>(T s, auto def)
      {
        if constexpr(concepts::extremum<T>) return s.size(count);
        else if constexpr(is_joker_v<T>)    return def;
        else                                return s;
      };

      auto b = fix_extremum(get<0>(storage), fixed<0>);
      auto s = fix_extremum(get<1>(storage), fixed<1>);
      auto e = fix_extremum(get<2>(storage), count);

      auto eval = [&]<typename S, typename B>(S s, B b)
      {
        auto fix  = [] (auto v, auto n) { return (v % n != 0) + v/n; };

        if constexpr(concepts::static_constant<S> && concepts::static_constant<B>)
          return fixed<(S::value == 0) ? 0 : fix(S::value, B::value)>;
        else
          return (s == 0) ? 0 : fix(s, b);
      };

      return eval(e-b,s);
    }

    friend std::ostream& operator<<(std::ostream& os, slicer const& f)
    {
      if constexpr(has_begin) os << get<0>(f.storage)<< ':'; else os << "begin:";
      if constexpr(has_step)  os << get<1>(f.storage) << ':';
      if constexpr(has_end)   os << get<2>(f.storage); else os << "end";
      return os;
    }
  };

  template<typename B, typename S, typename E> slicer(B,S,E)  -> slicer<B,S,E>;

  template<auto D, typename S, std::size_t N>
  constexpr auto reshape(shape<D> const& sh, S const& s, kumi::index_t<N> const& idx) noexcept
  {
    if constexpr(std::same_as<S,joker>)                 return sh.template extent<N>();
    else if constexpr( requires{ s.reshape(sh,idx); } ) return s.reshape(sh,idx);
    else                                                return fixed<1>;
  }

  constexpr auto by     (auto n)         noexcept { return slicer{_,n,_}; }
  constexpr auto from   (auto b)         noexcept { return slicer{b,_,_}; }
  constexpr auto to     (auto e)         noexcept { return slicer{_,_,e}; }
  constexpr auto between(auto b, auto e) noexcept { return slicer{b,_,e}; }
}

#if !defined(_MSC_VER)
#pragma GCC diagnostic pop
#endif
