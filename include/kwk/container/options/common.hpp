//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/container/options/forward.hpp>
#include <kwk/detail/raberu.hpp>
#include <kwk/options.hpp>

namespace kwk::options
{
  // For containers, we infer the block type from the source that must be present
  template<typename Tag, rbr::concepts::settings Settings>
  struct data
  {
    using type = decltype( block(Tag{}, std::declval<Settings>()) );
  };

  // The data source is fetched or an empty ptr_source is returned
  template<typename Tag, rbr::concepts::settings Settings>
  constexpr auto source(Tag const&, Settings const& p) noexcept
  {
    return p[kwk::source | ptr_source<rbr::unknown_key>{}];
  }

  // The shape has retrieved from the options or error occurs
  template<typename Tag, rbr::concepts::settings Settings>
  constexpr auto shape(Tag const& tag, Settings const& p) noexcept
  {
    return p[kwk::size | options::source(tag,p).default_shape()];
  }

  // Unless specified, the stride is computed from the shape
  template<typename Tag, rbr::concepts::settings Settings>
  constexpr auto stride(Tag const& m, Settings const& p) noexcept
  {
    return p[kwk::strides | options::shape(m,p).as_stride() ];
  }

  // Unless retrieved from options, the base_index is index_<0>
  template<typename Tag, rbr::concepts::settings Settings>
  constexpr auto base_index(Tag const&, Settings const& p) noexcept
  {
    return p[kwk::base_index | bases<0> ];
  }

  // Compute the offset from base_index and stride
  template<typename Tag, rbr::concepts::settings Settings>
  constexpr auto offset(Tag const& m, Settings const& p) noexcept
  {
    auto st = stride(m,p);
    auto bi = base_index(m,p).template as_position<decltype(st)::static_size>();
    return kumi::apply([&](auto... i) { return st.index(i...); }, bi);
  }
}
