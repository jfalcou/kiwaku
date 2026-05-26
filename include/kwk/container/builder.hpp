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
      template<typename Bag> static constexpr auto get_base_index(Bag const& bag) { return bag[kwk::base_index]; }

      template<typename Bag> static constexpr auto get_source(Bag const& bag) { return bag[kwk::source]; }

      template<typename Bag> static consteval auto get_type(Bag const& bag)
      {
        if constexpr (Bag::contains(kwk::source))
        {
          using qtype = decltype(source_pointer(get_source(std::declval<Bag>())));
          return as<std::remove_pointer_t<std::remove_cvref_t<qtype>>>();
        }
        else return bag.field_or(kwk::kind, kwk::real64);
      }

      template<typename Bag> static constexpr auto get_shape(Bag const& bag)
      {
        if constexpr (Bag::contains(kwk::source)) return bag.field_or(kwk::of_shape, shape_of(bag[kwk::source]));
        else return bag[kwk::of_shape];
      }

      template<typename Bag> static constexpr auto get_allocator(Bag const& bag)
      {
        if constexpr (!Bag::contains(kwk::allocator) && Bag::contains(kwk::__::deduce_allocator))
        {
          using shp_t = decltype(get_shape(std::declval<Bag>()));
          if constexpr (kumi::concepts::unit_type<shp_t>) return kumi::none;
          else
          {
            if constexpr (shp_t::has_dynamic_dim) return kwk::heap;
            else return kwk::__::deduce_allocator;
          }
        }
        else return bag[kwk::allocator];
      }

      template<typename Bag> static constexpr auto get_layout(Bag const& bag)
      {
        if constexpr (!Bag::contains(kwk::source)) return kumi::none;
        else
        {
          using shp_t = decltype(get_shape(std::declval<Bag>()));
          if constexpr (kumi::concepts::unit_type<shp_t>) return kumi::none;
          else if constexpr (Bag::contains(kwk::of_stride))
          {
            auto so = bag.field_or(kwk::storage_order, kwk::row_major_order);
            auto shp = get_shape(bag);
            auto strd = bag[kwk::of_stride];
            return kumi::tuple{shp, so, strd};
          }
          else
          {
            auto so = bag.field_or(kwk::storage_order, kwk::row_major_order);
            auto shp = get_shape(bag);
            auto strd = bag.field_or(kwk::of_stride, to_stride(shp, so));
            return kumi::tuple{shp, so, strd};
          }
        }
      }

      template<typename Bag> static constexpr auto get_storage(Bag const& bag)
      {
        using shp_t = decltype(get_shape(std::declval<Bag>()));
        using alloc_t = decltype(get_allocator(std::declval<Bag>()));
        using tmp_t = decltype(get_type(std::declval<Bag>()));
        using type_t = typename tmp_t::element_type;

        using source_t = decltype(get_source(std::declval<Bag>()));

        if constexpr (!kumi::concepts::unit_type<alloc_t>)
        {
          if constexpr (!shp_t::has_dynamic_dim && std::same_as<alloc_t, default_allocator>)
          {
            auto b = blob<type_t, kwk::stack_blob<alignof(type_t), shp_t{}.size() * sizeof(type_t)>>{};
            b.initialize(shp_t{}.size());
            if constexpr (!kumi::concepts::unit_type<source_t>)
            {
              b.copy(source_pointer(get_source(bag)), shp_t{}.size());
            }
            return b;
          }
          else
          {
            auto size = get_shape(bag).size();
            auto b = blob<type_t, kwk::heap_blob>{get_allocator(bag), static_cast<std::size_t>(size) * sizeof(type_t)};
            b.initialize(static_cast<std::size_t>(size));
            if constexpr (!kumi::concepts::unit_type<source_t>)
            {
              b.copy(source_pointer(get_source(bag)), static_cast<std::size_t>(size));
            }
            return b;
          }
        }
        else return blob<type_t, kwk::shallow_blob>{source_pointer(get_source(bag))};
      }

      template<typename Bag> static constexpr auto operator()(Bag const& bag)
      {
        auto filtered = kumi::flatten(kumi::tuple{get_layout(bag), get_storage(bag),
                                                  get_base_index(bag) /*, kwk::allocator = get_allocator(bag)*/});
        return kumi::apply([&](auto&&... elts) { return T{KWK_FWD(elts)...}; }, filtered);
      }
    };
  }

  template<template<typename...> typename T> inline constexpr __::builder_t<T> builder{};
}
