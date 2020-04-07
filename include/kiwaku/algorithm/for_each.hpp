//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_ALGORITHM_FOR_EACH_HPP_INCLUDED
#define KIWAKU_ALGORITHM_FOR_EACH_HPP_INCLUDED

namespace kwk
{
  //================================================================================================
  // n-Dimensionnal for_each algorithm
  //================================================================================================
  template<typename Check, typename Dim0>
  consetxpr void iterate(Check c, Dim0 d0)
  {
    for(Dim0 i0=0;i0<d0;++i0)
      c(i0);
  }

  template<typename Check, typename Dim, typename... Dims>
  void iterate(Check c, Dim d0, Dims... ds)
  {
    iterate ( [c,d0](auto... is)
              {
                for(Dim i0=0;i0<d0;++i0)
                  c(i0,is...);
              }, ds...
            );
  }
}

#endif
