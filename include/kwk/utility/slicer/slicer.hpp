//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kwk/utility/fixed.hpp>

namespace kwk
{

  namespace concepts
  {
    template<typename T>
    concept interval = kumi::concepts::sized_product_type<T, 3>;

    template<typename T>
    concept slicer = requires(T const& t) { to_slicer(t); };

    // template<typename T>
    // concept extremum = std::integral<>
  }

  template<typename Begin, typename End, typename Step> struct slicer : __::mixed_sequence<Begin, End, Step>
  {
    using parent = __::mixed_sequence<Begin, End, Step>;

    constexpr slicer(Begin b, End e, Step s) : parent{b, e, s} {}

    constexpr auto begin() const noexcept { return get<0>(*this); }

    constexpr auto end() const noexcept { return get<1>(*this); }

    constexpr auto step() const noexcept { return get<2>(*this); }

    template<std::size_t I>
    requires(I < 3)
    friend constexpr decltype(auto) get(slicer const& self) noexcept
    {
      return get<I>(static_cast<parent const&>(self));
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

  // constexpr auto to_slicer(concepts::extremum auto const& t)    noexcept { return slicer{ t.offset(), t.ratio(), _};
  // }
  template<typename T>
  constexpr auto to_slicer(T const&) noexcept
  requires(is_wildcard<T>)
  {
    return slicer{fixed<0>, _, fixed<1>};
  }

  // @brief helper to deduplicate code
  template<typename S, typename D> constexpr auto handle(S const& s, [[maybe_unused]] D dim, auto def)
  {
    // if constexpr ( std::is_invocable_v<S, D> ) return s(dim);
    // else
    if constexpr (concepts::static_constant<S>) return s;
    else if constexpr (std::integral<S>) return s;
    else return def;
  }

  template<concepts::slicer S> constexpr auto __reindex(auto dim, S const& s)
  {
    auto sl = to_slicer(s);
    return handle(sl.begin(), dim, fixed<0>);
  }

  template<concepts::slicer S> constexpr auto __reshape(auto dim, S const& s)
  {
    auto sl = to_slicer(s);
    auto eval = [&]<typename Len, typename Step>(Len l, Step st) {
      auto fix = [&](auto v, auto n) {
        auto w = (n < 0) ? -n : n;
        return (v % w != 0) + v / w;
      };
      if constexpr (concepts::static_constant<Len> && concepts::static_constant<Step>)
        return fixed < (Step::value == 0) ? 0 : fix(Len::value, Step::value) > ;
      else return (st == 0) ? 0 : fix(l, st);
    };

    auto begin = handle(sl.begin(), dim, fixed<0>);
    auto end = handle(sl.end(), dim, dim);
    auto step = handle(sl.step(), dim, fixed<1>);

    return (step >= 0) ? eval(end - begin, step) : eval(begin + 1_c, step);
  }

  template<concepts::slicer S>
  // requires( !concepts::extremum<S> )
  constexpr auto __restride(auto dim, S const& s)
  {
    auto sl = to_slicer(s);
    if constexpr (is_wildcard<decltype(sl.step())>) return dim;
    else return sl.step() * dim;
  }

  // Computes the offset to add to the base pointer of a container to retrieve the sliced container
  template<shape_descriptor D, storage_order_descriptor C, concepts::slicer... S>
  constexpr auto origin(shape<D> const& s, storage_order_t<C> const& so, S const&... slices) noexcept
  requires(D.ndim == sizeof...(S))
  {
    auto sliced = kumi::map([&](auto shp, auto slc) { return __reindex(shp, slc); }, s,
                            kumi::forward_as_tuple(to_slicer(slices)...));

    auto res = kumi::apply([](auto&&... elts) { return kwk::shape{KWK_FWD(elts)...}; }, sliced);

    return kumi::apply([&](auto&&... elts) { return linearize(kwk::stride{res, so}, elts...); }, sliced);
  }

  template<shape_descriptor D, concepts::slicer... S>
  constexpr auto reshape(shape<D> const& s, S const&... slices) noexcept
  requires(D.ndim == sizeof...(S))
  {
    auto sliced = kumi::map([&](auto shp, auto slc) { return __reshape(shp, slc); }, s,
                            kumi::forward_as_tuple(to_slicer(slices)...));
    return kumi::apply([](auto&&... elts) { return kwk::shape{KWK_FWD(elts)...}; }, sliced);
  }

  template<shape_descriptor D, concepts::slicer... S>
  constexpr auto restride(stride<D> const& s, S const&... slices) noexcept
  requires(D.ndim == sizeof...(S))
  {
    auto sliced = kumi::map([&](auto strd, auto slc) { return __restride(strd, slc); }, s,
                            kumi::forward_as_tuple(to_slicer(slices)...));
    return kumi::apply([](auto&&... elts) { return kwk::stride{KWK_FWD(elts)...}; }, sliced);
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
