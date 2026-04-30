//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kwk/utility/numeric/wildcard.hpp>

namespace kwk::__
{
  //====================================================================================================================
  // Compute a compile-time map of dynamic/static dimensions
  //====================================================================================================================
  template<kumi::concepts::product_type T> consteval auto compute_storage_map()
  {
    struct storage_map
    {
      int data[kumi::size_v<T>] = {};

      consteval int operator[](int i) const { return data[i]; }
    } map;

    int storage_idx = 0, logical_idx = 0;

    auto step = [&]<typename E>(E&) {
      map.data[logical_idx] = !std::is_empty_v<E> ? storage_idx++ : -1;
      logical_idx++;
    };

    auto v = kumi::values_of(T{});
    kumi::for_each(step, v);

    return map;
  }

  //====================================================================================================================
  // Internal type for axis identifier
  //====================================================================================================================
  consteval auto get_digits(std::integral auto n)
  {
    std::size_t size = 1;
    while (n >= 10) n /= 10, size++;
    return size;
  }

  template<int N> struct axis_t : kumi::identifier<axis_t<N>>
  {
    using kumi::identifier<axis_t<N>>::operator=;

    friend constexpr kumi::str to_str(axis_t) noexcept
    {
      constexpr auto size = get_digits(N);

      struct
      {
        char t[size + 6 + 1] = {'a', 'x', 'i', 's', '('};
        std::size_t multiplier = {1};
      } that{};

      [&]<std::size_t... I>(std::index_sequence<I...>) {
        ((that.t[size - I - 1 + 5] = char('0' + ((N / that.multiplier) % 10)), that.multiplier *= 10), ...);
      }(std::make_index_sequence<size>{});

      that.t[size + 5] = ')';
      return kumi::str{that.t};
    }
  };

  //====================================================================================================================
  // Traits for computing the record type with only dynamic axis inside
  //====================================================================================================================
  template<typename T> struct is_dynamic_dim : std::bool_constant<!std::is_empty_v<T>>
  {
  };

  template<typename... Ts>
  using mixed_type = decltype(kumi::filter<is_dynamic_dim>(std::declval<kumi::tuple<Ts...>>()));

  //====================================================================================================================
  //
  //====================================================================================================================
  template<typename... Ts> struct mixed_sequence : private mixed_type<Ts...>
  {
    using base_tuple = kumi::tuple<Ts...>;
    using parent = mixed_type<Ts...>;

    static constexpr auto rank = sizeof...(Ts);
    static constexpr auto stored_rank = kumi::size_v<parent>;
    static constexpr auto mapping = compute_storage_map<base_tuple>();

    template<typename Us> static consteval bool follow_mapping()
    {
      // Checks if the tuple Us can be used to construct the dynamic part of the sequence which means that :
      //  + -1 in mapping means the size is static and the corresponding type in Us... must be an integral constant
      //  with the same value or a wildcard.
      //  + otherwise, the type in Us... must be convertible to the corresponding type in the mixed_type tuple.
      bool valid = true;

      auto check = [&]<std::size_t I>(kumi::index_t<I>) {
        using u_type = std::remove_cvref_t<kumi::element_t<I, Us>>;
        using is_u_dyn = is_dynamic_dim<u_type>;

        if constexpr (mapping[I] == -1)
          return is_wildcard<u_type> || (!is_u_dyn::value && u_type::value == kumi::get<I>(base_tuple{}));
        else return std::convertible_to<u_type, kumi::element_t<mapping[I], parent>>;
      };

      [&]<std::size_t... I>(std::index_sequence<I...>) {
        valid = (valid && ... && check(kumi::index<I>));
      }(std::make_index_sequence<rank>{});

      return valid;
    }

    template<typename... Us> static constexpr auto pre_convert(Us... us)
    {
      auto base = kumi::tuple{us...};

      // Build the tuple of arguments to forward to the parent constructor by following the mapping :
      //  + -1 in mapping means the size is static, so we return the corresponding us as is.
      //  + otherwise, we convert the corresponding us to the type in the mixed_type tuple.
      auto convert = [&]<std::size_t I>(kumi::index_t<I>, auto m) {
        if constexpr (mapping[I] == -1) return m;
        else return static_cast<kumi::element_t<mapping[I], parent>>(m);
      };

      return kumi::map_index(convert, base);
    }

    constexpr mixed_sequence() = default;

    template<typename... Us>
    KWK_TRIVIAL constexpr mixed_sequence(Us const&... us) : parent(kumi::filter<is_dynamic_dim>(pre_convert(us...)))
    {
    }

    // template<typename C, typename Ct>
    // auto& display(std::basic_ostream<C,Ct>& os, auto sep = " ") const
    // {
    //   [&]<std::size_t... N>(std::index_sequence<N...>)
    //   {
    //     auto p = [&]<typename I>(I)
    //     {
    //       if(I::value) os << sep;
    //       if constexpr(mapping[I::value]==-1) os <<  kumi::get<I::value>(base_tuple{});
    //       else os << kumi::get<mapping[I::value]>(static_cast<parent const&>(*this));
    //     };
    //     (p(kumi::index<N>),...);
    //   }(std::make_index_sequence<rank>{});
    //   return os;
    // }

    // template<typename C, typename Ct>
    // friend auto& operator<<(std::basic_ostream<C,Ct>& os, mixed_sequence const& ms)
    // {
    //   return os << static_cast<mixed_type<Ts...> const&>(ms);
    // }

    template<std::size_t I, typename P> KWK_TRIVIAL static constexpr decltype(auto) get_impl(P&& p)
    {
      if constexpr (mapping[I] == -1) return kumi::get<I>(base_tuple{});
      else return kumi::get<mapping[I]>(KWK_FWD(p));
    }

    template<std::size_t I> KWK_TRIVIAL friend constexpr decltype(auto) get(mixed_sequence& s)
    {
      return get_impl<I>(static_cast<parent&>(s));
    }

    template<std::size_t I> KWK_TRIVIAL friend constexpr decltype(auto) get(mixed_sequence&& s)
    {
      return get_impl<I>(static_cast<parent&&>(s));
    }

    template<std::size_t I> KWK_TRIVIAL friend constexpr decltype(auto) get(mixed_sequence const& s)
    {
      return get_impl<I>(static_cast<parent const&>(s));
    }
  };

  template<typename... Ts> mixed_sequence(Ts const&...) -> mixed_sequence<Ts...>;
}

//======================================================================================================================
// Structured Binding Support
//======================================================================================================================
template<typename... Dims>
struct std::tuple_size<kwk::__::mixed_sequence<Dims...>> : std::integral_constant<std::size_t, sizeof...(Dims)>
{
};

template<std::size_t I, typename... Dims> struct std::tuple_element<I, kwk::__::mixed_sequence<Dims...>>
{
  using type = kumi::stored_element_t<I, typename kwk::__::mixed_sequence<Dims...>::base_tuple>;
};
