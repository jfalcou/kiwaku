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
  //====================================================================================================================
  /**
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

    /**
      @brief Constructor for fully dynamic shapes

      Creates a shape descriptor with the specified number of dimensions,
      where all dimensions are marked as dynamic (wildcards).

      @param v Number of dimensions (must be >= 0 and <= max_dimensions)
    **/
    consteval shape_descriptor(int v) : ndim(v), fully_dynamic(true)
    {
      // assert(v >= 0 && v <= kwk::config::max_dimensions);
      for (int i = 0; i < v; ++i) dims[i] = _;
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

    /// @brief Equality comparison operator (default implementation)
    consteval bool operator==(shape_descriptor const&) const = default;
  };
}
