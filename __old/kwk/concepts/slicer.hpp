//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kwk/concepts/extent.hpp>
#include <kwk/concepts/values.hpp>
#include <kwk/detail/stdfix.hpp>

namespace kwk::concepts
{
  template<typename S>
  inline constexpr bool is_contiguous = concepts::joker<S> || concepts::extremum <S> || std::integral<S>;

  template<typename S>
  requires( requires { S::has_step; } )
  inline constexpr bool is_contiguous<S>  = !S::has_step;

  //====================================================================================================================
  //! @brief slicer specifier concept
  //!
  //! A **KIWAKU** Slicer is a type which instances can be used to compute a sub-volume of a container.
  //====================================================================================================================
  template<typename S>
  concept slicer = requires(S const& s)
  {
    { origin  (shape<_>{} , s)                  };
    { reindex (shape<_>{} , s, kumi::index<0>)  };
    { reshape (shape<_>{} , s, kumi::index<0>)  };
    { restride(stride<_>{}, s, kumi::index<0>)  };
  };


  //====================================================================================================================
  //! @brief slicer specifier with unit step concept
  //!
  //! A kwk::unit_slicer is a type which fulfills kwk::slicer and doesn't break the contiguity of the dimension it
  //! slices.
  //====================================================================================================================
  template<typename S>
  concept unit_slicer = slicer<S> && is_contiguous<S>;

  template<typename S>
  concept interval_slicer = slicer<S> && requires(S) { typename S::kwk_is_interval; };

  template<typename S>
  concept unit_interval_slicer = interval_slicer<S> && is_contiguous<S>;
}
