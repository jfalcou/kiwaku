//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_ALGORITHM_ALL_HPP_INCLUDED
#define KIWAKU_ALGORITHM_ALL_HPP_INCLUDED

namespace kwk
{
  //================================================================================================
  // n-Dimensionnal all algorithm
  //================================================================================================
  template<typename Check, typename Dim0>
  bool all(Check c, Dim0 d0)
  {
    for(Dim0 i0=0;i0<d0;++i0)
    {
      if(!c(i0))
        return false;
    }

    return true;
  }

  template<typename Check, typename Dim, typename... Dims>
  bool all(Check c, Dim d0, Dims... ds)
  {
    return kwk::all( [c,d0](auto... is)
                    {
                      for(Dim i0=0;i0<d0;++i0) if(!c(i0,is...)) return false;
                      return true;
                    }
                    , ds...
                  );
  }
}

#endif
