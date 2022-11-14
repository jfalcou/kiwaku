//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/concepts/extent.hpp>
#include <kwk/detail/sequence/combo.hpp>
#include <kwk/detail/kumi.hpp>

namespace kwk::detail
{
  // Convert a sequence of values to a proper extent type
  template<typename SizeType, typename ... Ds>
  constexpr auto as_extent(Ds... ds) noexcept
  {
    return kumi::fold_right( []<typename T>(auto a, T)
                            {
                              if constexpr( requires{ T::value; } ) return a[T::value];
                              else                                  return a();
                            }
                          , kumi::tie(ds...)
                          , detail::combo<SizeType>{}
                          );
  }

  template< template<auto> class Wrapper
          , typename SizeType
          , concepts::extent<SizeType>... Ds
          >
  constexpr auto make_extent(Ds... ds) noexcept
  {
    return kumi::apply( [](auto... v)
                        {
                          return  Wrapper<as_extent<SizeType>(Ds{}...)>(v...);
                        }
                      , kumi::tuple{ds...}
                      );
  }
}
