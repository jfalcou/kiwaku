//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <kwk/concepts/container.hpp>
#include <kwk/context/context.hpp>
// #include <kwk/context/cpu/context.hpp>
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

  template<typename Context, typename Func, auto... S>
  constexpr auto for_each(Context ctx, Func f, shape<S...> const& shp)
  {
    return ctx.for_each(f, shp);
    // return [&]<std::size_t... N>(std::index_sequence<N...> const&)
    // {
    //   return __::for_each(f, shp );
    // }( std::make_index_sequence<shape<S...>::static_order>{} );
  }

  //================================================================================================
  //! @brief Simple walkthrough over all possible indexes
  //!
  //! Applies the given function object `f` to every coordinates that are valid inside the given
  //! kwk::shape `shp`. Coordinates are iterated in order of the optimal memory access.
  //!
  //! @param f 	  Function object, to be applied to a variadic list of integral coordinates. The
  //!             signature of the function should be equivalent to the following:
  //!             @code
  //!             void f(std::integral auto... is);
  //!             @endcode
  //!
  //! @param shp  A kwk::shape used as boundaries for the coordinates
  //! @return     The function object `f` which could have been modified.
  //!
  //! @groupheader{Example}
  //! @include docs/algorithms/for_each_shape.cpp
  //================================================================================================
  template<typename Func, auto... S>
  constexpr auto for_each(Func f, shape<S...> const& shp)
  {
    kwk::for_each(cpu, f, shp);
    // return [&]<std::size_t... N>(std::index_sequence<N...> const&)
    // {
    //   return __::for_each(f, shp );
    // }( std::make_index_sequence<shape<S...>::static_order>{} );
  }

  


  template<typename Context, typename Func, concepts::container C0, concepts::container... Cs>
  constexpr auto for_each(Context const& ctx, Func f, C0&& c0, Cs&&... cs)
  {
    ctx.for_each([&](auto... is) { return f(KWK_FWD(c0)(is...), KWK_FWD(cs)(is...)...); }, c0.shape() );
    return f;
  }
  
  //================================================================================================
  //! @brief Simple walkthrough over all elements of a container
  //!
  //! Applies the given function object `f` to every elements of a given kwk::container.
  //! Elements are iterated in order of the optimal memory access.
  //!
  //! @param f 	  Function object, to be applied to an element of `c`.  The
  //!             signature of the function should be equivalent to the following:
  //!             @code
  //!             void f(T&& v);
  //!             @endcode
  //!             with `T` convertible to the container's `value_type`.
  //! @param c    A kwk::container on which elements `f` will be applied.
  //! @return     The function object `f` which could have been modified.
  //!
  //! @groupheader{Example}
  //! @include docs/algorithms/for_each.cpp
  //================================================================================================
  template<typename Func, concepts::container C0, concepts::container... Cs>
  constexpr auto for_each(Func f, C0&& c0, Cs&&... cs)
  {
    // kwk::for_each([&](auto... is) { return f(KWK_FWD(c0)(is...), KWK_FWD(cs)(is...)...); }, c0.shape() );
    return kwk::for_each(cpu, f, KWK_FWD(c0), KWK_FWD(cs)...);
  }

  //================================================================================================
  //! @brief Simple walkthrough over all elements of a container and their coordinates
  //!
  //! Applies the given function object `f` to every elements of a given kwk::container and
  //! their current coordinates. Elements are iterated in order of the optimal memory access.
  //!
  //! @param f 	  Function object, to be applied to an element of `c` and its coordinates.  The
  //!             signature of the function should be equivalent to the following:
  //!             @code
  //!             void f(T&& v, std::integral auto... is);
  //!             @endcode
  //!             with `T` convertible to the container's `value_type`.
  //! @param c    A kwk::container on which elements `f` will be applied.
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
