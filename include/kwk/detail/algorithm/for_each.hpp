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
  namespace detail
  {
    //==============================================================================================
    // n-Dimensional for_each generator
    // Piles up single for() lambda across each dimension then run them
    //==============================================================================================
    template<typename Func, typename Container, typename Dims>
    constexpr auto for_each(Func&& f, Container&& c, Dims const&ds)
    {
      auto loops = kumi::fold_right ( [](auto acc, std::size_t m)
                                      {
                                        return [=](auto... is)
                                        {
                                          for(std::size_t i = 0;i<m;++i)
                                            acc(i,is...);
                                        };
                                      }
                                    , ds
                                    , [&](auto... is)
                                      {
                                        return KWK_FWD(f)(KWK_FWD(c)(static_cast<std::size_t>(is)...));
                                      }
                                    );
      loops();
      return f;
    }

    //==============================================================================================
    // n-Dimensional for_each_index generator
    // Piles up single for() lambda across each dimension then run them
    //==============================================================================================
    template<typename Func, typename Container, typename Dims>
    constexpr auto for_each_index(Func&& f, Container&& c, Dims const& ds)
    {
      auto loops = kumi::fold_right ( [](auto acc, std::size_t m)
                                      {
                                        return [=](auto... is)
                                        {
                                          for(std::size_t i = 0;i<m;++i)
                                            acc(i,is...);
                                        };
                                      }
                                    , ds
                                    , [&](auto... is)
                                      {
                                        return KWK_FWD(f) ( KWK_FWD(c)(static_cast<std::size_t>(is)...)
                                                          , static_cast<std::size_t>(is)...
                                                          );
                                      }
                                    );
      loops();
      return f;
    }
  }
}
