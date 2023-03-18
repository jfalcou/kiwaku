//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <kwk/detail/abi.hpp>
#include <kwk/detail/kumi.hpp>
#include <cstddef>
#include <utility>

namespace kwk
{
  namespace __
  {
    //==============================================================================================
    // n-Dimensional for_each generator
    // Piles up single for() lambda across each dimension then run them
    //==============================================================================================
    template<typename Func, typename Dims>
    constexpr auto for_each(Func&& f, Dims const& ds)
    {
      auto loops = kumi::fold_left ( [](auto acc, auto m)
                                      {
                                        return [=](auto... is)
                                        {
                                          for(to_int_t<decltype(m)> i = 0;i<m;++i)
                                            acc(is...,i);
                                        };
                                      }
                                    , ds
                                    , [&](auto... is) { return KWK_FWD(f)(is...); }
                                    );
      loops();
      return f;
    }
  }
}
