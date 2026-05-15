//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

namespace kwk
{
  namespace __
  {
    template<template<typename...> typename T> struct builder_t
    {
      template<typename Bag> static constexpr auto get_source(Bag const& bag) { return bag[kwk::source]; }

      template<typename Bag> static constexpr auto get_allocator(Bag const& bag) { return bag[kwk::allocator]; }

      template<typename Bag> static constexpr auto get_base_index(Bag const& bag) { return bag[kwk::base_index]; }

      template<typename Bag> static constexpr auto get_layout(Bag const& bag)
      {
        if constexpr (!Bag::contains(kwk::source)) return kumi::none;
        else
        {
          auto shp = bag.field_or(kwk::of_shape, shape_of(bag[kwk::source]));
          auto so = bag.field_or(kwk::storage_order, kwk::row_major_order);
          auto strd = bag.field_or(kwk::of_stride, to_stride(shp, so));
          return kumi::tuple{shp, so, strd};
        }
      }

      template<typename Bag> static constexpr auto operator()(Bag const& bag)
      {
        auto filtered = kumi::flatten(kumi::tuple{get_layout(bag), get_base_index(bag), get_allocator(bag)});
        return kumi::apply([&](auto&&... elts) { return T{get_source(bag), KWK_FWD(elts)...}; }, filtered);
      }
    };
  }

  template<template<typename...> typename T> inline constexpr __::builder_t<T> builder{};
}
