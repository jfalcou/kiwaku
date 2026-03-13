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
  struct base_index_id : kumi::identifier<base_index_id>
  {
    using kumi::identifier<base_index_id>::operator=;

    friend constexpr auto to_str(base_index_id) { return kumi::str{"Base Index"}; }
  };

  //====================================================================================================================
  /**
    @ingroup  settings-base-index
    @brief Identifier for the base index setting for KIWAKU containers

    Identifies the base index setting for KIWAKU containers, which specifies the base index to be used within the
    container.
  **/
  //====================================================================================================================
  inline constexpr base_index_id base_index = {};

  //====================================================================================================================
  /**
    @ingroup  settings-base-index
    @brief Pre-defined settings for 0-based indexing
    @see base_index
  **/
  //====================================================================================================================
  inline constexpr auto c_index = (base_index = kumi::index<0>);

  //====================================================================================================================
  /**
    @ingroup  settings-base-index
    @brief Pre-defined settings for 1-based indexing
    @see base_index
  **/
  //====================================================================================================================
  inline constexpr auto fortran_index = (base_index = kumi::index<1>);
}
