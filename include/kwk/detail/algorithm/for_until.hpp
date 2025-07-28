//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <kwk/detail/abi.hpp>
#include <kumi/kumi.hpp>
#include <cstddef>
#include <utility>

namespace kwk
{
  namespace __
  {
    template<typename Func, typename Dims>
    constexpr auto for_until(Func f, Dims const& ds)
    {
      bool found = false;
      auto loops = kumi::fold_left ( [&found](auto acc, auto m)
                                      {
                                        return [&found,acc,m](auto... is)
                                        {
                                          for(to_int_t<decltype(m)> i = 0;!found && i<m;++i)
                                            acc(is...,i);
                                        };
                                      }
                                    , ds
                                    , [&](auto... is) { found = f(is...); }
                                    );
      loops();
      return found;
    }
  }
}