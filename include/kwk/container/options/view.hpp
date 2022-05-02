//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#pragma once

#include <kwk/detail/raberu.hpp>
#include <kwk/options.hpp>

namespace kwk::tag { struct view_ {}; }

namespace kwk::options
{
  template<typename Mode, rbr::concepts::settings Settings> struct element;
  template<typename Mode, rbr::concepts::settings Settings> struct data;

  // The data source is fetched or an empty ptr_source is returned
  template<rbr::concepts::settings Settings>
  constexpr auto source(tag::view_ const&, Settings const& p) noexcept
  {
    return p[kwk::source | ptr_source<void>{}];
  }

  // Unless specified, the shape is retrieved from the source options
  template<rbr::concepts::settings Settings>
  constexpr auto shape(tag::view_ const&, Settings const& p) noexcept
  {
    return p[kwk::size | options::source(tag::view_{},p).default_shape()];
  }

  // Unless specified, the stride is computed from the shape
  template<rbr::concepts::settings Settings>
  constexpr auto stride(tag::view_ const&, Settings const& p) noexcept
  {
    return p[kwk::strides | options::shape(tag::view_{},p).as_stride() ];
  }

  // Unless retrieved from options, the base_index is index_<0>
  template<rbr::concepts::settings Settings>
  constexpr auto base_index(tag::view_ const&, Settings const& p) noexcept
  {
    return p[kwk::base_index | index<0> ];
  }

  // Compute the offset from base_index and stride
  template<rbr::concepts::settings Settings>
  constexpr auto offset(tag::view_ const& m, Settings const& p) noexcept
  {
    auto st = stride(m,p);
    auto bi = base_index(m,p).template as_position<decltype(st)::static_size>();
    return kumi::apply([&](auto... i) { return st.index(i...); }, bi);
  }

  // For view, we infer the block type from the source that must be present
  template<rbr::concepts::settings Settings>
  struct data<tag::view_, Settings>
  {
    using src_t = rbr::result::fetch_t<kwk::source | ptr_source<void>{}, Settings>;
    using type = typename src_t::span_type;
  };

  // For view, we infer the type from the source that must be present
  template<rbr::concepts::settings Settings>
  struct element<tag::view_, Settings>
  {
    using type = typename data<tag::view_,Settings>::type::base_type;
  };
}
