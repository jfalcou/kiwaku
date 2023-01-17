//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/concepts/axis.hpp>
#include <kwk/detail/abi.hpp>
#include <kwk/detail/kumi.hpp>
#include <kwk/detail/traits.hpp>
#include <kwk/settings/axis.hpp>
#include <ostream>

// Forward declaration & Helpers
namespace kwk
{
  struct joker;
  namespace detail { struct size_; }
}

namespace kwk::detail
{
  //================================================================================================
  // combo sequence allow for constructing sequence of static value and placeholders
  // It behaves as a tuple for easy access/algorithm applications.
  // It is meant to only be used as a constexpr object in shape/stride template parameters
  //================================================================================================
  template<typename T, concepts::axis... Axis>
  struct combo
  {
    using is_product_type = void;
    using base_type       = std::conditional_t<std::same_as<joker,T>,std::ptrdiff_t,T>;
    using contents_type   = kumi::tuple<Axis...>;

    // combo is its self option keyword
    using stored_value_type = combo<T,Axis...>;
    using keyword_type      = detail::size_;
    KWK_FORCEINLINE constexpr auto operator()(keyword_type const&) const noexcept { return *this; }

    // Size info
    static constexpr auto static_size = sizeof...(Axis);
    static constexpr auto size() noexcept { return static_size; }

    // Ctor from list of axis
    constexpr combo(Axis const&... x) : storage{x...} {}

    // Display helper
    friend std::ostream& operator<<(std::ostream& os, combo a)
    {
      os << front(a.storage);
      kumi::for_each([&](auto m) { os << " x " << m; }, pop_front(a.storage));
      return os;
    }

    // Tuple interface
    template<std::size_t N>
    friend constexpr decltype(auto) get(combo& s) noexcept { return get<N>(s.storage); }

    template<std::size_t N>
    friend constexpr decltype(auto) get(combo const& s) noexcept { return get<N>(s.storage); }

    // combo sequences are compatible if they have the same size
    // and don't contain conflicting static values
    template< typename T0, typename... E0>
    constexpr bool is_compatible(combo<T0,E0...> const& other) const noexcept
    {
      if constexpr(static_size != combo<T0,E0...>::static_size)  return false;
      else
      {
        return  kumi::fold_left
                ( [](bool acc, auto const& t)
                  {
                    auto[e,f] = t;
                    return checker(e, f, acc, acc);
                  }
                , kumi::zip(*this,other)
                , true
                );
      }
    }

    // combo sequences are similar if they have the same size
    // and have the same static/dynamic layout
    template< typename T0, typename... E0>
    constexpr bool is_similar(combo<T0,E0...> const& other) const noexcept
    {
      if constexpr(static_size != combo<T0,E0...>::static_size)  return false;
      else
      {
        return kumi::fold_left( [](bool acc, auto const& t)
                                {
                                  auto[e,f] = t;
                                  return checker(e, f, false, acc);
                                }
                              , kumi::zip(*this,other)
                              , true
                              );
      }
    }

    // Helper when we call is_similar in shape/stride require clauses
    template<typename S2>
    constexpr bool is_similar( S2 const& ) const noexcept
    {
      return is_similar(S2::descriptor);
    }

    // combo are equivalent if they have the same order/name of axis
    template< typename T0, typename... E0>
    constexpr bool is_equivalent(combo<T0,E0...> const&) const noexcept
    {
      if constexpr(static_size != combo<T0,E0...>::static_size)  return false;
      else
      {
        return (is_along_v<Axis,E0> && ... );
      }
    }

    template<typename N, typename T0, typename... E0>
    constexpr bool is_equivalent(combo<T0,E0...> const& that, N idx) const noexcept
    {
      auto ref = make_combo<T>(kumi::extract(storage,idx));
      return ref.is_equivalent(that);
    }

    //==============================================================================================
    // Combination expansion operators
    // Those operators helps build combo from user code via either
    //  - the extent generator: extent[4]()() etc
    //  - the * operator to combine with axis
    //==============================================================================================
    KWK_FORCEINLINE constexpr auto operator()() const noexcept
    {
      auto renum = kumi::map([]<typename M>( M m) { return make_axis(m); } , storage);
      return make_combo<T>(push_back(renum,detail::indexed_axis_<0>{}));
    }

    KWK_FORCEINLINE constexpr auto operator[](std::integral auto i) const noexcept
    {
      auto renum = kumi::map([]<typename M>( M m) { return make_axis(m); } , storage);
      return make_combo<T>(push_back(renum,detail::indexed_axis_<0,decltype(i)>{i}));
    }

    KWK_FORCEINLINE
    friend constexpr auto operator*(combo const& l, concepts::axis auto const& o) noexcept
    {
      return make_combo<T>(push_back(l.storage,o));
    }

    KWK_FORCEINLINE
    friend constexpr auto operator*(concepts::axis auto const& o,combo const& l) noexcept
    {
      return make_combo<T>(push_front(l.storage,o));
    }

    template<typename T2, typename... A2>
    KWK_FORCEINLINE
    friend constexpr auto operator*(combo const& lhs,combo<T2,A2...> const& rhs) noexcept
    {
      using type = std::conditional_t<std::same_as<T,kwk::joker>,T2,T>;
      return make_combo<type>(cat(lhs.storage,rhs.storage));
    }

    // Storage tuple
    contents_type storage;

    private:

    template<typename M>
    static KWK_FORCEINLINE constexpr auto make_axis(M m) noexcept
    {
      if constexpr( requires(M) { M::static_index; })
        return detail::indexed_axis_<M::static_index+1, decltype(m.value)>{m.value};
      else
        return m;
    }

    template<typename E, typename F>
    static KWK_FORCEINLINE constexpr bool checker(E e, F f, auto def, auto acc) noexcept
    {
      if constexpr(concepts::dynamic_axis<E>)                                     return acc;
      else if constexpr(!concepts::dynamic_axis<E> && concepts::dynamic_axis<F>)  return def;
      else                                                return acc && (e.value == f.value);
    }
  };

  // Builder function - Used by axis
  template<typename T, typename... Axis> KWK_FORCEINLINE constexpr auto make_combo(Axis... ax)
  {
    return combo<T,Axis...>(ax...);
  }

  template<typename T> KWK_FORCEINLINE constexpr auto make_combo(kumi::product_type auto const& st)
  {
    return kumi::apply( []<typename... M>(M... m) { return  combo<T,M...>(m...); },st);
  }

  // Axis combinator
  KWK_FORCEINLINE
  constexpr auto operator*(concepts::axis auto const& a, concepts::axis auto const& b) noexcept
  {
    return make_combo<kwk::joker>(a, b);
  }

  // Compress combo at a given size, projecting remaining elements
  template<std::size_t N, typename T, typename... E>
  constexpr auto compress(combo<T,E...> s)  noexcept
  {
    constexpr auto sz = combo<T,E...>::static_size;
    if constexpr(N == sz) return s;
    else
    {
      auto [head,tail] = kumi::split(s.storage, kumi::index<sz-N+1>);
      auto value = kumi::fold_right( [](auto acc, auto v) { return compress(acc,v); }, pop_front(head), get<0>(head));
      return make_combo<std::ptrdiff_t>( kumi::push_front( tail, value ) );
    }
  }
}

// Tuple interface adaptation
template<typename T, typename... D>
struct  std::tuple_size<kwk::detail::combo<T, D...>>
      : std::integral_constant<std::size_t,sizeof...(D)>
{};

template<std::size_t N, typename T, typename... D>
struct  std::tuple_element<N, kwk::detail::combo<T,D...>> :
        std::tuple_element<N, kumi::tuple<D...>>
{};
