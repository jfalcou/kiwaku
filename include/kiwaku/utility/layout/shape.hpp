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
    template<auto N>
    using normalize_dim = std::conditional_t<
        kumi::concepts::product_type<decltype(N)>,
        decltype(N),
        std::conditional_t<N == kwk::_, kwk::config::default_size_type, constant<N>>>;

    template<kwk::concepts::deep_extent auto... Extents>
    using as_sequence_t =
        decltype(kwk::__::compressed_tuple(std::declval<normalize_dim<Extents>>()...));

    template<typename T> consteval auto make_dimension()
    {
      if constexpr(kumi::concepts::product_type<T>) return T {};
      else if constexpr(concepts::dynamic_extent<T>) return _;
      else return T::value;
    }

    //@brief Simply stores the indexes of non trivial dimension to be used as an index-map
    consteval auto squeeze_dimensions(concepts::extent auto... Dims)
    {
      struct
      {
        std::size_t data[ sizeof...(Dims) ], count = {};
      } d {};

      [ & ]<std::size_t... I>(std::index_sequence<I...>)
      {
        ((void)((Dims != 1) ? (d.data[ d.count++ ] = I) : 0), ...);
      }(std::make_index_sequence<sizeof...(Dims)> {});
      return d;
    }
  }

  //====================================================================================================================
  /**
    @ingroup layout-utility
    @brief Shape of multi-dimensional container

    Template class representing the shape (dimensions) of multi-dimensional containers in KIWAKU.
    @ref shape can have both compile-time known dimensions and runtime dimensions, as specified
    by it's NTTPs.

    The @ref shape class provides a unified interface to access dimension sizes, regardless of
  whether they are static or dynamic. Static dimensions are stored as compile-time constants, while
  dynamic dimensions are stored in an internal array.

    To do so, @ref shape is parameterized by a variadic number of NTTPs that defines the structure
  of the @ref shape, including the number of dimensions and which dimensions are static vs dynamic.
  It is a list of dimensions where static dimensions are represented by their compile-time values
  and dynamic dimensions are represented by the KIWAKU @ref wildcard: `kwk::_`.

    For examples, consider the following shape descriptors:
    @code
    // A shape where the first two dimensions are static (3 and 4) and the last dimension is dynamic
  (`kwk::_`) kwk::shape<3, 4, kwk::_> shp_3_4_dyn;

    // A shape where the first dimension is static (5) and the next two dimensions are dynamic
    kwk::shape<5, kwk::_, kwk::_> shp_5_dyn_dyn;

    // A shape where all dimensions are static (2, 3, and 4)
    kwk::shape<2, 3, 4> shp_2_3_4;
    @endcode

    Beware that `shape<4>`and `shape<{4}>` are not the same : the first one is a shape with 4
  dynamic dimensions while the second one is a shape with 1 static dimension of size 4.

    shape also provides a template deduction guide that allows constructing a shape directly from a
  list of dimension sizes, where static dimensions and dynamic dimensions are captured to ensure
  accurate construction.

    For example:
    @code
    using namespace kwk::literals;

    // Constructing a shape with 4 dynamic dimensions from runtime sizes
    auto shp_4_dyn = kwk::shape(2, 3, 4 5);

    // Constructing a shape with 2 static dimensions (3 and 4) and 1 dynamic dimension from runtime
  sizes auto shp_3_4_dyn = kwk::shape(3_c, 4_c, 5);

    // Constructing a shape with 1 static dimension (5) and 2 dynamic dimensions from runtime sizes
    auto shp_5_dyn_dyn = kwk::shape(5_c, 2, 3);

    // Constructing a shape with 3 static dimensions (2, 3, and 4) from compile-time sizes
    auto shp_2_3_4 = kwk::shape(2_c, 3_c, 4_c);
    @endcode

    @tparam Dims   Constant template parameters defining the structure of the shape, including
                the number of dimensions and which dimensions are static vs dynamic.
  **/
  //====================================================================================================================
  template<concepts::deep_extent auto... Dims> struct shape_t : __::as_sequence_t<Dims...>
  {
    //==================================================================================================================
    // Shape is a field over itself
    //==================================================================================================================
    using element_type    = shape_t;
    using type            = shape_t;
    using identifier_type = __::shape_id;
    using label_type      = kumi::str;
    using size_type       = kwk::config::default_size_type;
    using shape_type      = shape_t;

    /// @brief Internal type for efficient storage of hybrid static/dynamic dimensions
    using storage_type    = __::as_sequence_t<Dims...>;

    template<typename Self> constexpr auto&& operator()(this Self&& self, identifier_type const&)
    {
      using self_t = std::
          conditional_t<std::is_const_v<std::remove_reference_t<Self>>, shape_t const, shape_t>;
      return std::forward_like<Self>(static_cast<self_t&>(self));
    }

    //==================================================================================================================
    // A shape is a property over itself : it exposes its own value through a `shape()` accessor.
    //==================================================================================================================
    template<typename Self>
    [[nodiscard]] KWK_ABI constexpr decltype(auto) shape(this Self&& self) noexcept
    {
      using fwd_t = decltype(std::forward_like<Self>(std::declval<shape_t>()));
      return static_cast<fwd_t>(self);
    }

    static constexpr label_type label() { return kumi::str {"Shape"}; }

    /// @brief Number of dimensions in this shape
    static constexpr size_type  ndim      = sizeof...(Dims);

    /// @brief Number of dimensions in the shape once flattened
    static constexpr size_type  flat_ndim = kumi::size_v<kumi::result::flatten_all_t<storage_type>>;

    /// @brief Dynamic property of this shape
    // static constexpr bool fully_dynamic = ((Dims == kwk::_) && ...);

    /// @brief Detection mechanism for fully static shapes
    // static constexpr bool has_dynamic_dim = ((Dims == kwk::_) || ...);

    /// @brief Total number of elements the current shape represents
    constexpr size_type         size() const noexcept
    {
      // Read the dimension values directly (no bit-cast) : a fully-static shape may fold to a
      // single empty EBO byte, so it cannot be layout_cast (bit_cast) to a same-rank flat tuple.
      return kumi::prod(kumi::flatten_all(*this), fixed<1>);
    }

    /// @brief Default constructor
    constexpr shape_t()
        : storage_type()
    {
    }

    /**
      @brief Constructor from dimension sizes

      Constructs a shape with a set of dimension sizes.
      Does not participate in overload resolution if the provided sizes do not match the expected
    structure of the shape (e.g., static dimensions must match their compile-time values) or if they
    are not convertible to the specified SizeType.

      @param s Dimension sizes, which quantity must match ndim.
    **/
    template<concepts::deep_extent... S>
      requires(sizeof...(S) ==
               ndim) // && storage_type::template follow_mapping<kumi::tuple<S...>>())
    constexpr shape_t(S... s)
        : storage_type {s...}
    {
    }

    /**
      @brief Constructor from partial dimension sizes

      Constructs a shape with a set of dimension sizes, where the number of provided sizes is less
    than ndim. The missing dimensions are filled with zeros. Does not participate in overload
    resolution if the provided sizes do not match the expected descriptor of the shape (e.g., static
    dimensions must match their compile-time values) or if they are not convertible to the specified
    SizeType.

      @param s Dimension sizes, which quantity must be less than ndim.
    **/
    template<concepts::deep_extent... S>
      requires(sizeof...(S) < ndim)
    constexpr shape_t(S... s)
        : storage_type {[ & ]<std::size_t... I>(std::index_sequence<I...>)
                        {
                          return storage_type {s..., kumi::index<I * 0>...};
                        }(std::make_index_sequence<ndim - sizeof...(S)> {})}
    {
    }

    //==================================================================================================================
    /// Assignment operator
    //==================================================================================================================
    template<concepts::extent auto... D>
    constexpr shape_t& operator=(shape_t<D...> const& other) & noexcept
    {
      this->self() = other.self();
      return *this;
    }

    //==================================================================================================================
    /// Equality comparison operator
    //==================================================================================================================
    template<auto... D>
    friend constexpr bool operator==(shape_t const& a, shape_t<D...> const& b) noexcept
    {
      if constexpr(shape_t::ndim != shape_t<D...>::ndim) return false;
      else return kumi::to_tuple(a) == kumi::to_tuple(b);
    }

    constexpr storage_type const& self() const { return static_cast<storage_type const&>(*this); }

    constexpr storage_type&       self() { return static_cast<storage_type&>(*this); }

    template<std::size_t I> KWK_ABI friend constexpr decltype(auto) get(shape_t const& s)
    {
      if constexpr(I >= shape_t::ndim) return fixed<1>;
      else return get<I>(s.self());
    }

    template<std::size_t I> KWK_ABI friend constexpr decltype(auto) get(shape_t& s)
    {
      if constexpr(I >= shape_t::ndim) return fixed<1>;
      else return get<I>(s.self());
    }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                         shape_t const& s) noexcept
    {
      os << "(";
      kumi::for_each([ & ](auto e) { os << " " << e; }, s);
      return os << " )";
    }
  };

  //@brief Deduction guide
  template<concepts::deep_extent... S>
  shape_t(S&&...) -> shape_t<__::make_dimension<std::unwrap_ref_decay_t<S>>()...>;

  /// @brief Alias for the shape type (natural name of the property)
  template<concepts::deep_extent auto... Dims> using shape = shape_t<Dims...>;

  /// @brief Transforms an abritrary product type into a shape
  template<kumi::concepts::product_type T> constexpr auto as_shape(T&& t)
  {
    return kumi::apply(
        [](auto&&... elt)
        {
          auto v_or_t = []<typename V>(V&& v)
          {
            if constexpr(kumi::concepts::product_type<V>) return as_shape(KWK_FWD(v));
            else return KWK_FWD(v);
          };
          return shape_t {v_or_t(KWK_FWD(elt))...};
        },
        KWK_FWD(t));
  }

  //@brief Squeeze the current shape by removing trivial dimensions
  template<auto... D> constexpr auto squeeze(shape_t<D...> const& s)
  {
    constexpr auto pos = __::squeeze_dimensions(D...);
    return [ & ]<std::size_t... I>(std::index_sequence<I...>)
    { return shape_t {get<pos.data[ I ]>(s)...}; }(std::make_index_sequence<pos.count> {});
  }

  template<auto... D, concepts::extent Dim> constexpr auto add_rank(shape_t<D...> const& s, Dim N)
  {
    return kumi::apply([ & ](auto&&... elts) { return shape_t {KWK_FWD(elts)..., N}; }, s);
  }

  //@brief Extends the current shape by adding trivial dimensions
  // template<shape_descriptor D1, shape_descriptor D2>
  // constexpr auto expand(shape<D1> const& source, shape<D2> const& target)
  //{
  //  static_assert(shape<D1>::ndim <= shape<D2>::ndim, "[Kiwaku] - Cannot broadcast to smaller
  //  shape");
  //}

  //@brief Check if two shapes are isomorph, ie they describe the same data space
  template<auto... D1, auto... D2>
  constexpr bool equivalent(shape_t<D1...> const& a, shape_t<D2...> const& b)
  {
    return squeeze(a) == squeeze(b);
  }

  template<kwk::concepts::shape Shape, concepts::slicer... S>
  constexpr auto reshape(Shape&& s, kumi::tuple<S...> const& sl) noexcept
    requires(kumi::size_v<Shape> == sizeof...(S))
  {
    return kumi::apply(
        [](auto&&... elts)
        { return kwk::shape_t {slice_extent(get<0>(KWK_FWD(elts)), get<1>(KWK_FWD(elts)))...}; },
        kumi::zip(kumi::to_tuple(s), sl));
  }

  template<kwk::concepts::shape Shape, concepts::slicer... S>
  constexpr auto reshape(Shape&& shp, S const&... sl) noexcept
    requires(kumi::size_v<Shape> == sizeof...(S))
  {
    return reshape(shp, kumi::make_tuple(to_slicer(sl)...));
  }

  namespace __
  {
    template<std::size_t... I> constexpr auto make_shape(std::index_sequence<I...>) noexcept
    {
      return kwk::shape<((void)I, kwk::_)...> {};
    }
  }

  inline namespace literals
  {
    //====================================================================================================================
    /**
      @ingroup layout-utility
      @brief Literal operator to build an n-dimensional dynamic shape
      @related kwk::shape

      @details `N_D` describes a fully-dynamic shape of rank `N`: every dimension is the `kwk::_`
      wildcard, so the shape has no fixed extent (`size() == 0`) but a compile-time `ndim == N`. It
    is used to build shapes whose extent is filled in at runtime, e.g. an n-D table of unknown,
      runtime-determined dimensions.

      @return a `kwk::shape` whose rank is the number of `c` digits and whose every dimension is
      `kwk::_` (fully dynamic).
    **/
    //====================================================================================================================
    template<char... c> inline constexpr auto operator""_D() noexcept
    {
      return kwk::__::make_shape(std::make_index_sequence<kwk::__::b10<c...>()> {});
    }
  }
}

#if !defined(KWK_DOXYGEN_INVOKED)
template<auto... D>
struct std::tuple_size<kwk::shape<D...>> : std::integral_constant<std::size_t, sizeof...(D)>
{
};

template<std::size_t I, auto... D> struct std::tuple_element<I, kwk::shape<D...>>
{
  using type = std::remove_cvref_t<decltype(get<I>(std::declval<kwk::shape<D...>>()))>;
};

template<auto... D> struct kumi::builder<kwk::shape<D...>>
{
  using type = kwk::shape<D...>;

  template<typename... Us>
  using to = kwk::shape<kwk::__::make_dimension<std::unwrap_ref_decay_t<Us>>()...>;

  template<typename... Args> [[nodiscard]] KUMI_ABI static constexpr auto make(Args&&... args)
  {
    return kwk::shape {KUMI_FWD(args)...};
  }

  template<typename... Args> [[nodiscard]] KUMI_ABI static constexpr auto build(Args&&... args)
  {
    return kwk::shape {KUMI_FWD(args)...};
  }
};
#endif
