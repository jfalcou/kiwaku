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

namespace kwk::tag { struct table_ {}; }

namespace kwk::options
{
  template<typename Mode, rbr::concepts::settings Settings> struct element;
  template<typename Mode, rbr::concepts::settings Settings> struct data;

  // // The data source is fetched or an empty ptr_source is returned
  // template<rbr::concepts::settings Settings>
  // constexpr auto source(tag::table_ const&, Settings const& p) noexcept
  // {
  //   return p[kwk::source | ptr_source<void>{}];
  // }

  // Unless specified, the shape is retrieved from the source options
  template<rbr::concepts::settings Settings>
  constexpr auto shape(tag::table_ const&, Settings const& p) noexcept
  {
    return p[kwk::size | options::source(tag::table_{},p).default_shape()];
  }

  // Unless specified, the stride is computed from the shape
  template<rbr::concepts::settings Settings>
  constexpr auto stride(tag::table_ const&, Settings const& p) noexcept
  {
    return p[kwk::strides | options::shape(tag::table_{},p).as_stride() ];
  }

  // // For view, we infer the block type from the source that must be present
  // template<rbr::concepts::settings Settings>
  // struct data<tag::table_, Settings>
  // {
  //   using src_t = rbr::result::fetch_t<kwk::source | ptr_source<void>{}, Settings>;;
  //   using type = typename src_t::span_type;
  // };

  // // For view, we infer the type from the source that must be present
  // template<rbr::concepts::settings Settings>
  // struct element<tag::table_, Settings>
  // {
  //   using type = typename data<tag::table_,Settings>::type::base_type;
  // };
}
