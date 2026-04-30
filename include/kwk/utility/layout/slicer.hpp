//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kwk/utility/numeric/fixed.hpp>

namespace kwk
{

  template<typename Begin, typename End, typename Step> struct slicer : private __::mixed_sequence<Begin, End, Step>
  {
    using parent = __::mixed_sequence<Begin, End, Step>;

    constexpr slicer(Begin b, End e, Step s) : parent{b, e, s} {}

    constexpr auto begin(this auto&& self) noexcept { return get<0>(KWK_FWD(self)); }

    constexpr auto end(this auto&& self) noexcept { return get<1>(KWK_FWD(self)); }

    constexpr auto step(this auto&& self) noexcept { return get<2>(KWK_FWD(self)); }

    template<std::size_t I>
    requires(I < 3)
    friend constexpr decltype(auto) get(slicer const& self) noexcept
    {
      return get<I>(static_cast<parent const&>(self));
    }

    template<std::size_t I>
    requires(I < 3)
    friend constexpr decltype(auto) get(slicer& self) noexcept
    {
      return get<I>(static_cast<parent&>(self));
    }

    template<std::size_t I>
    requires(I < 3)
    friend constexpr decltype(auto) get(slicer&& self) noexcept
    {
      return get<I>(static_cast<parent&&>(KWK_FWD(self)));
    }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                         slicer const& s) noexcept
    {
      os << s.begin() << ':';
      os << s.end() << ':';
      os << s.step();

      return os;
    }
  };

  // @brief Deduction guide
  template<typename B, typename E, typename S> slicer(B, E, S) -> slicer<B, E, S>;

  // @brief builds a slicer from a type
  template<typename B, typename E, typename S> constexpr auto to_slicer(slicer<B, E, S> const& s) noexcept
  {
    return s;
  }

  constexpr auto to_slicer(std::integral auto const& t) noexcept
  {
    return slicer{t, t + 1, fixed<1>};
  }

  template<concepts::static_constant T> constexpr auto to_slicer(T const&) noexcept
  {
    return slicer{fixed<T::value>, fixed<T::value + 1>, fixed<1>};
  }

  constexpr auto to_slicer(kwk::concepts::extremum auto const& t) noexcept
  {
    return slicer{t.offset(), t.ratio(), _};
  }

  template<typename T>
  constexpr auto to_slicer(T const&) noexcept
  requires(is_wildcard<T>)
  {
    return slicer{fixed<0>, _, fixed<1>};
  }

  // @brief helper to deduplicate code
  template<typename S, typename D> constexpr auto handle(S const& s, D dim, auto def)
  {
    if constexpr (std::is_invocable_v<S, D>) return s(dim);
    else if constexpr (kwk::concepts::static_constant<S>) return s;
    else if constexpr (std::integral<S>) return s;
    else return def;
  }

  template<typename B, typename E, typename S> constexpr auto slice_extent(auto dim, slicer<B, E, S> const& slc)
  {
    auto eval = [&]<typename Len, typename Step>(Len l, Step st) {
      auto fix = [&](auto v, auto n) {
        auto w = (n < 0) ? -n : n;
        return (v % w != 0) + v / w;
      };
      if constexpr (concepts::static_constant<Len> && concepts::static_constant<Step>)
        return fixed < (Step::value == 0) ? 0 : fix(Len::value, Step::value) > ;
      else return (st == 0) ? 0 : fix(l, st);
    };

    auto b = handle(slc.begin(), dim, fixed<0>);
    auto e = handle(slc.end(), dim, dim);
    auto s = handle(slc.step(), dim, fixed<1>);

    return (s >= 0) ? eval(e - b, s) : eval(b + 1_c, s);
  }

  template<typename B, typename E, typename S> constexpr auto slice_stride(auto strd, slicer<B, E, S> const& slc)
  {
    if constexpr (is_wildcard<S>) return strd;
    else return slc.step() * strd;
  }

  // Computes the offset to add to the base pointer of a container to retrieve the sliced container
  template<shape_descriptor D, storage_order_descriptor C, concepts::slicer... S>
  constexpr auto origin(shape<D> const& s, storage_order_t<C> const& so, S const&... slices) noexcept
  requires(D.ndim == sizeof...(S))
  {
    auto pos = kumi::apply(
      [](auto&&... elts) {
        return kwk::shape{handle(get<1>(KWK_FWD(elts)).begin(), get<0>(KWK_FWD(elts)), fixed<0>)...};
      },
      kumi::zip(kumi::to_tuple(s), kumi::forward_as_tuple(to_slicer(slices)...)));
    return linearize(kwk::stride{pos, so}, pos);
  }

  template<shape_descriptor D, concepts::slicer... S>
  constexpr auto reshape(shape<D> const& s, S const&... slices) noexcept
  requires(D.ndim == sizeof...(S))
  {
    return kumi::apply(
      [](auto&&... elts) { return kwk::shape{slice_extent(get<0>(KWK_FWD(elts)), get<1>(KWK_FWD(elts)))...}; },
      kumi::zip(kumi::to_tuple(s), kumi::forward_as_tuple(to_slicer(slices)...)));
  }

  template<shape_descriptor D, concepts::slicer... S>
  constexpr auto restride(stride<D> const& strd, S const&... slices) noexcept
  requires(D.ndim == sizeof...(S))
  {
    return kumi::apply(
      [](auto&&... elts) { return kwk::stride{slice_stride(get<0>(KWK_FWD(elts)), get<1>(KWK_FWD(elts)))...}; },
      kumi::zip(kumi::to_tuple(strd), kumi::forward_as_tuple(to_slicer(slices)...)));
  }

  constexpr auto at(std::integral auto i)
  {
    return kwk::slicer{i, i + 1, _};
  }

  constexpr auto every(std::integral auto s)
  {
    return kwk::slicer{_, _, s};
  }

  constexpr auto first(std::integral auto i)
  {
    return kwk::slicer{_, i, _};
  }

  constexpr auto last(std::integral auto i)
  {
    return kwk::slicer{kwk::end - i, _, _};
  }

  constexpr auto drop_first(std::integral auto i)
  {
    return kwk::slicer{i, _, _};
  }

  constexpr auto drop_last(std::integral auto i)
  {
    return kwk::slicer{_, kwk::end - i, _};
  }

  template<typename Begin, typename End> constexpr auto between(Begin b, End e)
  {
    return kwk::slicer{b, e, _};
  }
}

#if !defined(KWK_DOXYGEN_INVOKED)
template<typename B, typename E, typename S>
struct std::tuple_size<kwk::slicer<B, E, S>> : std::integral_constant<std::size_t, 3>
{
};

template<std::size_t I, typename B, typename E, typename S> struct std::tuple_element<I, kwk::slicer<B, E, S>>
{
  using type = decltype(get<I>(std::declval<kwk::slicer<B, E, S>>()));
};
#endif
