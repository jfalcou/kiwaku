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
#include <kwk/utility/invoke.hpp>
#include <kwk/container/settings/category.hpp>

namespace kwk::tags
{
  struct callable_pick
  {
    KWK_DEFERS_CALLABLE(pick_);

    template<rbr::concepts::keyword Keyword, rbr::concepts::settings Settings>
    constexpr KWK_FORCEINLINE auto operator()(Keyword const& k, Settings const& opts) const noexcept
                ->  decltype(kwk::tag_invoke(*this, opts[category], k, opts))
    {
      return kwk::tag_invoke(*this, opts[category], k, opts);
    }

    template<rbr::concepts::keyword Keyword, rbr::concepts::settings Settings>
    unsupported_call<callable_pick(Keyword,Settings)>
    operator()(Keyword const& k, Settings const& opts) const
    requires(!requires { kwk::tag_invoke(*this, opts[category], k, opts); }) = delete;
  };
}

namespace kwk
{
  inline constexpr tags::callable_pick pick = {};

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

#include <kwk/container/settings/allocator.hpp>
#include <kwk/container/settings/label.hpp>
#include <kwk/container/settings/source.hpp>
#include <kwk/container/settings/size.hpp>
#include <kwk/container/settings/strides.hpp>
#include <kwk/container/settings/type.hpp>
