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

    using boundary_t = decltype(std::declval<option_type>()[kwk::boundary]);
    using coordinate_t = decltype(std::declval<option_type>()[kwk::coordinate]);
    using interpolator_t = decltype(std::declval<option_type>()[kwk::interpolator]);

    // using accessor_type = decltype(get_accessor(std::declval<option_type>()));

    using source_type = Kind;
    using value_type = typename storage_type::element_type;

    static constexpr auto ndim = shape_type::ndim;
    static constexpr auto flat_ndim = kumi::size_v<typename shape_type::storage_type::flat_tuple>;
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
    {
      static_assert(!kumi::concepts::unit_type<shape_type>, "[KWK] - Invalid shape");
      return static_cast<shape_type const&>(*this);
    }

    [[nodiscard]] KWK_TRIVIAL constexpr stride_type const& stride() const noexcept
    {
      static_assert(!kumi::concepts::unit_type<stride_type>, "KWK - Invalid stride");
      return static_cast<stride_type const&>(*this);
    }

    [[nodiscard]] KWK_TRIVIAL constexpr storage_order_type const& storage_order() const noexcept
    {
      static_assert(!kumi::concepts::unit_type<storage_order_type>, "[KWK] - Invalid storage order");
      return static_cast<storage_order_type const&>(*this);
    }

    [[nodiscard]] KWK_TRIVIAL constexpr boundary_t const& boundary_conditions() const noexcept
    {
      static_assert(!kumi::concepts::unit_type<boundary_t>, "[KWK] - Boundary conditions not set");
      return static_cast<boundary_t const&>(*this);
    }

    [[nodiscard]] KWK_TRIVIAL constexpr coordinate_t const& coordinate_function() const noexcept
    {
      static_assert(!kumi::concepts::unit_type<coordinate_t>, "[KWK] - Coordinate function not set");
      return static_cast<coordinate_t const&>(*this);
    }

    [[nodiscard]] KWK_TRIVIAL constexpr interpolator_t const& interpolator() const noexcept
    {
      static_assert(!kumi::concepts::unit_type<boundary_t>, "[KWK] - Interpolator not set");
      return static_cast<interpolator_t const&>(*this);
    }

    [[nodiscard]] KWK_TRIVIAL constexpr auto size() const noexcept
    {
      static_assert(!kumi::concepts::unit_type<shape_type>, "[KWK] - Invalid size");
      return shape().size();
    }

    template<typename Self> [[nodiscard]] KWK_TRIVIAL constexpr decltype(auto) blob(this Self& self)
    {
      static_assert(!kumi::concepts::unit_type<storage_type>, "[KWK] - Invalid storage");
      if constexpr (std::is_const_v<Self>) return static_cast<storage_type const&>(self);
      else return static_cast<storage_type&>(self);
    }

    template<typename Self> [[nodiscard]] KWK_TRIVIAL constexpr decltype(auto) data(this Self& self)
    {
      static_assert(!kumi::concepts::unit_type<storage_type>, "[KWK] - Invalid storage");
      return &self.blob()[0];
    }

    //==================================================================================================================
    /*
      Access helpers
    */
    //==================================================================================================================
    template<typename Self, kumi::concepts::product_type Position, kumi::concepts::product_type Stride>
    KWK_TRIVIAL constexpr decltype(auto) access(this Self&& self, Position&& p, Stride&& s)
    {
      /// Lambda storage value retrieval shall be done here
      return self.blob()[self.coordinate_function()(KWK_FWD(p), KWK_FWD(s))];
    }

    template<typename Self,
             kumi::concepts::product_type Position,
             kumi::concepts::product_type Shape,
             kumi::concepts::product_type Stride>
    KWK_TRIVIAL constexpr decltype(auto) bounds_check(this Self&& self, Position&& p, Shape&& sp, Stride&& sd)
    {
      if constexpr (kumi::concepts::unit_type<boundary_t>) return self.access(KWK_FWD(p), KWK_FWD(sd));
      else return self.access(self.boundary_conditions()(KWK_FWD(p), KWK_FWD(sp)), KWK_FWD(sd));
    }

    template<typename Self,
             kumi::concepts::product_type Position,
             kumi::concepts::product_type Shape,
             kumi::concepts::product_type Stride>
    KWK_TRIVIAL constexpr decltype(auto) resolve(this Self&& self, Position&& p, Shape&& sh, Stride&& sd)
    {
      /// Interpolation should be handled in this step
      return self.bounds_check(KWK_FWD(p), KWK_FWD(sh), KWK_FWD(sd));
    }

    template<typename Self,
             kumi::concepts::product_type Position,
             kumi::concepts::product_type Shape,
             kumi::concepts::product_type Stride>
    KWK_TRIVIAL constexpr decltype(auto) source_at(this Self&& self, Position&& p, Shape&& sh, Stride&& sd)
    {
      /// If the blob actually is a lambda : return it
      return self.resolve(KWK_FWD(p), KWK_FWD(sh), KWK_FWD(sd));
    }

    //==================================================================================================================
    /*
      Access operators
    */
    //==================================================================================================================
    template<typename Self, kumi::concepts::product_type T>
    KWK_TRIVIAL decltype(auto) operator[](this Self& self, T&& t) noexcept
    requires(kumi::size_v<T> == ndim && !kumi::concepts::unit_type<storage_type>)
    {
      return kumi::apply([&](auto&&... i) -> decltype(auto) { return std::forward_like<Self>(self[KWK_FWD(i)...]); },
                         KWK_FWD(t));
    }

    template<std::integral... Is>
    decltype(auto) operator[](this auto& self, Is... is) noexcept
    requires(!kumi::concepts::unit_type<storage_type>)
    {
      static_assert(sizeof...(Is) == ndim, "[KWK] - Invalid number of coordinates in table access");
      if constexpr (ndim == sizeof...(Is) && ndim == flat_ndim)
        return self.resolve(kumi::tuple{is...}, self.shape(), self.stride());
      else if constexpr (ndim != flat_ndim)
      {
        constexpr auto id = order(0, ndim);
        using element_type = kumi::element_t<id, shape_type>;
        static_assert(kumi::concepts::product_type<element_type>, "[KWK] - Invalid tile access");
        static_assert(sizeof...(Is) == kumi::size_v<element_type>,
                      "[KWK] - Invalid number of coordinates for tile access");

        auto const strd = kumi::compress(kumi::remove(self.stride(), kumi::index<id>, kumi::index<id + 1>));
        auto const shp = kumi::compress(kumi::remove(self.shape(), kumi::index<id>, kumi::index<id + 1>));
        auto pos = self.source_at(kumi::tuple{is...}, get<id>(self.shape()), get<id>(self.stride()));
        return kwk::builder<kwk::collection>(kwk::options{kwk::source = pos, strd, shp});
      }
    }

    template<concepts::slicer... S>
    constexpr auto operator[](this auto& self, S const&... s) noexcept
    requires(sizeof...(S) == ndim)
    {
      auto slicer = kumi::tuple{to_slicer(s)...};

      auto const shp = kwk::reshape(self.shape(), slicer);
      auto const strd = kwk::restride(self.stride(), slicer);

      return kwk::builder<kwk::collection>(kwk::options{
        kwk::source = &self.source_at(kwk::origin(self.shape(), slicer), self.shape(), self.stride()), shp, strd});
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
