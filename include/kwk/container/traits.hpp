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
  //====================================================================================================================
  /**
    @ingroup container
    @brief Compile-time setup for view
  **/
  //====================================================================================================================
  struct view_options
  {
    shape_descriptor shape_ = _1D;
    shape_descriptor stride_ = shape_descriptor{1};
    storage_order_descriptor order_ = row_major_order.descriptor;
    int base_index_ = 0;
    bool valid_ = false;
  };

  namespace __
  {
    template<typename... Opts> using make_bag_t = decltype(options{std::declval<Opts>()...});

    //==================================================================================================================
    // view_traits: Centralized deduction for both compile-time options and runtime extraction
    //==================================================================================================================
    template<typename Bag, bool isValid = Bag::contains(kwk::source)> struct view_traits
    {
      using source_t = std::remove_cvref_t<decltype(std::declval<Bag>()[kwk::source])>;
      using source_type = typename source_t::source_type;

      static constexpr auto get_shape(Bag const& bag)
      {
        return bag.field_or(kwk::__::shape_id{}, shape_of(bag[kwk::source]));
      }

      using shape_type = decltype(get_shape(std::declval<Bag>()));

      static constexpr auto get_storage_order(Bag const& bag)
      {
        return bag.field_or(kwk::__::storage_order_id{}, kwk::row_major_order);
      }

      using storage_order_type = decltype(get_storage_order(std::declval<Bag>()));

      static constexpr auto get_stride(Bag const& bag)
      {
        auto def_stride = kwk::stride(get_shape(bag), get_storage_order(bag));
        return bag.field_or(kwk::__::stride_id{}, def_stride);
      }

      using stride_type = decltype(get_stride(std::declval<Bag>()));

      static constexpr view_options make_options()
      {
        view_options result{};
        result.shape_ = shape_type::descriptor;
        result.stride_ = stride_type::descriptor;
        result.order_ = storage_order_type::descriptor;
        result.valid_ = true;
        return result;
      }
    };

    template<typename Bag> struct view_traits<Bag, false>
    {
      using source_type = kumi::unit;

      static constexpr view_options make_options() { return {}; }
    };
  }
}
