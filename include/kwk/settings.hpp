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
  /**
    @defgroup settings  Container Settings

    This group includes components that define various settings for KIWAKU containers.
  **/
  //====================================================================================================================

  //====================================================================================================================
  /**
  @ingroup settings
  @defgroup settings-kind  Container Data Type
  @brief Components related to the data type of the container elements
  **/
  //====================================================================================================================

  //====================================================================================================================
  /**
    @ingroup settings
    @defgroup settings-base-index  Container Base Index
    @brief Components related to the base index of the container
  **/
  //====================================================================================================================

  //====================================================================================================================
  /**
    @defgroup settings-allocator  Container Allocator
    @ingroup settings

    This group includes components that define the allocator used by KIWAKU containers, allowing users to specify
    custom memory management strategies for their containers.
  **/
  //====================================================================================================================

  //====================================================================================================================
  /**
    @ingroup settings
    @defgroup settings-source  Container Source
    @brief Components related to the source of the container
  **/
  //====================================================================================================================
}

#include <kwk/detail/detail.hpp>
#include <kwk/settings/options.hpp>

#include <kwk/settings/allocator.hpp>
// #include <kwk/settings/axis.hpp>
#include <kwk/settings/generator.hpp>
#include <kwk/settings/indexing.hpp>
#include <kwk/settings/kind.hpp>
// #include <kwk/settings/size.hpp>
#include <kwk/settings/source.hpp>
// #include <kwk/settings/stride.hpp>
