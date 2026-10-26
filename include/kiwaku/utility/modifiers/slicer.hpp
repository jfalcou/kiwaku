//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kiwaku/utility/numeric/fixed.hpp>

namespace kwk
{
  namespace __
  {
    // @brief helper to deduplicate code
    template<typename S, typename D> constexpr auto handle(S const& s, D dim, auto def)
    {
      if constexpr(std::is_invocable_v<S, D>) return s(dim);
      else if constexpr(kwk::concepts::integral_arithmetic_value<S>) return s;
      else return def;
    }
  }

  template<typename Start, typename Stop, typename Step>
  struct slicer : private __::compressed_tuple<Start, Stop, Step>
  {
    using parent = __::compressed_tuple<Start, Stop, Step>;

    constexpr slicer(Start b, Stop e, Step s)
        : parent {b, e, s}
    {
    }

    constexpr slicer(Start b, kwk::wildcard_t const&, Step s)
        : parent {b, kwk::end, s}
    {
    }

    constexpr slicer(Start b, Stop e, kwk::wildcard_t const&)
        : parent {b, e, fixed<1>}
    {
    }

    constexpr slicer(kwk::wildcard_t const&, Stop e, Step s)
        : parent {fixed<0>, e, s}
    {
    }

    constexpr slicer(Start b, kwk::wildcard_t const&, kwk::wildcard_t const&)
        : parent {b, kwk::end, fixed<1>}
    {
    }

    constexpr slicer(kwk::wildcard_t const&, Stop e, kwk::wildcard_t const&)
        : parent {fixed<0>, e, fixed<1>}
    {
    }

    constexpr slicer(kwk::wildcard_t const&, kwk::wildcard_t const&, Step s)
        : parent {fixed<0>, kwk::end, s}
    {
    }

    constexpr slicer(kwk::wildcard_t const&, kwk::wildcard_t const&, kwk::wildcard_t const&)
        : parent {fixed<0>, kwk::end, fixed<1>}
    {
    }

    /*
      Iterator Interface
    */
    struct iterator
    {
      using value_type      = std::size_t;
      using difference_type = std::ptrdiff_t;

      value_type          i;
      value_type          s;

      constexpr bool      operator==(iterator const& b) const noexcept { return i >= b.i; }

      constexpr auto      operator<=>(iterator const& b) const noexcept { return i <=> b.i; }

      constexpr iterator& operator++() noexcept
      {
        i += s;
        return *this;
      }

      constexpr iterator& operator--() noexcept
      {
        i -= s;
        return *this;
      }

      constexpr iterator operator++(int) noexcept
      {
        auto copy = *this;
        ++*this;
        return copy;
      }

      constexpr iterator operator--(int) noexcept
      {
        auto copy = *this;
        --*this;
        return copy;
      }

      friend constexpr auto operator+(iterator a, difference_type b) noexcept
      {
        return iterator {a.i + (b * a.s), a.s};
      }

      friend constexpr auto operator-(iterator a, difference_type b) noexcept
      {
        return iterator {a.i - (b * a.s), a.s};
      }

      friend constexpr auto operator+(difference_type b, iterator a) noexcept
      {
        return iterator {a.i + (b * a.s), a.s};
      }

      constexpr iterator& operator+=(difference_type b) noexcept
      {
        i += (b * s);
        return *this;
      }

      constexpr iterator& operator-=(difference_type b) noexcept
      {
        i -= (b * s);
        return *this;
      }

      constexpr difference_type operator-(iterator const& b) const noexcept
      {
        return (static_cast<difference_type>(i) - static_cast<difference_type>(b.i)) /
               static_cast<difference_type>(s);
      }

      constexpr auto operator*() const noexcept { return i; }

      constexpr auto operator[](difference_type b) const noexcept { return i + (b * s); }
    };

    /*
      Properties
    */
    constexpr auto     start(this auto&& self) noexcept { return get<0>(KWK_FWD(self)); }

    constexpr auto     stop(this auto&& self) noexcept { return get<1>(KWK_FWD(self)); }

    constexpr auto     step(this auto&& self) noexcept { return get<2>(KWK_FWD(self)); }

    constexpr iterator begin(this auto&& self) noexcept
    {
      return {.i = static_cast<iterator::value_type>(self.process_start()),
              .s = static_cast<iterator::value_type>(self.step())};
    }

    constexpr iterator end(this auto&& self) noexcept
    {
      static_assert(!kwk::concepts::extremum<Stop>,
                    "[KWK] - Can't iterate over a slicer with non-finite upper bound");
      return {.i = static_cast<iterator::value_type>(self.stop()),
              .s = static_cast<iterator::value_type>(self.step())};
    }

    template<typename Self> constexpr auto process_start(this Self&& self) noexcept
    {
      if constexpr(kwk::concepts::extremum<Start>) return self.start()(0);
      else return self.start();
    }

    template<typename Self, typename E> constexpr auto process_end(this Self&& self, E e) noexcept
    {
      bool cond = self.stop() < e;
      if constexpr(!kwk::concepts::extremum<Stop> && kwk::concepts::extremum<E>)
        return e(self.stop() - self.process_start() / self.step());
      else return self.stop() * cond + e * (1 - cond);
    }

    template<typename Self, kwk::concepts::slicer Slc>
    constexpr auto operator[](this Self&& self, Slc s) noexcept
    {
      return self[ to_slicer(s) ];
    }

    template<typename Self, typename B, typename E, typename S>
    constexpr auto operator[](this Self&& self, slicer<B, E, S> const& slc) noexcept
    {
      return kwk::slicer {self.process_start() + slc.process_start() * self.step(),
                          self.process_end(slc.stop()),
                          self.step() * slc.step()};
    }

    /*
      Product Type Interface
    */
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

    /*
      Streaming operator
    */
    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                         slicer const& s) noexcept
    {
      return os << s.start() << ':' << s.stop() << ':' << s.step();
    }
  };

  // @brief Deduction guide
  template<typename B, typename E, typename S> slicer(B, E, S) -> slicer<B, E, S>;

  template<typename B, typename S>
  slicer(B, kwk::wildcard_t const&, S) -> slicer<B, decltype(kwk::end), S>;
  template<typename B, typename E>
  slicer(B, E, kwk::wildcard_t const&) -> slicer<B, E, constant<1>>;
  template<typename E, typename S>
  slicer(kwk::wildcard_t const&, E, S) -> slicer<constant<0>, E, S>;

  template<typename B>
  slicer(B, kwk::wildcard_t const&, kwk::wildcard_t const&)
      -> slicer<B, decltype(kwk::end), constant<1>>;
  template<typename E>
  slicer(kwk::wildcard_t const&, E, kwk::wildcard_t const&) -> slicer<constant<0>, E, constant<1>>;
  template<typename S>
  slicer(kwk::wildcard_t const&, kwk::wildcard_t const&, S)
      -> slicer<constant<0>, decltype(kwk::end), S>;

  slicer(kwk::wildcard_t const&, kwk::wildcard_t const&, kwk::wildcard_t const&)
      -> slicer<constant<0>, decltype(kwk::end), constant<1>>;

  // @brief builds a slicer from a type
  template<typename B, typename E, typename S>
  KWK_ABI constexpr auto to_slicer(slicer<B, E, S> const& s) noexcept
  {
    return s;
  }

  template<typename T>
    requires(kwk::concepts::integral_arithmetic_value<T> && !is_wildcard_v<T>)
  constexpr auto to_slicer(T const& t) noexcept
  {
    return slicer {t, t + fixed<1>, fixed<1>};
  }

  constexpr auto to_slicer(kwk::concepts::extremum auto const& t) noexcept
  {
    return slicer {t.offset(), t.ratio(), fixed<1>};
  }

  template<typename T>
  constexpr auto to_slicer(T const&) noexcept
    requires(is_wildcard_v<std::remove_cvref_t<T>>)
  {
    return slicer {fixed<0>, kwk::end, fixed<1>};
  }

  template<typename B, typename E, typename S>
  constexpr auto slice_extent(auto dim, slicer<B, E, S> const& slc)
  {
    auto eval = [ & ]<typename Len, typename Step>(Len l, Step st)
    {
      auto fix = [ & ](auto v, auto n)
      {
        auto w = (n < 0) ? -n : n;
        return (v % w != 0) + v / w;
      };
      if constexpr(concepts::static_constant<Len> && concepts::static_constant<Step>)
        return fixed < (Step::value == 0) ? 0 : fix(Len::value, Step::value) > ;
      else return (st == 0) ? 0 : fix(l, st);
    };

    auto b = __::handle(slc.start(), dim, fixed<0>);
    auto e = __::handle(slc.stop(), dim, dim);
    auto s = __::handle(slc.step(), dim, fixed<1>);

    return (s >= 0) ? eval(e - b, s) : eval(b + 1_c, s);
  }

  template<typename B, typename E, typename S>
  constexpr auto slice_stride(auto strd, slicer<B, E, S> const& slc)
  {
    if constexpr(is_wildcard_v<std::remove_cvref_t<S>>) return strd;
    else return slc.step() * strd;
  }

  // Computes the offset to add to the base pointer of a container to retrieve the sliced container
  template<kwk::concepts::shape Shape, concepts::slicer... S>
  constexpr auto origin(Shape&& s, kumi::tuple<S...> const& sl) noexcept
    requires(kumi::size_v<Shape> == sizeof...(S))
  {
    return kumi::apply(
        [](auto&&... elts)
        {
          return kumi::tuple {
              __::handle(get<1>(KWK_FWD(elts)).start(), get<0>(KWK_FWD(elts)), fixed<0>)...};
        },
        kumi::zip(kumi::to_tuple(s), sl));
  }

  template<kwk::concepts::shape Shape, concepts::slicer... S>
  constexpr auto origin(Shape&& shp, S const&... sl) noexcept
    requires(kumi::size_v<Shape> == sizeof...(S))
  {
    return origin(shp, kumi::make_tuple(to_slicer(sl)...));
  }

  /*
    Slicer shortcuts
  */
  constexpr auto at(std::integral auto i) { return kwk::slicer {i, i + 1, fixed<1>}; }

  constexpr auto every(std::integral auto s) { return kwk::slicer {fixed<0>, kwk::end, s}; }

  constexpr auto first(std::integral auto i) { return kwk::slicer {fixed<0>, i, fixed<1>}; }

  constexpr auto last(std::integral auto i)
  {
    return kwk::slicer {kwk::end - i, kwk::end, fixed<1>};
  }

  constexpr auto drop_first(std::integral auto i) { return kwk::slicer {i, kwk::end, fixed<1>}; }

  constexpr auto drop_last(std::integral auto i)
  {
    return kwk::slicer {fixed<0>, kwk::end - i, fixed<1>};
  }

  template<typename Start, typename Stop> constexpr auto between(Start b, Stop e)
  {
    return kwk::slicer {b, e, fixed<1>};
  }
}

#if !defined(KWK_DOXYGEN_INVOKED)
template<typename B, typename E, typename S>
struct std::tuple_size<kwk::slicer<B, E, S>> : std::integral_constant<std::size_t, 3>
{
};

template<std::size_t I, typename B, typename E, typename S>
struct std::tuple_element<I, kwk::slicer<B, E, S>>
{
  using type = decltype(get<I>(std::declval<kwk::slicer<B, E, S>>()));
};

namespace kwk
{
  // Catch errors and make diagnostics clearer
  template<typename Start, typename Stop, typename Step>
    requires((!kwk::concepts::integral_arithmetic_value<Start> && !kwk::concepts::extremum<Start> &&
              !kwk::is_wildcard_v<Start>) &&
             (!kwk::concepts::integral_arithmetic_value<Stop> && !kwk::concepts::extremum<Stop> &&
              !kwk::is_wildcard_v<Stop>) &&
             (!kwk::concepts::integral_arithmetic_value<Step> && !kwk::is_wildcard_v<Step>))
  struct slicer<Start, Stop, Step>
  {
    constexpr slicer(Start&& b, Stop&& e, Step&& s) = delete;

    static constexpr bool valid_start =
        (kwk::concepts::integral_arithmetic_value<Start> || kwk::concepts::extremum<Start>);
    static constexpr bool valid_stop =
        (kwk::concepts::integral_arithmetic_value<Stop> || kwk::concepts::extremum<Stop>);
    static constexpr bool valid_step = kwk::concepts::integral_arithmetic_value<Step>;

    static_assert(valid_start,
                  "[KWK] - Start must either model kwk::concepts::arithmetic_integral_value or "
                  "kwk::concepts::extremum");
    static_assert(valid_stop,
                  "[KWK] - Stop must either model kwk::concepts::arithmetic_integral_value or "
                  "kwk::concepts::extremum");
    static_assert(valid_step, "[KWK] - Step must model kwk::concepts::arithmetic_integral_value");
  };
}
#endif
