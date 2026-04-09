//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kwk/utility/shape.hpp>

namespace kwk
{
  //====================================================================================================================
  /**
    @ingroup settings
    @defgroup settings-shape  Container Shape
    @brief Components related to the shape of the container
  **/
  //====================================================================================================================
  namespace __
  {
    struct shape_id : kumi::identifier<shape_id>
    {
      // friend constexpr auto to_str(shape_id) { return kumi::str{"Shape"}; }
    };
  }
}
