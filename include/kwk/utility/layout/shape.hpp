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
    struct normalize_dim
      : std::conditional<kumi::concepts::product_type<decltype(N)>,
                         decltype(N),
                         std::conditional_t<N == kwk::_, kwk::config::default_size_type, constant<N>>>
    {
    };

    template<concepts::deep_extent auto... x> struct as_sequence
    {
      using type = decltype(kwk::__::compressed_tuple(std::declval<typename normalize_dim<x>::type>()...));
    };

    template<typename T> consteval auto make_dimension()
    {
      if constexpr (kumi::concepts::product_type<T>) return T{};
      else if constexpr (concepts::dynamic_extent<T>) return _;
      else return T::value;
    }

    //@brief Simply stores the indexes of non trivial dimension to be used as an index-map
    consteval auto squeeze_dimensions(concepts::extent auto... Dims)
    {
      struct
      {
        std::size_t data[sizeof...(Dims)], count = {};
      } d{};

      [&]<std::size_t... I>(std::index_sequence<I...>) {
        ((void)((Dims != 1) ? (d.data[d.count++] = I) : 0), ...);
      }(std::make_index_sequence<sizeof...(Dims)>{});
      return d;
    }
  }

  //====================================================================================================================
  /**
    @ingroup shape-utility
    @brief Shape of multi-dimensional container

    Template class representing the shape (dimensions) of multi-dimensional containers in KIWAKU.
    @ref shape can have both compile-time known dimensions and runtime dimensions, as specified
    by it's NTTPs.

    The @ref shape class provides a unified interface to access dimension sizes, regardless of whether they are static
    or dynamic. Static dimensions are stored as compile-time constants, while dynamic dimensions are stored in an
    internal array.

    To do so, @ref shape is parameterized by a variadic number of NTTPs that defines the structure of the @ref shape,
    including the number of dimensions and which dimensions are static vs dynamic. It is a list of dimensions where
    static dimensions are represented by their compile-time values and dynamic dimensions are represented by the
    KIWAKU @ref wildcard: `kwk::_`.

    For examples, consider the following shape descriptors:
    @code
    // A shape where the first two dimensions are static (3 and 4) and the last dimension is dynamic (`kwk::_`)
    kwk::shape<3, 4, kwk::_> shp_3_4_dyn;

    // A shape where the first dimension is static (5) and the next two dimensions are dynamic
    kwk::shape<5, kwk::_, kwk::_> shp_5_dyn_dyn;

    // A shape where all dimensions are static (2, 3, and 4)
    kwk::shape<2, 3, 4> shp_2_3_4;
    @endcode

    Beware that `shape<4>`and `shape<{4}>` are not the same : the first one is a shape with 4 dynamic dimensions
    while the second one is a shape with 1 static dimension of size 4.

    shape also provides a template deduction guide that allows constructing a shape directly from a list of dimension
    sizes, where static dimensions and dynamic dimensions are captured to ensure accurate construction.

    For example:
    @code
    using namespace kwk::literals;

    // Constructing a shape with 4 dynamic dimensions from runtime sizes
    auto shp_4_dyn = kwk::shape(2, 3, 4 5);

    // Constructing a shape with 2 static dimensions (3 and 4) and 1 dynamic dimension from runtime sizes
    auto shp_3_4_dyn = kwk::shape(3_c, 4_c, 5);

    // Constructing a shape with 1 static dimension (5) and 2 dynamic dimensions from runtime sizes
    auto shp_5_dyn_dyn = kwk::shape(5_c, 2, 3);

    // Constructing a shape with 3 static dimensions (2, 3, and 4) from compile-time sizes
    auto shp_2_3_4 = kwk::shape(2_c, 3_c, 4_c);
    @endcode

    @tparam D   Constant template parameters defining the structure of the shape, including
                the number of dimensions and which dimensions are static vs dynamic.
  **/
  //====================================================================================================================
  template<concepts::deep_extent auto... Dims> struct shape : __::as_sequence<Dims...>::type
  {
    //==================================================================================================================
    // Shape is a field over itself
    //==================================================================================================================
    using element_type = shape;
    using type = shape;
    using identifier_type = __::shape_id;
    using label_type = kumi::str;
    using size_type = kwk::config::default_size_type;

    /// @brief Internal type for efficient storage of hybrid static/dynamic dimensions
    using storage_type = __::as_sequence<Dims...>::type;

    constexpr auto operator()(identifier_type const&) const { return *this; }

    static constexpr label_type label() { return kumi::str{"Shape"}; }

    /// @brief Number of dimensions in this shape
    static constexpr auto ndim = sizeof...(Dims);

    /// @brief Dynamic property of this shape
    static constexpr bool fully_dynamic = ((Dims == kwk::_) && ...);

    /// @brief Detection mechanism for fully static shapes
    static constexpr bool has_dynamic_dim = ((Dims == kwk::_) || ...);

    /// @brief Total number of elements the current shape represents
    constexpr size_type size() const noexcept
    {
      return kumi::fold_left(kumi::function::multiplies, (*this).flatten(), fixed<1>);
    }

    /// @brief Default constructor
    constexpr shape() : storage_type() {}

    /**
      @brief Constructor from dimension sizes

      Constructs a shape with a set of dimension sizes.
      Does not participate in overload resolution if the provided sizes do not match the expected structure of the shape
      (e.g., static dimensions must match their compile-time values) or if they are not convertible to the specified
      SizeType.

      @param s Dimension sizes, which quantity must match ndim.
    **/
    template<concepts::deep_extent... S>
    requires(sizeof...(S) == ndim) // && storage_type::template follow_mapping<kumi::tuple<S...>>())
    constexpr shape(S... s) : storage_type{s...}
    {
    }

    /**
      @brief Constructor from partial dimension sizes

      Constructs a shape with a set of dimension sizes, where the number of provided sizes is less than ndim.
      The missing dimensions are filled with zeros. Does not participate in overload resolution if the provided sizes
      do not match the expected descriptor of the shape (e.g., static dimensions must match their compile-time values)
      or if they are not convertible to the specified SizeType.

      @param s Dimension sizes, which quantity must be less than ndim.
    **/
    template<concepts::deep_extent... S>
    requires(sizeof...(S) < ndim)
    constexpr shape(S... s)
      : storage_type{[&]<std::size_t... I>(std::index_sequence<I...>) {
          return storage_type{s..., kumi::index<I * 0>...};
        }(std::make_index_sequence<ndim - sizeof...(S)>{})}
    {
    }

    //==================================================================================================================
    /// Assignment operator
    //==================================================================================================================
    template<concepts::extent auto... D> constexpr shape& operator=(shape<D...> const& other) & noexcept
    {
      this->self() = other.self();
      return *this;
    }

    //==================================================================================================================
    /// Equality comparison operator
    //==================================================================================================================
    template<auto... D> friend constexpr bool operator==(shape const& a, shape<D...> const& b) noexcept
    {
      if constexpr (shape::ndim != shape<D...>::ndim) return false;
      else return kumi::to_tuple(a) == kumi::to_tuple(b);
    }

    constexpr storage_type const& self() const { return static_cast<storage_type const&>(*this); }

    constexpr storage_type& self() { return static_cast<storage_type&>(*this); }

    template<std::size_t I> KWK_TRIVIAL friend constexpr decltype(auto) get(shape const& s)
    {
      if constexpr (I >= shape::ndim) return fixed<1>;
      else return get<I>(s.self());
    }

    template<std::size_t I> KWK_TRIVIAL friend constexpr decltype(auto) get(shape& s)
    {
      if constexpr (I >= shape::ndim) return fixed<1>;
      else return get<I>(s.self());
    }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, shape const& s) noexcept
    {
      os << "(";
      kumi::for_each([&](auto e) { os << " " << e; }, s);
      return os << " )";
    }

    KWK_TRIVIAL constexpr decltype(auto) flatten(this auto&& self) noexcept
    {
      constexpr auto as_flat = [](auto&&... elts) { return kwk::shape{KWK_FWD(elts)...}; };
      using flat_t = kumi::result::apply_t<decltype(as_flat), typename storage_type::flat_tuple>;
      return std::bit_cast<flat_t>(self);
    }
  };

  //@brief Deduction guide
  template<concepts::deep_extent... S> shape(S&&...) -> shape<__::make_dimension<std::unwrap_ref_decay_t<S>>()...>;

  /// @brief Transforms an abritrary product type into a shape
  template<kumi::concepts::product_type T> constexpr auto as_shape(T&& t)
  {
    return kumi::apply(
      [](auto&&... elt) {
        auto v_or_t = []<typename V>(V&& v) {
          if constexpr (kumi::concepts::product_type<V>) return as_shape(KWK_FWD(v));
          else return KWK_FWD(v);
        };
        return shape{v_or_t(KWK_FWD(elt))...};
      },
      KWK_FWD(t));
  }

  //@brief Squeeze the current shape by removing trivial dimensions
  template<auto... D> constexpr auto squeeze(shape<D...> const& s)
  {
    constexpr auto pos = squeeze_dimensions(D...);
    return [&]<std::size_t... I>(std::index_sequence<I...>) {
      return shape{get<pos.data[I]>(s)...};
    }(std::make_index_sequence<pos.count>{});
  }

  template<auto... D, concepts::extent Dim> constexpr auto add_rank(shape<D...> const& s, Dim N)
  {
    return kumi::apply([&](auto&&... elts) { return shape{KWK_FWD(elts)..., N}; }, s);
  }

  //@brief Extends the current shape by adding trivial dimensions
  // template<shape_descriptor D1, shape_descriptor D2>
  // constexpr auto expand(shape<D1> const& source, shape<D2> const& target)
  //{
  //  static_assert(shape<D1>::ndim <= shape<D2>::ndim, "[Kiwaku] - Cannot broadcast to smaller shape");
  //}

  //@brief Check if two shapes are isomorph, ie they describe the same data space
  template<auto... D1, auto... D2> constexpr bool equivalent(shape<D1...> const& a, shape<D2...> const& b)
  {
    return squeeze(a) == squeeze(b);
  }

  inline namespace literals
  {
    //====================================================================================================================
    /**
      @ingroup shape-utility
      @brief Describe a 1D dynamic shape
      @relates shape
    **/
    //====================================================================================================================
    inline constexpr shape<kwk::_> _1D{};

    //====================================================================================================================
    /**
      @ingroup shape-utility
      @brief Describe a 2D dynamic shape
      @relates shape
    **/
    //====================================================================================================================
    inline constexpr shape<kwk::_, kwk::_> _2D{};

    //====================================================================================================================
    /**
      @ingroup shape-utility
      @brief Describe a 3D dynamic shape
      @relates shape
    **/
    //====================================================================================================================
    inline constexpr shape<kwk::_, kwk::_, kwk::_> _3D{};

    //====================================================================================================================
    /**
      @ingroup shape-utility
      @brief Describe a 4D dynamic shape
      @relates shape
    **/
    //====================================================================================================================
    inline constexpr shape<kwk::_, kwk::_, kwk::_, kwk::_> _4D{};

    //====================================================================================================================
    /**
      @ingroup shape-utility
      @brief Describe a dynamic shape with N dimensions
      @relates shape
    **/
    //====================================================================================================================
    template<std::convertible_to<std::size_t> auto N>
    inline constexpr auto _nD = []<std::size_t... I>(std::index_sequence<I...>) {
      auto eval = [](auto, auto const& v) { return v; };
      return shape<eval(I, kwk::_)...>{};
    }(std::make_index_sequence<N>{});
  }
}

#if !defined(KWK_DOXYGEN_INVOKED)
template<auto... D> struct std::tuple_size<kwk::shape<D...>> : std::integral_constant<std::size_t, sizeof...(D)>
{
};

template<std::size_t I, auto... D> struct std::tuple_element<I, kwk::shape<D...>>
{
  using type = std::remove_cvref_t<decltype(get<I>(std::declval<kwk::shape<D...>>()))>;
};

template<auto... Ts> struct kumi::builder<kwk::shape<Ts...>>
{
  using type = kwk::shape<Ts...>;

  template<typename... Us> using to = kwk::shape<kwk::__::make_dimension<std::unwrap_ref_decay_t<Us>>()...>;

  template<typename... Args> [[nodiscard]] KUMI_ABI static constexpr auto make(Args&&... args)
  {
    return kwk::shape{KUMI_FWD(args)...};
  }

  template<typename... Args> [[nodiscard]] KUMI_ABI static constexpr auto build(Args&&... args)
  {
    return kwk::shape{KUMI_FWD(args)...};
  }
};
#endif
