//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kwk/utility/shape/storage_order.hpp>

namespace kwk
{
  //====================================================================================================================
  /**
    @ingroup settings
    @defgroup settings-shape  Container Shape
    @brief Components related to the shape of the container
  **/
  //====================================================================================================================
  namespace __
  {
    struct shape_id : kumi::identifier<shape_id>
    {
    };

    struct stride_id : kumi::identifier<stride_id>
    {
    };

    template<typename T> struct storage_order_option;

    struct storage_order_id : kumi::identifier<storage_order_id>
    {
      template<storage_order_descriptor O> constexpr auto operator=(storage_order_t<O>) const noexcept
      {
        return storage_order_option<storage_order_t<O>>{};
      }

      template<typename Func>
      requires(std::is_empty_v<Func>)
      constexpr auto operator=(Func f) const noexcept
      {
        return storage_order_option<storage_order_t<Func{}>>{};
      }

      friend constexpr auto to_str(storage_order_id) { return kumi::str{"Storage Order"}; }
    };

    template<typename T> struct storage_order_option
    {
      using element_type = T;
      using type = storage_order_option<T>;
      using identifier_type = storage_order_id;
      using label_type = kumi::str;

      constexpr auto operator()(identifier_type) const { return T{}; }

      static constexpr label_type label() { return kumi::str{"Storage Order"}; }

      template<typename CharT, typename Traits>
      friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                           storage_order_option const&) noexcept
      {
        if constexpr (row_major_order == T::descriptor) return os << "Storage Order : Row Major";
        else if constexpr (column_major_order == T::descriptor) return os << "Storage Order : Column Major";
        else return os << "Storage Order : Custom";
      }
    };
  }

  inline constexpr __::storage_order_id storage_order = {};
}
