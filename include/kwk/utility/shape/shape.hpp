//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kwk/utility/shape/shape_descriptor.hpp>
#include <kwk/utility/fixed.hpp>

namespace kwk
{
  namespace __
  {
    struct shape_id;

    template<int N> struct normalize_dim : std::conditional<N == 0, kwk::config::default_size_type, constant<N>>
    {
    };

    template<kwk::shape_descriptor S> struct as_sequence
    {
      static constexpr auto build()
      {
        return []<std::size_t... I>(std::index_sequence<I...>) {
          return kwk::__::mixed_sequence(typename normalize_dim<S.dims[I]>::type{}...);
        }(std::make_index_sequence<S.ndim>{});
      }

      using type = decltype(build());
    };
  }

  //====================================================================================================================
  /**
    @ingroup shape-utility
    @brief Shape of multi-dimensional container

    Template class representing the shape (dimensions) of multi-dimensional containers in KIWAKU.
    @ref shape can have both compile-time known dimensions and runtime dimensions, as specified
    by the shape descriptor template parameter.

    The @ref shape class provides a unified interface to access dimension sizes, regardless of whether they are static
    or dynamic. Static dimensions are stored as compile-time constants, while dynamic dimensions are stored in an
    internal array.

    To do so, @ref shape is parameterized by a @ref shape_descriptor that defines the structure of the @ref shape,
    including the number of dimensions and which dimensions are static vs dynamic. The @ref shape_descriptor is a
    compile-time construct that encodes this information in its type. It can either be a single integral constant
    representing the number of dynamic dimensions, or a list of dimensions where static dimensions are represented by
    their compile-time values and dynamic dimensions are represented by the KIWAKU wildcard: `kwk::_`.

    For examples, consider the following shape descriptors:
    @code
    // A shape with 4 dynamic dimensions
    kwk::shape<4> shp_4_dyn;

    // A shape where the first two dimensions are static (3 and 4) and the last dimension is dynamic (`kwk::_`)
    kwk::shape<{3, 4, kwk::_}> shp_3_4_dyn;

    // A shape where the first dimension is static (5) and the next two dimensions are dynamic
    kwk::shape<{5, kwk::_, kwk::_}> shp_5_dyn_dyn;

    // A shape where all dimensions are static (2, 3, and 4)
    kwk::shape<{2, 3, 4}> shp_2_3_4;
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

    @tparam Descriptor  A shape_descriptor that defines the structure of the shape, including
                        the number of dimensions and which dimensions are static vs dynamic.

    @see shape_descriptor
  **/
  //====================================================================================================================
  template<shape_descriptor Descriptor> struct shape : private __::as_sequence<Descriptor>::type
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
    using storage_type = __::as_sequence<Descriptor>::type;

    constexpr auto operator()(identifier_type const&) const { return *this; }

    static constexpr label_type label() { return kumi::str{"Shape"}; }

    /// @brief The shape descriptor associated with this shape
    static constexpr auto descriptor = Descriptor;

    /// @brief Number of dimensions in this shape
    static constexpr auto ndim = Descriptor.ndim;

    /// @brief Total number of elements the current shape represents
    constexpr size_type size() const noexcept
    {
      return kumi::fold_left([](auto a, auto b) { return a * b; }, *this, fixed<1>);
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
    template<std::convertible_to<size_type>... S>
    requires(sizeof...(S) == Descriptor.ndim && storage_type::template follow_mapping<kumi::tuple<S...>>())
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
    template<std::convertible_to<size_type>... S>
    requires(sizeof...(S) < Descriptor.ndim)
    constexpr shape(S... s)
      : storage_type{[&]<std::size_t... I>(std::index_sequence<I...>) {
          return storage_type{s..., kumi::index<I * 0>...};
        }(std::make_index_sequence<Descriptor.ndim - sizeof...(S)>{})}
    {
    }

    //==================================================================================================================
    /// Assignment operator
    //==================================================================================================================
    // constexpr shape& operator=(shape const& other) & noexcept
    //{
    //  this->self() = other.self();
    //  return *this;
    //}

    //==================================================================================================================
    /// Equality comparison operator
    //==================================================================================================================
    template<shape_descriptor D> friend constexpr bool operator==(shape const& a, shape<D> const& b) noexcept
    {
      if constexpr (Descriptor.ndim != D.ndim) return false;
      else return kumi::to_tuple(a) == kumi::to_tuple(b);
    }

    storage_type const& self() const { return static_cast<storage_type const&>(*this); }

    storage_type& self() { return static_cast<storage_type&>(*this); }

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
  };

  //@brief Deduction guide
  template<typename... S> shape(S...) -> shape<__::make_descriptor<S...>()>;

  //@brief Squeeze the current shape by removing trivial dimensions
  template<shape_descriptor D> constexpr auto squeeze(shape<D> const& s)
  {
    constexpr auto descriptor = __::squeeze_descriptor(D);
    return [&]<std::size_t... I>(std::index_sequence<I...>) {
      return shape{get<descriptor.dims[I]>(s)...};
    }(std::make_index_sequence<descriptor.ndim>{});
  }

  //@brief Extends the current shape by adding trivial dimensions
  // template<shape_descriptor D1, shape_descriptor D2>
  // constexpr auto expand(shape<D1> const& source, shape<D2> const& target)
  //{
  //  static_assert(shape<D1>::ndim <= shape<D2>::ndim, "[Kiwaku] - Cannot broadcast to smaller shape");
  //}

  //@brief Check if two shapes are isomorph, ie they describe the same data space
  template<shape_descriptor D1, shape_descriptor D2> constexpr bool equivalent(shape<D1> const& a, shape<D2> const& b)
  {
    return squeeze(a) == squeeze(b);
  }
}

#if !defined(KWK_DOXYGEN_INVOKED)
template<kwk::shape_descriptor Descriptor>
struct std::tuple_size<kwk::shape<Descriptor>> : std::integral_constant<std::size_t, Descriptor.ndim>
{
};

template<std::size_t I, kwk::shape_descriptor Descriptor> struct std::tuple_element<I, kwk::shape<Descriptor>>
{
  using type = decltype(get<I>(std::declval<kwk::shape<Descriptor>>()));
};
#endif
