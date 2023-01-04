//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <kwk/concepts/values.hpp>
#include <kwk/detail/raberu.hpp>
#include <kwk/utility/end.hpp>
#include <kwk/utility/fixed.hpp>
#include <kwk/utility/joker.hpp>
#include <kwk/utility/traits/extent.hpp>

#include <concepts>
#include <ostream>

#if !defined(_MSC_VER)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#endif

namespace kwk
{
  //================================================================================================
  // Keyword for range(...) specifications
  //================================================================================================
  namespace detail
  {
    struct to_   {};
    struct from_ {};
    struct by_   {};
  }

  inline constexpr auto to    = rbr::keyword(detail::to_  {});
  inline constexpr auto from  = rbr::keyword(detail::from_{});
  inline constexpr auto by    = rbr::keyword(detail::by_  {});

  template<auto D> struct shape;

  //================================================================================================
  // range(...) specifications
  //================================================================================================
  template<typename Begin, typename Step, typename End>
  struct slicer
  {
    Begin begin;
    Step  step;
    End   end;

    static constexpr bool has_begin = !std::same_as<Begin,joker>;
    static constexpr bool has_step  = !std::same_as<Step,joker>;
    static constexpr bool has_end   = !std::same_as<End,joker>;

    slicer(Begin b, Step s, End e) : begin{b},step{s},end{e} {}

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

      auto b = fix_extremum(begin, fixed<0>);
      auto s = fix_extremum(step , fixed<1>);
      auto e = fix_extremum(end  , count   );

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
      if constexpr(has_begin) os << f.begin << ':'; else os << "begin:";
      if constexpr(has_step)  os << f.step  << ':';
      if constexpr(has_end)   os << f.end; else os << "end";
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

  //================================================================================================
  // range constructor
  //================================================================================================
  template<rbr::concepts::option... Os>
  constexpr auto range(Os... os) noexcept requires(rbr::settings<Os...>::contains_only(to,from,by))
  {
    auto const opts = rbr::settings{os...};
    return slicer{opts[from | _], opts[by | _], opts[to | _]};
  }
}

#if !defined(_MSC_VER)
#pragma GCC diagnostic pop
#endif

