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
#include <kwk/detail/stdfix.hpp>
#include <kwk/utility/end.hpp>
#include <kwk/utility/fixed.hpp>
#include <kwk/utility/joker.hpp>
#include <kwk/utility/linear_index.hpp>
#include <kwk/utility/traits/extent.hpp>

#include <ostream>

#if !defined(_MSC_VER)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#endif

namespace kwk
{
  //================================================================================================
  // Keyword for slice(...) specifications
  //================================================================================================
  namespace __
  {
    struct to_      {};
    struct from_    {};
    struct by_      {};
    struct length_  {};
  }

  inline constexpr auto to      = rbr::keyword(__::to_{}    );
  inline constexpr auto by      = rbr::keyword(__::by_{}    );
  inline constexpr auto from    = rbr::keyword(__::from_{}  );
  inline constexpr auto length  = rbr::keyword(__::length_{});

  template<auto... D> struct shape;
  template<auto... D> struct stride;

  //================================================================================================
  // slice(...) specifications
  //================================================================================================
  template<typename Begin, typename Step, typename End, typename Length>
  struct slicer
  {
    Begin   begin;
    Step    step;
    End     end;
    Length  len;

    using kwk_is_interval = void;
    static constexpr bool has_begin   = !std::same_as<Begin,joker>;
    static constexpr bool has_step    = !std::same_as<Step,joker>;
    static constexpr bool has_end     = !std::same_as<End,joker>;
    static constexpr bool has_length  = !std::same_as<Length,joker>;

    slicer(Begin b, Step s, End e, Length l) : begin{b},step{s},end{e}, len{l} {}

    template<auto... D, std::size_t N>
    constexpr auto reindex(shape<D...> const& sh, kumi::index_t<N> const&) const noexcept
    {
      auto count = get<N>(sh);
      if constexpr(has_begin)
      {
        if constexpr(concepts::extremum<Begin>) return begin.size(count);
        else                                    return begin;
      }
      else
      {
        if constexpr(has_step)    return step > 0 ? 0 : count - 1;
        else                      return 0;
      }
    }

    template<auto... D, std::size_t N>
    constexpr auto reshape(shape<D...> const& sh, kumi::index_t<N> const&) const noexcept
    {
      auto count = get<N>(sh);

      auto fix_extremum = [&]<typename T>(T s, auto def)
      {
        if constexpr(concepts::extremum<T>) return s.size(count);
        else if constexpr(is_joker_v<T>)    return def;
        else                                return s ;
      };

      if constexpr(has_length)
      {
        return fix_extremum(len, count);
      }
      else
      {
        auto eval = [&]<typename S, typename B>(S s, B b)
        {
          auto fix  = [&] (auto v, auto n) { auto w = n<0 ? -n : n; return (v % w != 0) + v/w; };

          if constexpr(concepts::static_constant<S> && concepts::static_constant<B>)
            return fixed<(S::value == 0) ? 0 : fix(S::value, B::value)>;
          else
            return (s == 0) ? 0 : fix(s, b);
        };

        auto s = fix_extremum(step , fixed<1>);

        if constexpr(has_begin || has_end)
        {
          auto b = fix_extremum(begin, fixed<0>);
          auto e = fix_extremum(end  , count   );

          if constexpr(has_step)
          {
            return (s>=0) ? eval(e-b, s) : eval(b+1_c,s);
          }
          else
          {
            return eval(e-b, fixed<1>);
          }
        }
        else
        {
          return eval(count,s);
        }
      }
    }

    template<auto... D, std::size_t N>
    constexpr auto restride(stride<D...> const& sh, kumi::index_t<N> const&) const noexcept
    {
      auto original_steps = get<N>(sh);
      auto fix_extremum = [&]<typename T>(T s)
      {
        if constexpr(is_joker_v<T>) return original_steps;
        else                        return s * original_steps;
      };

      return fix_extremum(step);
    }

    friend std::ostream& operator<<(std::ostream& os, slicer const& f)
    {
      auto const factor = [](auto const& v)
      {
        if constexpr(has_step) return v.step; else return fixed<1>;
      }(f);

      auto const start = [factor](auto const& v)
      {
        if constexpr(has_begin)                   return v.begin;
        else if constexpr(has_length && has_end)  return v.end - v.len * factor;
        else                                      return "begin";
      };

      auto const stop = [factor](auto const& v)
      {
        if constexpr(has_end)                       return v.end;
        else if constexpr(has_length && has_begin)  return v.begin + v.len * factor;
        else if constexpr(has_length)               return v.len * factor;
        else                                        return "end";
      };

      os << start(f) << ':';
      if constexpr(has_step)  os << f.step  << ':';
      os << stop(f);

      return os;
    }
  };

  template<typename B, typename S, typename E, typename L> slicer(B,S,E,L)  -> slicer<B,S,E,L>;

  //================================================================================================
  // Computes how much we should shift the beginning of the data along this dimension
  //================================================================================================
  template<auto... D, std::size_t N>
  constexpr auto reindex(shape<D...> const&, joker const&, kumi::index_t<N> const&) noexcept
  {
    return 0;
  }

  template<auto... D, concepts::extremum S, std::size_t N>
  constexpr auto reindex(shape<D...> const& sh, S const& s, kumi::index_t<N> const&) noexcept
  {
    return s.size(get<N>(sh));
  }

  template<auto... D, typename S, std::size_t N>
  constexpr auto reindex(shape<D...> const& sh, S const& s, kumi::index_t<N> const& idx) noexcept
  requires( requires{ s.reindex(sh,idx); } )
  {
    return s.reindex(sh,idx);
  }

  template<auto... D, std::convertible_to<std::ptrdiff_t> S, std::size_t N>
  constexpr auto reindex(shape<D...> const&, S const& s, kumi::index_t<N> const&) noexcept
  {
    return s;
  }

  //================================================================================================
  // Computes the number of new elements along this dimension
  //================================================================================================
  template<auto... D, std::size_t N>
  constexpr auto reshape(shape<D...> const& sh, joker const&, kumi::index_t<N> const&) noexcept
  {
    return get<N>(sh);
  }

  template<auto... D, typename S, std::size_t N>
  constexpr auto reshape(shape<D...> const& sh, S const& s, kumi::index_t<N> const& idx) noexcept
  {
    if constexpr( requires{ s.reshape(sh,idx); } ) return s.reshape(sh,idx);
    else                                           return fixed<1>;
  }

  //================================================================================================
  // Computes the new stride along this dimension
  //================================================================================================
  template<auto... D, typename S, std::size_t N>
  constexpr auto restride(stride<D...> const& sh, S const& s, kumi::index_t<N> const& idx) noexcept
  {
    if constexpr( requires{ s.restride(sh,idx); } ) return s.restride(sh,idx);
    else                                            return get<N>(sh);
  }


  //================================================================================================
  // Computes the shift in element to apply to the original pointer
  //================================================================================================
  template<auto... DS, typename... Slicers>
  auto origin(shape<DS...> const& shp, Slicers... slice) noexcept
  requires( requires{ reindex(shp,slice,kumi::index<0>);} && ... )
  {
    auto c = compress<sizeof...(slice)>(shp);
    return  linear_index( c
                        , kumi::map_index
                          ( [&](auto i, auto e) { return reindex(c,e,i); }
                          , kumi::tie(slice...)
                          )
                        );
  }

  //================================================================================================
  // slice constructor
  //================================================================================================
  template<rbr::concepts::option... Os>
  constexpr auto slice(Os... os) noexcept
  requires(   (rbr::settings<Os...>::contains_only(by, to    , from)  )
          ||  (rbr::settings<Os...>::contains_only(by, to    , length))
          ||  (rbr::settings<Os...>::contains_only(by, length, from)  )
          )
  {
    auto const opts = rbr::settings{os...};
    return slicer{opts[from | _], opts[by | _], opts[to | _],  opts[length | _]};
  }

  template<typename E>
  requires( concepts::extremum<E> || std::integral<E> )
  constexpr auto between(std::integral auto b, E e) noexcept { return slicer{b,_,e,_}; }

  constexpr auto at         (std::integral auto i) noexcept { return slicer{i    ,_,i    ,_}; }
  constexpr auto every      (std::integral auto s) noexcept { return slicer{_    ,s,_    ,_}; }
  constexpr auto first      (std::integral auto n) noexcept { return slicer{_    ,_,_    ,n}; }
  constexpr auto drop_first (std::integral auto n) noexcept { return slicer{n    ,_,_    ,_}; }
  constexpr auto last       (std::integral auto n) noexcept { return slicer{end-n,_,_    ,n}; }
  constexpr auto drop_last  (std::integral auto n) noexcept { return slicer{_    ,_,end-n,_}; }
}

#if !defined(_MSC_VER)
#pragma GCC diagnostic pop
#endif
