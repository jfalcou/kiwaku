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
  namespace __
  {
    struct stride_product_t
    {
      template<typename T, typename U>
      constexpr auto operator()(T && t, U && u) const noexcept(noexcept((KWK_FWD(t)-1) * KWK_FWD(u)))
      requires( requires{ (KWK_FWD(t)-1) * KWK_FWD(u); })
      {
        return (KWK_FWD(t)-1) * KWK_FWD(u);
      }
    } inline constexpr stride_product;
  }

  //====================================================================================================================
  /**
    @ingroup layout-utility
    @brief Stride of multi-dimensional container (Supports Arbitrary Layouts)
  **/
  //====================================================================================================================
  template<concepts::deep_extent auto... D> struct stride : __::as_sequence_t<D...>
  {
    using element_type = stride;
    using type = stride;
    using identifier_type = __::stride_id;
    using label_type = kumi::str;
    using size_type = kwk::config::default_size_type;

    template <typename Self>
    constexpr auto&& operator()(this Self&& self, identifier_type const&) {
      using stride_t= std::conditional_t<
        std::is_const_v<std::remove_reference_t<Self>>,
        const stride,
        stride 
      >;
      return std::forward_like<Self>(static_cast<stride_t&>(self));
    }

    static constexpr label_type label() { return kumi::str{"Stride"}; }

    /// @brief Number of dimensions in this stride
    static constexpr size_type ndim = sizeof...(D);

    /// @brief Dynamic property of this stride 
    //static constexpr bool fully_dynamic = ((D == kwk::_) && ...);

    // The cosize of a container is defined as : 1 + sum_(i=0)^(i<N)(shape(i) -1)*stride;
    // which computes the global interval [0, memory_size)     
    template<kumi::concepts::product_type T>
    constexpr size_type cosize(T && t) const noexcept
    {
      return kumi::inner_product(kumi::flatten_all(KWK_FWD(t)), kumi::flatten_all(*this),
                                 fixed<1>, kumi::function::plus, __::stride_product);
    }

    using storage_type = __::as_sequence_t<D...>;

    constexpr stride() = default;

    template<concepts::deep_extent... S>
    requires(sizeof...(S) == sizeof...(D)) //&& storage_type::template follow_mapping<kumi::tuple<S...>>())
    constexpr stride(S... s) : storage_type{s...}
    {
    }

    template<auto... StrideDims>
    friend constexpr bool operator==(stride const& a, stride<StrideDims...> const& b) noexcept
    {
      if constexpr (stride::ndim != stride<StrideDims...>::ndim) return false;
      else return kumi::to_tuple(a) == kumi::to_tuple(b);
    }

    constexpr storage_type const& self() const { return static_cast<storage_type const&>(*this); }

    constexpr storage_type& self() { return static_cast<storage_type&>(*this); }

    template<std::size_t I> KWK_ABI friend constexpr decltype(auto) get(stride const& s)
    {
      if constexpr (I >= stride::ndim) return kwk::fixed<0>;
      else return get<I>(s.self());
    }

    template<std::size_t I> KWK_ABI friend constexpr decltype(auto) get(stride& s)
    {
      if constexpr (I >= stride::ndim) return kwk::fixed<0>;
      else return get<I>(s.self());
    }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                         stride const& s) noexcept
    {
      os << "(";
      kumi::for_each([&](auto e) { os << " " << e; }, s);
      return os << " )";
    }
  };

  template<concepts::deep_extent... S> stride(S...) -> stride<__::make_dimension<std::unwrap_ref_decay_t<S>>()...>;

  /// @brief Transforms an abritrary product type into a stride
  template<kumi::concepts::product_type T> constexpr auto as_stride(T&& t)
  {
    return kumi::apply(
      [](auto&&... elt) {
        auto v_or_t = []<typename V>(V&& v) {
          if constexpr (kumi::concepts::product_type<V>) return as_stride(KWK_FWD(v));
          else return KWK_FWD(v);
        };
        return stride{v_or_t(KWK_FWD(elt))...};
      },
      KWK_FWD(t));
  }

  //==================================================================================================================
  // Compute a stride from a product type representing the shape and a custom layout permutation at compile time
  //==================================================================================================================
  template<kumi::concepts::product_type T, storage_order_descriptor order>
  constexpr auto to_stride(T&& t, storage_order_option<order> const&) noexcept
  {
    if constexpr (kumi::concepts::empty_product_type<T>) return kwk::stride{};
    else if constexpr (order == kwk::row_major_order)
    {
      auto&& strd = as_stride(kumi::exclusive_scan_right(kumi::function::multiplies, kumi::flatten_all(t), fixed<1>));
      return kwk::__::layout_cast<T>(KWK_FWD(strd));
    }
    else if constexpr (order == kwk::column_major_order)
    {
      auto&& strd = as_stride(kumi::exclusive_scan_left(kumi::function::multiplies, kumi::flatten_all(t), fixed<1>));
      return kwk::__::layout_cast<T>(KWK_FWD(strd));
    }
    else
    {
      constexpr auto N = kumi::size_v<T>;
      constexpr auto perm =
        kumi::generate<N>([&](auto i) { return kumi::index<order.generator(static_cast<int>(i), N)>; });

      auto permuted =
        kumi::apply([&]<std::size_t... I>(kumi::index_t<I>...) { return kumi::reorder<I...>(KUMI_FWD(t)); }, perm);

      auto tmp = kumi::exclusive_scan_right(kumi::function::multiplies, permuted, fixed<1>);

      return as_stride(
        kumi::generate<N>([&](auto i) { return kumi::get<order.generator(static_cast<int>(i), N)>(tmp); }));
    }
  }

  template<kwk::concepts::stride Stride, concepts::slicer... S>
  constexpr auto restride(Stride && strd, kumi::tuple<S...> const& sl) noexcept
  requires(kumi::size_v<Stride> == sizeof...(S))
  {
    return kumi::apply(
      [](auto&&... elts) { return kwk::stride{slice_stride(get<0>(KWK_FWD(elts)), get<1>(KWK_FWD(elts)))...}; },
      kumi::zip(kumi::to_tuple(strd), sl));
  }

  template<kwk::concepts::stride Stride, concepts::slicer... S>
  constexpr auto restride(Stride && strd, S const&... sl) noexcept
  requires(kumi::size_v<Stride> == sizeof...(S))
  {
    return restride(strd, kumi::make_tuple(to_slicer(sl)...));
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
  using type = std::remove_cvref_t<decltype(get<I>(std::declval<kwk::stride<StrideDims...>>()))>;
};

template<auto... D> struct kumi::builder<kwk::stride<D...>>
{
  using type = kwk::stride<D...>;

  template<typename... Us> using to = kwk::stride<kwk::__::make_dimension<std::unwrap_ref_decay_t<Us>>()...>;

  template<typename... Args> [[nodiscard]] KUMI_ABI static constexpr auto make(Args&&... args)
  {
    return kwk::stride{KUMI_FWD(args)...};
  }

  template<typename... Args> [[nodiscard]] KUMI_ABI static constexpr auto build(Args&&... args)
  {
    return kwk::stride{KUMI_FWD(args)...};
  }
};

#endif
