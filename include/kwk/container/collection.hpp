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
  // collection: The main container-view class
  // Private inheritance of both shape and stride guarantees Empty Base Class Optimization
  //====================================================================================================================
  template<typename... Opts> struct KWK_STRUCT_ABI collection : private Opts...
  {
    using option_type = decltype(options{std::declval<Opts>()...});
    using Kind = decltype(std::declval<option_type>()[kwk::source]);
    using shape_type = decltype(std::declval<option_type>()[kwk::of_shape]);
    using stride_type = decltype(std::declval<option_type>()[kwk::of_stride]);
    using storage_type = decltype(std::declval<option_type>()[kwk::__::blob_type]);
    using allocator_type = decltype(std::declval<option_type>()[kwk::allocator]);
    using storage_order_type = decltype(std::declval<option_type>()[kwk::storage_order]);

    using source_type = Kind;
    using value_type = typename storage_type::element_type;
    // using reference       = std::add_lvalue_reference<value_type>;
    // using const_reference = std::add_lvalue_reference<std::add_const_t<value_type>>;
    // using pointer         = std::add_pointer_t<value_type>;
    // using const_pointer   = std::add_pointer_t<value_type const>;

    static constexpr auto ndim = shape_type::ndim;
    static constexpr auto flat_ndim = kumi::size_v<
      typename shape_type::storage_type::flat_tuple>; // decltype(std::declval<shape_type>().flatten())::rank;
    static constexpr auto kind = as<value_type>();
    static constexpr auto itemsize = sizeof(value_type);
    static constexpr auto order = storage_order_type::descriptor;

    //==================================================================================================================
    /*
      Constructors
    */
    //==================================================================================================================
    template<int Flags, kumi::concepts::product_type Values>
    // requires(  ) need to check that options and Opts are the same basically
    KWK_TRIVIAL constexpr collection(options<Flags, Values> const& opts) : Opts(opts[kumi::identifier_of<Opts>()])...
    {
    }

    template<kumi::concepts::field... Options>
    requires(sizeof...(Options) == (sizeof...(Opts)))
    KWK_TRIVIAL constexpr collection(Options&&... opts) : collection(options{KWK_FWD(opts)...})
    {
    }

    // template<kwk::concepts::collection<decltype(kwk::source = std::declval<source_type>()), Opts...> C>
    // KWK_TRIVIAL constexpr collection(C const& other)
    //   : collection(other.data(), other.shape(), other.stride())
    //{
    // }

    template<kumi::concepts::field... Options>
    requires(sizeof...(Options) != (sizeof...(Opts)))
    constexpr collection(Options const&...) = delete;

    //==================================================================================================================
    /*
      Properties
    */
    //==================================================================================================================
    [[nodiscard]] KWK_TRIVIAL constexpr shape_type const& shape() const noexcept
    requires(!kumi::concepts::unit_type<shape_type>)
    {
      return static_cast<shape_type const&>(*this);
    }

    [[nodiscard]] KWK_TRIVIAL constexpr stride_type const& stride() const noexcept
    requires(!kumi::concepts::unit_type<stride_type>)
    {
      return static_cast<stride_type const&>(*this);
    }

    //[[nodiscard]] KWK_TRIVIAL constexpr storage_order_type const& storage_order() const noexcept
    // requires(!kumi::concepts::unit_type<storage_order_type>)
    //{
    //  return static_cast<storage_order_type const&>(*this);
    //}

    [[nodiscard]] KWK_TRIVIAL constexpr auto size() const noexcept
    requires(!kumi::concepts::unit_type<shape_type>)
    {
      return shape().size();
    }

    template<typename Self> [[nodiscard]] KWK_TRIVIAL constexpr decltype(auto) blob(this Self& self)
    {
      if constexpr (std::is_const_v<Self>) return static_cast<storage_type const&>(self);
      else return static_cast<storage_type&>(self);
    }

    template<typename Self>
    [[nodiscard]] KWK_TRIVIAL constexpr decltype(auto) data(this Self& self)
    requires(!kumi::concepts::unit_type<storage_type>)
    {
      return &self.blob()[0];
    }

    //==================================================================================================================
    /*
      Access operators
    */
    //==================================================================================================================
    template<typename Self, kumi::concepts::product_type T>
    decltype(auto) operator[](this Self& self, T&& t) noexcept
    requires(kumi::size_v<T> == ndim && !kumi::concepts::unit_type<storage_type>)
    {
      return kumi::apply([&](auto&&... i) -> decltype(auto) { return std::forward_like<Self>(self[KWK_FWD(i)...]); },
                         KWK_FWD(t));
    }

    template<std::integral... Is>
    auto operator[](this auto& self, Is... is) noexcept
    requires((kumi::concepts::product_type<kumi::element_t<order(0, ndim), shape_type>>) &&
             (sizeof...(Is) == kumi::size_v<kumi::element_t<order(0, ndim), shape_type>>) && (ndim != flat_ndim) &&
             !kumi::concepts::unit_type<storage_type>)
    {
      constexpr auto t_id = order(0, ndim);

      auto const strd = kumi::join(kumi::remove(self.stride(), kumi::index<t_id>, kumi::index<t_id + 1>));
      auto const shp = kumi::join(kumi::remove(self.shape(), kumi::index<t_id>, kumi::index<t_id + 1>));
      auto src = (kwk::source = &self.blob()[linearize(get<t_id>(self.stride()), is...)]);

      return kwk::builder<kwk::collection>(kwk::options{src, strd, shp});
    }

    // If the product types are matching
    template<std::integral... Is>
    decltype(auto) operator[](this auto& self, Is... is) noexcept
    requires(sizeof...(Is) == flat_ndim && !kumi::concepts::unit_type<storage_type>)
    {
      using type = typename stride_type::storage_type::flat_tuple;
      return self.blob()[linearize(kwk::__::layout_cast<type>(self.stride()), is...)];
    }

    template<concepts::slicer... S>
    auto operator[](this auto& self, S const&... s) noexcept
    requires(sizeof...(S) == ndim)
    {
      auto src = (kwk::source = self.data() + kwk::origin(self.shape(), self.storage_order(), s...));
      auto shp = kwk::reshape(self.shape(), s...);
      auto strd = kwk::restride(self.stride(), s...);

      return kwk::builder<kwk::collection>(kwk::options{src, shp, strd});
    }
  };

  //
  template<kumi::concepts::field... Opts>
  requires(!kumi::concepts::uniquely_named<Opts...> || !kumi::concepts::fully_named<Opts...> ||
           !kumi::concepts::uniquely_labeled<Opts...>)
  struct collection<Opts...>
  {
    static_assert(kumi::concepts::uniquely_named<Opts...>, "[KWK] - Duplicate option in collection definition");
    static_assert(kumi::concepts::uniquely_labeled<Opts...>,
                  "[KWK] - Duplicate option representation in collection definition");
    static_assert(kumi::concepts::fully_named<Opts...>, "[KWK] - Anonymous option usage in collection definition");

    template<typename... Ts> collection(Ts&&...) = delete;
  };

  //====================================================================================================================
  // Deduction Guides
  //====================================================================================================================
  template<kumi::concepts::field Source, kumi::concepts::field... Opts>
  collection(Source&&, Opts&&...) -> collection<std::unwrap_ref_decay_t<Source>, std::unwrap_ref_decay_t<Opts>...>;

  // template<kumi::concepts::field... Opts>
  // collection(Opts&&... opts) -> collection<decltype(builder(options{std::declval<Opts>()...}))>;

  // template<int Flags, kumi::concepts::product_type Values>
  // collection(options<Flags, Values> const& opts) -> collection<>;
  // collection<typename __::view_traits<options<Flags, Values>>::source_type,
  //__::view_traits<options<Flags, Values>>::make_options()>;
}
