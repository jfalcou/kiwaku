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
  // consteval auto get_digits(std::integral auto n)
  //{
  //  std::size_t size = 1;
  //  while (n >= 10) n /= 10, size++;
  //  return size;
  //}

  // template<int N> struct axis_t : kumi::identifier<axis_t<N>>
  //{
  //   using kumi::identifier<axis_t<N>>::operator=;

  //  friend constexpr kumi::str to_str(axis_t) noexcept
  //  {
  //    constexpr auto size = get_digits(N);

  //    struct
  //    {
  //      char t[size + 6 + 1] = {'a', 'x', 'i', 's', '('};
  //      std::size_t multiplier = {1};
  //    } that{};

  //    [&]<std::size_t... I>(std::index_sequence<I...>) {
  //      ((that.t[size - I - 1 + 5] = char('0' + ((N / that.multiplier) % 10)), that.multiplier *= 10), ...);
  //    }(std::make_index_sequence<size>{});

  //    that.t[size + 5] = ')';
  //    return kumi::str{that.t};
  //  }
  //};

  //====================================================================================================================
  // Traits for computing the record type with only dynamic axis inside
  //====================================================================================================================
  template<typename T> struct empty_class : std::bool_constant<!std::is_empty_v<T>>
  {
  };

  template<typename... Ts> using mixed_type = decltype(kumi::filter<empty_class>(std::declval<kumi::tuple<Ts...>>()));

  //====================================================================================================================
  //
  //====================================================================================================================
  template<typename... Ts> struct compressed_tuple : mixed_type<Ts...>
  {
    using base_tuple = kumi::tuple<Ts...>;
    using flat_tuple = kumi::result::flatten_all_t<base_tuple>;
    using parent = mixed_type<Ts...>;

    static constexpr auto rank = sizeof...(Ts);
    static constexpr auto stored_rank = kumi::size_v<parent>;
    static constexpr auto mapping = compute_storage_map<base_tuple>();

    template<kumi::concepts::product_type U> static consteval bool follow_mapping()
    {
      // Checks if the tuple U can be used to construct the dynamic part of the sequence which means that :
      //  + -1 in mapping means the size is static and the corresponding type in U must be an integral constant
      //  with the same value or a wildcard.
      //  + otherwise, the type in U must be convertible to the corresponding type in the mixed_type tuple.
      bool valid = true;

      auto check = [&]<std::size_t I>(kumi::index_t<I>) {
        using u_type = std::remove_cvref_t<kumi::element_t<I, U>>;
        using is_u_dyn = empty_class<u_type>;

        if constexpr (mapping[I] == -1)
          return (std::same_as<u_type, wildcard_t> || (!is_u_dyn::value && u_type::value == kumi::get<I>(U{})));
        else return std::convertible_to<u_type, kumi::element_t<mapping[I], parent>>;
      };

      [&]<std::size_t... I>(std::index_sequence<I...>) {
        valid = (valid && ... && check(kumi::index<I>));
      }(std::make_index_sequence<rank>{});

      return valid;
    }

    template<typename... Us> static constexpr auto pre_convert(Us&&... us)
    {
      auto base = kumi::tuple{KWK_FWD(us)...};

      // Build the tuple of arguments to forward to the parent constructor by following the mapping :
      //  + -1 in mapping means the element is empty, so we return the corresponding us as is.
      //  + otherwise, we convert the corresponding us to the type in the mixed_type tuple.
      auto convert = [&]<std::size_t I>(kumi::index_t<I>, auto m) {
        if constexpr (mapping[I] == -1) return m;
        else return static_cast<kumi::element_t<mapping[I], parent>>(m);
      };

      return kumi::map_index(convert, base);
    }

    constexpr compressed_tuple() = default;

    template<typename... Us>
    requires(... && !std::is_base_of_v<compressed_tuple, std::decay_t<Us>>)
    KWK_TRIVIAL constexpr compressed_tuple(Us&&... us) : parent{kumi::filter<empty_class>(pre_convert(KWK_FWD(us)...))}
    {
    }

    template<typename C, typename Ct> friend auto& operator<<(std::basic_ostream<C, Ct>& os, compressed_tuple const& ms)
    {
      os << "(";
      kumi::for_each([&](auto e) { os << " " << e; }, ms);
      return os << " )";
    }

    KWK_TRIVIAL constexpr decltype(auto) flatten(this auto&& self) noexcept
    {
      constexpr auto as_flat = [](auto&&... elts) { return kwk::__::compressed_tuple{KWK_FWD(elts)...}; };
      using flat_t = kumi::result::apply_t<decltype(as_flat), flat_tuple>;

      return std::bit_cast<flat_t>(self);
    }

    template<std::size_t I>
    requires(I < kumi::size_v<flat_tuple>)
    KWK_TRIVIAL constexpr decltype(auto) operator[](this auto&& self, kumi::index_t<I>) noexcept
    {
      if constexpr (mapping[I] == -1) return kumi::element_t<I, base_tuple>{};
      // Tbh not sure about the forward_like of cast to const& here...
      else return kumi::get<mapping[I]>(std::forward_like<decltype(self)>(static_cast<parent const&>(self)));
    }

    template<std::size_t I> KWK_TRIVIAL friend constexpr decltype(auto) get(compressed_tuple& s)
    {
      return s[kumi::index<I>];
    }

    template<std::size_t I> KWK_TRIVIAL friend constexpr decltype(auto) get(compressed_tuple&& s)
    {
      return static_cast<compressed_tuple&&>(s)[kumi::index<I>];
    }

    template<std::size_t I> KWK_TRIVIAL friend constexpr decltype(auto) get(compressed_tuple const& s)
    {
      return s[kumi::index<I>];
    }
  };

  template<> struct compressed_tuple<>
  {
    static constexpr auto rank = 0;
    static constexpr auto stored_rank = 0;
  };

  template<typename... Ts> compressed_tuple(Ts&&...) -> compressed_tuple<std::unwrap_ref_decay_t<Ts>...>;
}

//======================================================================================================================
// Structured Binding Support
//======================================================================================================================
template<typename... Dims>
struct std::tuple_size<kwk::__::compressed_tuple<Dims...>> : std::integral_constant<std::size_t, sizeof...(Dims)>
{
};

template<std::size_t I, typename... Dims> struct std::tuple_element<I, kwk::__::compressed_tuple<Dims...>>
{
  using type = kumi::stored_element_t<I, typename kwk::__::compressed_tuple<Dims...>::base_tuple>;
};

#ifndef KWK_DOXYGEN_INVOKED
// Builder protocole
template<typename... Ts> struct kumi::builder<kwk::__::compressed_tuple<Ts...>>
{
  using type = kwk::__::compressed_tuple<Ts...>;

  template<typename... Us> using to = kwk::__::compressed_tuple<Us...>;

  template<typename... Args> [[nodiscard]] KUMI_ABI static constexpr auto make(Args&&... args)
  {
    return kwk::__::compressed_tuple(KUMI_FWD(args)...);
  }

  template<typename... Args> [[nodiscard]] KUMI_ABI static constexpr auto build(Args&&... args)
  {
    return kwk::__::compressed_tuple{KUMI_FWD(args)...};
  }
};

#endif
