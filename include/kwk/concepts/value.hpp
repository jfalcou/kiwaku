//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

namespace kwk::concepts
{
  template<typename T>
  concept static_constant = requires(T)
  {
    { T::value };
  };
};