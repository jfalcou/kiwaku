//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kumi/kumi.hpp>

namespace kwk
{
  //====================================================================================================================
  /**
    @ingroup settings
    @defgroup settings-base-index  Container Base Index
    @brief Components related to the base index of the container
  **/
  //====================================================================================================================

  namespace __
  {
    // Value type carried by `base_index = <kumi::index<N>>` (also via `c_index`/`fortran_index`).
    // It *is* a `kumi::index_t<N>` (so the base `N` is directly readable through the inherited
    // static `value`) and also masquerades as a proper `kwk` option via `element_type`, which the
    // `maybe` monad used by `get<base_index>(opts)` requires. A bare `kumi::index_t<N>` cannot be
    // stored in an option bag: `maybe_properties` probes `T::element_type` and would fail.
    template<std::size_t N> struct base_index_option : kumi::index_t<N>
    {
      using element_type = kumi::index_t<N>;
    };
  }

  struct base_index_id : kumi::identifier<base_index_id>
  {
    template<std::size_t N> constexpr auto operator=(kumi::index_t<N> const&) const noexcept
    {
      return kumi::field<base_index_id, kwk::__::base_index_option<N>> {};
    }

    friend constexpr auto to_str(base_index_id) { return kumi::str {"Base Index"}; }
  };

  //====================================================================================================================
  /**
    @ingroup  settings-base-index
    @brief Identifier for the base index setting for KIWAKU containers

    Identifies the base index setting for KIWAKU containers, which specifies the base index to be
  used within the container.
  **/
  //====================================================================================================================
  inline constexpr base_index_id base_index    = {};

  //====================================================================================================================
  /**
    @ingroup  settings-base-index
    @brief Pre-defined settings for 0-based indexing
    @see base_index
  **/
  //====================================================================================================================
  inline constexpr auto          c_index       = (base_index = kumi::index<0>);

  //====================================================================================================================
  /**
    @ingroup  settings-base-index
    @brief Pre-defined settings for 1-based indexing
    @see base_index
  **/
  //====================================================================================================================
  inline constexpr auto          fortran_index = (base_index = kumi::index<1>);
}
