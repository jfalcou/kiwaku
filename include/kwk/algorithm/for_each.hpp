//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <kwk/concepts/container.hpp>
#include <kwk/detail/algorithm/for_each.hpp>
#include <kwk/detail/abi.hpp>
#include <cstddef>
#include <utility>

namespace kwk
{
  //================================================================================================
  //! @addtogroup algorithms
  //! @{
  //================================================================================================

  //================================================================================================
  //! @brief Simple walkthrough over all possible indexes
  //!
  //! Applies the given function object `f` to every coordinates that are valid inside the given
  //! @ref kwk::shape `shp`. Coordinates are iterated in order of the optimal memory access.
  //!
  //! @param f 	  Function object, to be applied to a variadic list of integral coordinates. The
  //!             signature of the function should be equivalent to the following:
  //!             @code
  //!             void f(std::integral auto... is);
  //!             @endcode
  //!
  //! @param shp  A @ref kwk::shape used as boundaries for the coordinates
  //! @return     The function object `f` which could have been modified.
  //!
  //! @groupheader{Example}
  //! @include docs/algorithms/for_each_shape.cpp
  //================================================================================================
  template<typename Func, auto Desc>
  constexpr auto for_each(Func f, shape<Desc> const& shp)
  {
    return [&]<std::size_t... N>(std::index_sequence<N...> const&)
    {
      return __::for_each(f, shp );
    }( std::make_index_sequence<shape<Desc>::static_order>{} );
  }

  //================================================================================================
  //! @brief Simple walkthrough over all elements of a container
  //!
  //! Applies the given function object `f` to every elements of a given @ref kwk::container.
  //! Elements are iterated in order of the optimal memory access.
  //!
  //! @param f 	  Function object, to be applied to an element of `c`.  The
  //!             signature of the function should be equivalent to the following:
  //!             @code
  //!             void f(T&& v);
  //!             @endcode
  //!             with `T` convertible to the container's `value_type`.
  //! @param c    A @ref kwk::container on which elements `f` will be applied.
  //! @return     The function object `f` which could have been modified.
  //!
  //! @groupheader{Example}
  //! @include docs/algorithms/for_each.cpp
  //================================================================================================
  template<typename Func, concepts::container Container>
  constexpr auto for_each(Func f, Container&& c)
  {
    kwk::for_each([&](auto... is) { return f(KWK_FWD(c)(is...)); }, c.shape() );
    return f;
  }

  //================================================================================================
  //! @brief Simple walkthrough over all elements of a container and their coordinates
  //!
  //! Applies the given function object `f` to every elements of a given @ref kwk::container and
  //! their current coordinates. Elements are iterated in order of the optimal memory access.
  //!
  //! @param f 	  Function object, to be applied to an element of `c` and its coordinates.  The
  //!             signature of the function should be equivalent to the following:
  //!             @code
  //!             void f(T&& v, std::integral auto... is);
  //!             @endcode
  //!             with `T` convertible to the container's `value_type`.
  //! @param c    A @ref kwk::container on which elements `f` will be applied.
  //! @return     The function object `f` which could have been modified.
  //!
  //! @groupheader{Example}
  //! @include docs/algorithms/for_each_index.cpp
  //================================================================================================
  template<typename Func, concepts::container Container>
  constexpr auto for_each_index(Func f, Container&& c)
  {
    kwk::for_each ( [&](auto... is) { return f(KWK_FWD(c)(is...), is...); }
                  , c.shape()
                  );
    return f;
  }

  //================================================================================================
  //! @}
  //================================================================================================
}
