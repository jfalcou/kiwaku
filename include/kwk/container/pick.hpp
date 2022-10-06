//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <kwk/detail/abi.hpp>
#include <kwk/detail/raberu.hpp>
#include <kwk/container/settings/allocator.hpp>
#include <kwk/container/settings/label.hpp>
#include <kwk/container/settings/size.hpp>
#include <kwk/container/settings/source.hpp>
#include <kwk/container/settings/strides.hpp>
#include <kwk/container/settings/type.hpp>

namespace kwk
{
  /// Retrieve a value from a settings pack using semantic category specialization
  template<rbr::concepts::keyword Keyword, rbr::concepts::settings Settings>
  KWK_FORCEINLINE constexpr auto pick(Keyword const& k, Settings const& opts)
  requires( Settings::contains(category).value )
  {
    return pick(opts[category], k, opts);
  }

  namespace result
  {
    template<auto Keyword, rbr::concepts::settings Settings>
    struct pick
    {
      using type = decltype( kwk::pick(Keyword, std::declval<Settings>() ));
    };

    template<auto Keyword, rbr::concepts::settings Settings>
    using pick_t = typename pick<Keyword, Settings>::type;
  }
}
