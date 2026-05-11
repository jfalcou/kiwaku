//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

namespace kwk
{
  //====================================================================================================================
  /**
    @ingroup shape-utility
    @brief Stride of multi-dimensional container (Supports Arbitrary Layouts)
  **/
  //====================================================================================================================
  template<auto... D> struct stride : private __::as_sequence<D...>::type
  {
    using element_type    = stride;
    using type            = stride;
    using identifier_type = __::stride_id;
    using label_type      = kumi::str;
    using size_type       = kwk::config::default_size_type;

    constexpr auto operator()(identifier_type const&) const { return *this; }

    static constexpr label_type label() { return kumi::str{"Stride"}; }

    //static constexpr auto descriptor = Descriptor;
    static constexpr auto ndim = sizeof...(D);

    /// @brief Dynamic property of this shape
    static constexpr bool fully_dynamic = ((D == kwk::_) && ...); 

    using storage_type = __::as_sequence<D...>::type;

    constexpr stride() = default;

    template<std::convertible_to<size_type>... S>
    requires(sizeof...(S) == ndim && storage_type::template follow_mapping<kumi::tuple<S...>>())
    constexpr stride(S... s) : storage_type{s...}
    {
    }

    template<auto... StrideDims> friend constexpr bool operator==(stride const& a, stride<StrideDims...> const& b) noexcept
    {
      if constexpr (stride::ndim != stride<StrideDims...>::ndim) return false;
      else return kumi::to_tuple(a) == kumi::to_tuple(b);
    }

    storage_type const& self() const { return static_cast<storage_type const&>(*this); }

    storage_type& self() { return static_cast<storage_type&>(*this); }

    template<std::size_t I> KWK_TRIVIAL friend constexpr decltype(auto) get(stride const& s)
    {
      if constexpr (I >= stride::ndim) return kwk::fixed<0>;
      else return get<I>(s.self());
    }

    template<std::size_t I> KWK_TRIVIAL friend constexpr decltype(auto) get(stride& s)
    {
      if constexpr (I >= stride::ndim) return kwk::fixed<0>;
      else return get<I>(s.self());
    }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                         stride const& s) noexcept
    {
      os << "(";
      kumi::for_each([&](auto e) { os << " " << +e; }, s);
      return os << " )";
    }
  };

  template<concepts::extent... S>
  requires(sizeof...(S) > 0)
  stride(S...) -> stride<__::make_dimension<std::unwrap_ref_decay_t<S>>()...>;

  /// @brief Transforms an abritrary product type into a stride
  template<kumi::concepts::product_type T>
  constexpr auto as_stride(T&& t)
  {
    return kumi::apply([](auto &&... elt)
    {
      auto v_or_t = []<typename V>(V && v)
      {
        if constexpr( kumi::concepts::product_type<V> ) return to_stride(KWK_FWD(v));
        else return KWK_FWD(v);
      };
      return stride{v_or_t(KWK_FWD(elt))...};
    }, KWK_FWD(t));
  }

 //==================================================================================================================
 // Compute a stride from a product type representing the shape and a custom layout permutation at compile time
 //==================================================================================================================
  template<kumi::concepts::product_type T, storage_order_descriptor order>
  constexpr auto to_stride(T && t, storage_order_t<order> const&)
  {
    if constexpr (kumi::concepts::empty_product_type<T>) return kwk::stride{};
    else 
    {
      constexpr auto N = kumi::size_v<T>;
      constexpr auto perm = kumi::generate<N>([&](auto i)
      {
        return kumi::index<order.generator(i, N)>;
      });

      auto permuted = kumi::apply([&]<std::size_t... I>(kumi::index_t<I>...)
      {
        return kumi::reorder<I...>(KUMI_FWD(t));
      }, perm);
      
      auto tmp = kumi::exclusive_scan_right(kumi::function::multiplies, permuted, fixed<1>);

      return as_stride(kumi::generate<N>([&](auto i)
      {
        return kumi::get<order.generator(i,N)>(tmp);
      }));
    }
  }

  // @brief : computes the linear position considering a stride and a tuple representing the MD position by
  // performing a dot product between them
  template<auto... StrideDims, kumi::concepts::product_type T>
  KWK_TRIVIAL constexpr auto linearize(stride<StrideDims...> const& s, T&& t)
  {
    return kumi::inner_product(s, KWK_FWD(t), 0);
  }

  //@brief utility to linearize a position based on the stride
  template<auto... StrideDims, std::convertible_to<std::ptrdiff_t>... Is>
  KWK_TRIVIAL constexpr auto linearize(stride<StrideDims...> const& s, Is... is) noexcept
  requires(sizeof...(Is) == sizeof...(StrideDims))
  {
    // conversion to ptrdiff_t is a bit to harsh, it avoid conversions warning from unsigned position to
    // signed one when going through kumi::inner_product
    return linearize(s, kumi::tuple{std::ptrdiff_t(is)...});
  }
}

//======================================================================================================================
// Structured Binding Support
//======================================================================================================================
#if !defined(KWK_DOXYGEN_INVOKED)
template<auto... StrideDims>
struct std::tuple_size<kwk::stride<StrideDims...>> : std::integral_constant<std::size_t, sizeof...(StrideDims)>
{
};

template<std::size_t I, auto... StrideDims> struct std::tuple_element<I, kwk::stride<StrideDims...>>
{
  using type = decltype(get<I>(std::declval<kwk::stride<StrideDims...>>()));
};
#endif
