//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kiwaku/utility/numeric/wildcard.hpp>

namespace kwk::__
{
  //====================================================================================================================
  // Compute a compile-time map of dynamic/static dimensions
  //====================================================================================================================
  template<kumi::concepts::product_type T> consteval auto compute_storage_map()
  {
    struct storage_map
    {
      int           data[ kumi::size_v<T> ] = {};

      consteval int operator[](int i) const { return data[ i ]; }
    } map;

    int  storage_idx = 0, logical_idx = 0;

    auto step = [ & ]<typename E>(E&)
    {
      map.data[ logical_idx ] = !std::is_empty_v<E> ? storage_idx++ : -1;
      logical_idx++;
    };

    auto v = kumi::values_of(T {});
    kumi::for_each(step, v);

    return map;
  }

  // When every element is empty (purely static), the storage layer collapses to a single empty
  // product type. It must be *distinct per instantiation* (not the shared `kumi::tuple<>`), so that
  // two compressed tuples can still be empty-base-optimized together. If shape and stride both
  // inherited a common `kumi::tuple<>` base, the layout aggregating them could not EBO the two
  // empty bases (two subobjects of the same type may not overlap), and even a fully-static layout
  // would not collapse to a single byte.
  template<typename... Ts> struct empty_product
  {
    template<typename... Us> constexpr empty_product(Us&&...) noexcept {}
  };

  template<typename... Ts>
  using mixed_type = std::conditional_t<(std::is_empty_v<Ts> && ...),
                                        empty_product<Ts...>,
                                        decltype(kumi::filter_not<std::is_empty>(
                                            std::declval<kumi::tuple<Ts...>>()))>;

  //====================================================================================================================
  //
  //====================================================================================================================
  template<typename... Ts> struct compressed_tuple : mixed_type<Ts...>
  {
    using base_tuple                  = kumi::tuple<Ts...>;
    using flat_tuple                  = kumi::result::flatten_all_t<base_tuple>;
    using parent                      = mixed_type<Ts...>;

    static constexpr auto rank        = sizeof...(Ts);
    static constexpr auto stored_rank = kumi::size_v<parent>;
    static constexpr auto mapping     = compute_storage_map<base_tuple>();

    template<kumi::concepts::product_type U> static consteval bool follow_mapping()
    {
      // Checks if the tuple U can be used to construct the dynamic part of the sequence which means
      // that :
      //  + -1 in mapping means the size is static and the corresponding type in U must be an
      //  integral constant with the same value or a wildcard.
      //  + otherwise, the type in U must be convertible to the corresponding type in the mixed_type
      //  tuple.
      bool valid = true;

      auto check = [ & ]<std::size_t I>(kumi::index_t<I>)
      {
        using u_type   = std::remove_cvref_t<kumi::element_t<I, U>>;
        using is_u_dyn = std::is_empty<u_type>;

        if constexpr(mapping[ I ] == -1)
          return (std::same_as<u_type, wildcard_t> ||
                  (!is_u_dyn::value && u_type::value == kumi::get<I>(U {})));
        else return std::convertible_to<u_type, kumi::element_t<mapping[ I ], parent>>;
      };

      [ & ]<std::size_t... I>(std::index_sequence<I...>)
      { valid = (valid && ... && check(kumi::index<I>)); }(std::make_index_sequence<rank> {});

      return valid;
    }

    template<typename... Us> static constexpr auto pre_convert(Us&&... us)
    {
      auto base    = kumi::tuple {KWK_FWD(us)...};

      // Build the tuple of arguments to forward to the parent constructor by following the mapping
      // :
      //  + -1 in mapping means the element is empty, so we return the corresponding us as is.
      //  + otherwise, we convert the corresponding us to the type in the mixed_type tuple.
      auto convert = [ & ]<std::size_t I>(kumi::index_t<I>, auto m)
      {
        if constexpr(mapping[ I ] == -1) return m;
        else return static_cast<kumi::element_t<mapping[ I ], parent>>(m);
      };

      return kumi::map_index(convert, base);
    }

    constexpr compressed_tuple() = default;

    template<typename... Us>
      requires(... && !std::is_base_of_v<compressed_tuple, std::decay_t<Us>>)
    KWK_ABI constexpr compressed_tuple(Us&&... us)
        : parent {kumi::filter_not<std::is_empty>(pre_convert(KWK_FWD(us)...))}
    {
    }

    template<typename C, typename Ct>
    friend auto& operator<<(std::basic_ostream<C, Ct>& os, compressed_tuple const& ms)
    {
      os << "(";
      kumi::for_each([ & ](auto e) { os << " " << e; }, ms);
      return os << " )";
    }

    template<std::size_t I>
      requires(I < kumi::size_v<flat_tuple>)
    KWK_ABI constexpr decltype(auto) operator[](this auto&& self, kumi::index_t<I>) noexcept
    {
      if constexpr(mapping[ I ] == -1) return kumi::element_t<I, base_tuple> {};
      else
      {
        // View `self` as its `parent` subobject with the SAME cv- and ref-qualifiers, so element
        // access is const-correct and ref-correct: a const lvalue tuple keeps `const E&`, a
        // non-const lvalue gets a mutable `E&`, and an rvalue tuple yields `E&&`. (Casting straight
        // to `parent&` would silently drop const on a const object, which is ill-formed.)
        using self_ref  = decltype(self);
        using plain     = std::remove_reference_t<self_ref>;
        using parent_cv = std::conditional_t<std::is_const_v<plain>, parent const, parent>;
        using parent_view =
            std::conditional_t<std::is_lvalue_reference_v<self_ref>, parent_cv&, parent_cv&&>;
        return kumi::get<mapping[ I ]>(static_cast<parent_view>(self));
      }
    }

    template<std::size_t I> KWK_ABI friend constexpr decltype(auto) get(compressed_tuple& s)
    {
      return s[ kumi::index<I> ];
    }

    template<std::size_t I> KWK_ABI friend constexpr decltype(auto) get(compressed_tuple&& s)
    {
      return static_cast<compressed_tuple&&>(s)[ kumi::index<I> ];
    }

    template<std::size_t I> KWK_ABI friend constexpr decltype(auto) get(compressed_tuple const& s)
    {
      return s[ kumi::index<I> ];
    }
  };

  template<> struct compressed_tuple<>
  {
    static constexpr auto rank        = 0;
    static constexpr auto stored_rank = 0;
  };

  template<typename... Ts>
  compressed_tuple(Ts&&...) -> compressed_tuple<std::unwrap_ref_decay_t<Ts>...>;

  template<typename T> KWK_ABI consteval auto extract_map(std::size_t N)
  {
    if constexpr(kumi::concepts::product_type<T>)
      return [ & ]<std::size_t... I>(std::index_sequence<I...>)
      {
        return kumi::tuple {extract_map<kumi::element_t<I, T>>(I)...};
      }(std::make_index_sequence<kumi::size_v<T>> {});
    else return N;
  }

  template<auto t> KWK_ABI consteval auto as_projection_map()
  {
    if constexpr(kumi::concepts::product_type<decltype(t)>)
      return [ & ]<std::size_t... I>(std::index_sequence<I...>)
      {
        return kumi::projection_map {as_projection_map<get<I>(t)>()...};
      }(std::make_index_sequence<kumi::size_v<decltype(t)>> {});
    else return kumi::index<t>;
  }

  template<kumi::concepts::product_type Target, kumi::concepts::product_type T>
  [[nodiscard]] KWK_ABI constexpr decltype(auto) layout_cast(T&& t) noexcept
  {
    using flat_t             = kumi::result::flatten_all_t<T>;
    constexpr auto t_1       = kwk::__::extract_map<flat_t>(0);
    using t_2                = decltype(kwk::__::extract_map<Target>(0));

    constexpr auto final_map = as_projection_map<std::bit_cast<t_2>(t_1)>();
    using skeletton_t        = kumi::result::reindex_t<flat_t, final_map>;

    static_assert(sizeof(skeletton_t) == sizeof(T),
                  "[KWK] - Cannot layout_cast product types of incompatible size");

    return std::bit_cast<skeletton_t>(KUMI_FWD(t));
  }
}

//======================================================================================================================
// Structured Binding Support
//======================================================================================================================
template<typename... Ts>
struct std::tuple_size<kwk::__::empty_product<Ts...>> : std::integral_constant<std::size_t, 0>
{
};

template<typename... Dims>
struct std::tuple_size<kwk::__::compressed_tuple<Dims...>>
    : std::integral_constant<std::size_t, sizeof...(Dims)>
{
};

template<std::size_t I, typename... Dims>
struct std::tuple_element<I, kwk::__::compressed_tuple<Dims...>>
{
  using type = kumi::stored_element_t<I, typename kwk::__::compressed_tuple<Dims...>::base_tuple>;
};

#ifndef KWK_DOXYGEN_INVOKED
// Builder protocole
template<typename... Ts> struct kumi::builder<kwk::__::compressed_tuple<Ts...>>
{
  using type                        = kwk::__::compressed_tuple<Ts...>;

  template<typename... Us> using to = kwk::__::compressed_tuple<Us...>;

  template<typename... Args> [[nodiscard]] KUMI_ABI static constexpr auto make(Args&&... args)
  {
    return kwk::__::compressed_tuple(KUMI_FWD(args)...);
  }

  template<typename... Args> [[nodiscard]] KUMI_ABI static constexpr auto build(Args&&... args)
  {
    return kwk::__::compressed_tuple {KUMI_FWD(args)...};
  }
};

#endif
