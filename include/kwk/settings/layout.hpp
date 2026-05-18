//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kwk/utility/layout/storage_order.hpp>

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
      template<kumi::concepts::product_type T> constexpr auto operator=(T&& t) const noexcept
      {
        return as_shape(KWK_FWD(t));
      }

      friend constexpr auto to_str(shape_id) { return kumi::str{"Shape"}; }
    };

    struct stride_id : kumi::identifier<stride_id>
    {
      template<kumi::concepts::product_type T> constexpr auto operator=(T&& t) const noexcept
      {
        return as_stride(KWK_FWD(t));
      }

      friend constexpr auto to_str(stride_id) { return kumi::str{"Stride"}; }
    };

    struct storage_order_id : kumi::identifier<storage_order_id>
    {
      template<storage_order_descriptor O> constexpr auto operator=(storage_order_t<O>) const noexcept
      {
        return storage_order_t<O>{};
      }

      template<typename Func>
      requires(std::is_empty_v<Func>)
      constexpr auto operator=(Func) const noexcept
      {
        return storage_order_t<Func{}>{};
      }

      friend constexpr auto to_str(storage_order_id) { return kumi::str{"Storage Order"}; }
    };
  }

  inline constexpr __::shape_id of_shape{};
  inline constexpr __::stride_id of_stride{};
  inline constexpr __::storage_order_id storage_order{};
}
