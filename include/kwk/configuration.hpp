//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

//======================================================================================================================
// Check if the user has provided a custom configuration file in their include path
//======================================================================================================================
#if __has_include(<kwk_config.hpp>)
#include <kwk_config.hpp>
#else
//======================================================================================================================
// Default Library Configuration
//======================================================================================================================
namespace kwk::config
{
  /// @brief Maximum number of dimensions for KIWAKU's containers
  inline constexpr int max_dimensions = 32;
}
#endif
