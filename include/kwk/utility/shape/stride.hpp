//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kwk/utility/shape/shape_descriptor.hpp>
#include <kwk/utility/shape/shape.hpp>
#include <kwk/utility/fixed.hpp>

namespace kwk
{
  namespace __
  {
    struct stride_id;

    //==================================================================================================================
    // Compute stride descriptor from a shape descriptor at compile time
    //==================================================================================================================
    consteval shape_descriptor make_stride_descriptor(shape_descriptor s_desc)
    {
      shape_descriptor st_desc;
      st_desc.ndim = s_desc.ndim;

      if (st_desc.ndim > 0)
      {
        st_desc.dims[0] = 1;

        for (int i = 1; i < st_desc.ndim; ++i)
        {
          if (st_desc.dims[i - 1] == 0 || s_desc.dims[i - 1] == 0) st_desc.dims[i] = 0;
          else st_desc.dims[i] = st_desc.dims[i - 1] * s_desc.dims[i - 1];
        }
      }

      st_desc.fully_dynamic = true;
      for (int i = 0; i < st_desc.ndim; ++i)
      {
        if (st_desc.dims[i] != 0) st_desc.fully_dynamic = false;
      }

      return st_desc;
    }
  }

  //====================================================================================================================
  /**
    @ingroup shape-utility
    @brief Stride of multi-dimensional container

    The `stride` class template represents the stride of a multi-dimensional container, which is used to compute the
    memory offset for each dimension when accessing elements. It is parameterized by a `shape_descriptor` that describes
    the layout of the container and an optional `SizeType` that specifies the type used for storing stride values.

    The `stride` class provides constructors for initializing the stride from explicit values or from a shape, and
    it supports structured bindings for easy access to individual stride values. The stride values are computed based
    on the dimensions of the shape, with the first dimension having a stride of 1 and subsequent dimensions having
    strides.

    @tparam Descriptor A `shape_descriptor` that describes the layout of the container.
    @tparam SizeType An optional type used for storing stride values, defaulting to `kwk::config::default_size_type`.
  **/
  //====================================================================================================================
  template<shape_descriptor Descriptor, typename SizeType = kwk::config::default_size_type> struct stride
  {

    //==================================================================================================================
    // Shape is a field over itself
    //==================================================================================================================
    using element_type = stride;
    using type = stride;
    using identifier_type = __::stride_id;
    using label_type = kumi::str;

    constexpr auto operator()(identifier_type const&) const { return *this; }

    static constexpr label_type label() { return kumi::str{"Stride"}; }

    static constexpr auto descriptor = Descriptor;
    static constexpr auto ndim = Descriptor.ndim;

    using storage_type = __::as_sequence<Descriptor>::type;

    constexpr stride() = default;

    //==================================================================================================================
    /**
      @brief Constructor that initializes the stride with explicit values

      This constructor allows you to initialize the stride with explicit values for each dimension. The number of
      arguments must match the number of dimensions specified in the `Descriptor`, and each argument must be
      convertible to `SizeType`. Additionally, the provided values must be compatible with the mapping defined by
      the `storage_type` associated with the `Descriptor`.

      @param s The stride values for each dimension.
    **/
    //==================================================================================================================
    template<std::convertible_to<SizeType>... S>
    requires(sizeof...(S) == Descriptor.ndim && storage_type::template follow_mapping<kumi::tuple<S...>>())
    constexpr stride(S... s) : data_{s...}
    {
    }

    //==================================================================================================================
    /**
      @brief Constructor that initializes the stride from a shape

      This constructor computes the stride values based on the dimensions of a given shape. The shape must have a
      descriptor that matches the `Descriptor` of the `stride`. The stride values are computed such that the first
      dimension has a stride of 1, and each subsequent dimension has a stride equal to the product of the previous
      dimension's stride and size.

      @param shp The shape from which to compute the stride.
    **/
    //==================================================================================================================
    template<shape_descriptor ShapeDesc>
    requires(__::make_stride_descriptor(ShapeDesc) == Descriptor)
    constexpr stride(shape<ShapeDesc, SizeType> const& shp) : stride(shp, std::make_index_sequence<ShapeDesc.ndim>{})
    {
    }

    template<std::size_t I> KWK_TRIVIAL friend constexpr decltype(auto) get(stride const& s)
    {
      if constexpr (I >= s.ndim) return kwk::fixed<0>;
      else return get<I>(s.data_);
    }

    template<std::size_t I> KWK_TRIVIAL friend constexpr decltype(auto) get(stride& s)
    {
      if constexpr (I >= s.ndim) return kwk::fixed<0>;
      else return get<I>(s.data_);
    }

  private:
    storage_type data_;

    //==================================================================================================================
    // Helper for computing the stride value for dimension K, handling both static and dynamic cases
    //==================================================================================================================
    template<std::size_t K, shape_descriptor ShapeDesc>
    static constexpr auto get_dim_val(shape<ShapeDesc, SizeType> const& shp)
    {
      if constexpr (ShapeDesc.dims[K] != static_cast<int>(kwk::_))
      {
        return kwk::fixed<ShapeDesc.dims[K]>; // Dimension statique
      }
      else
      {
        return get<K>(shp); // Dimension dynamique
      }
    }

    //==================================================================================================================
    // Helper for computing the stride value for dimension I, handling both static and dynamic cases
    //==================================================================================================================
    template<std::size_t I, shape_descriptor ShapeDesc>
    static constexpr auto get_stride_val(shape<ShapeDesc, SizeType> const& shp)
    {
      return [&]<std::size_t... K>(std::index_sequence<K...>) {
        return (kwk::fixed<1> * ... * get_dim_val<K>(shp));
      }(std::make_index_sequence<I>{});
    }

    //==================================================================================================================
    // Delegating constructor that computes the stride values from a shape and an index sequence
    //==================================================================================================================
    template<shape_descriptor ShapeDesc, std::size_t... I>
    constexpr stride(shape<ShapeDesc, SizeType> const& shp, std::index_sequence<I...>)
      : data_{get_stride_val<I>(shp)...}
    {
    }
  };

  template<typename... S> stride(S...) -> stride<__::make_descriptor<S...>()>;

  template<shape_descriptor ShapeDesc, typename SizeType>
  stride(shape<ShapeDesc, SizeType> const&) -> stride<__::make_stride_descriptor(ShapeDesc), SizeType>;
}

//======================================================================================================================
// Structured Binding Support
//======================================================================================================================
#if !defined(KWK_DOXYGEN_INVOKED)
template<kwk::shape_descriptor Descriptor, typename SizeType>
struct std::tuple_size<kwk::stride<Descriptor, SizeType>> : std::integral_constant<std::size_t, Descriptor.ndim>
{
};

template<std::size_t I, kwk::shape_descriptor Descriptor, typename SizeType>
struct std::tuple_element<I, kwk::stride<Descriptor, SizeType>>
{
  using type = decltype(get<I>(std::declval<kwk::stride<Descriptor, SizeType>>()));
};
#endif
