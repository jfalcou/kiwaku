//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kwk/utility/shape.hpp>

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
    template<typename Shape> struct shape_option;

    struct shape_id : kumi::identifier<shape_id>
    {
      template<shape_descriptor Desc, typename T> auto operator=(kwk::shape<Desc, T> const& v) const
      {
        return shape_option<kwk::shape<Desc, T>>{v};
      }

      friend constexpr auto to_str(shape_id) { return kumi::str{"Shape"}; }
    };

    template<typename Shape> struct shape_option
    {
      using element_type = Shape;
      using type = shape_option<Shape>;
      using identifier_type = shape_id;

      Shape value;

      constexpr auto operator()(identifier_type) const { return *this; }

      static constexpr auto name() { return kumi::str{"Shape"}; }

      template<typename CharT, typename Traits>
      friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                           shape_option const& s) noexcept
      {
        return os << "Shape : " << s.value;
      }
    };
  }
}
