//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kwk/configuration.hpp>
#include <kwk/utility/wildcard.hpp>

namespace kwk
{
  namespace __
  {
    struct rank
    {
      int value;
    };
  }

  //====================================================================================================================
  /**
    @ingroup shape-utility
    @brief Shape hybrid descriptor

    Helper class that describes the shape of multi-dimensional containers in KIWAKU.
    A shape descriptor can represent both static and dynamic dimensions, where dynamic
    dimensions are indicated by the wildcard value (_).

    The descriptor maintains an array of dimension sizes and tracks whether the shape
    is fully dynamic (all dimensions are wildcards) or partially static.
  **/
  //====================================================================================================================
  struct shape_descriptor
  {
    /// @brief Array storing the size of each dimension
    int dims[kwk::config::max_dimensions] = {};

    /// @brief Number of dimensions in the shape
    int ndim = 0;

    /// @brief Flag indicating if all dimensions are dynamic (wildcards)
    bool fully_dynamic = false;

    /// @brief Default constructor creating an empty shape descriptor
    consteval shape_descriptor() = default;

    consteval shape_descriptor(__::rank r) : ndim(r.value), fully_dynamic(true)
    {
      // assert(v >= 0 && v <= kwk::config::max_dimensions);
      for (int i = 0; i < ndim; ++i) dims[i] = _;
    }

    /**
      @brief Constructor from initializer list

      Creates a shape descriptor from a list of dimension sizes.
      Dimensions can be static (positive integers) or dynamic (wildcard _).
      The shape is considered fully dynamic only if all dimensions are wildcards.

      @param il List of dimension sizes (size must be <= max_dimensions)
    **/
    consteval shape_descriptor(std::initializer_list<int> il) : ndim(static_cast<int>(il.size()))
    {
      // assert(il.size() <= kwk::config::max_dimensions);
      int i = 0;
      fully_dynamic = true;
      for (int val : il)
      {
        dims[i++] = val;
        if (val != _) fully_dynamic = false;
      }
    }

    /// @brief Add a rank to the current shape_descriptor
    constexpr void add_rank(__::rank r) { dims[ndim++] = r.value; }

    /// @brief Equality comparison operator (default implementation)
    constexpr bool operator==(shape_descriptor const&) const = default;
  };

  namespace __
  {
    // Build the descriptor from a pack of types
    template<std::convertible_to<config::default_size_type>... Ts> consteval shape_descriptor make_descriptor()
    {
      auto val = []<typename T>() {
        if constexpr (is_dynamic_dim<T>::value) return _;
        else return T::value;
      };

      shape_descriptor d;
      d.ndim = sizeof...(Ts);

      int i = 0;
      ((d.dims[i++] = val.template operator()<Ts>()), ...);

      // Re-evaluate fully_dynamic flag
      d.fully_dynamic = true;
      for (int j = 0; j < d.ndim; ++j)
        if (d.dims[j] != _) d.fully_dynamic = false;

      return d;
    }

    //@brief Simply stores the indexes of non trivial dimension to be used as an index-map
    consteval shape_descriptor squeeze_descriptor(shape_descriptor sd)
    {
      shape_descriptor d;
      for (int i = 0; i < sd.ndim; ++i)
        if (sd.dims[i] != 1) d.dims[d.ndim++] = i;
      return d;
    }
  }

  inline namespace literals
  {
    //====================================================================================================================
    /**
      @ingroup shape-utility
      @brief Describe a  N-dimensions shape
      @relates shape_descriptor
    **/
    //====================================================================================================================
    template<int N> inline constexpr auto _nD = shape_descriptor{__::rank{N}};

    //====================================================================================================================
    /**
      @ingroup shape-utility
      @brief Describe a  1D shape
      @relates shape_descriptor
    **/
    //====================================================================================================================
    inline constexpr auto _1D = _nD<1>;

    //====================================================================================================================
    /**
      @ingroup shape-utility
      @brief Describe a  2D shape
      @relates shape_descriptor
    **/
    //====================================================================================================================
    inline constexpr auto _2D = _nD<2>;

    //====================================================================================================================
    /**
      @ingroup shape-utility
      @brief Describe a  3D shape
      @relates shape_descriptor
    **/
    //====================================================================================================================
    inline constexpr auto _3D = _nD<3>;

    //====================================================================================================================
    /**
      @ingroup shape-utility
      @brief Describe a  4D shape
      @relates shape_descriptor
    **/
    //====================================================================================================================
    inline constexpr auto _4D = _nD<4>;
  }
}
