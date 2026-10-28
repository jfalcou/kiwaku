//======================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================
#pragma once

#include <kiwaku/detail/abi.hpp>

namespace kwk
{
  //====================================================================================================================
  //! @ingroup utility
  //! @brief A constant, function-backed value source.
  //!
  //! A `constant_blob` never owns memory: it is a *dumb* templated type holding a compile-time
  //! function `Generator` that maps a coordinate to a value. Every read simply re-evaluates the
  //! function, which makes it the natural backend for functionally-defined (constant or otherwise
  //! position-dependent) spaces.
  //!
  //! @tparam Value      the type of the value produced by the source.
  //! @tparam Generator  a callable used as a non-type template parameter (a `constexpr` function
  //!                    pointer or a value of an empty constexpr-callable class).
  //====================================================================================================================
  template<typename Value, auto Generator> struct constant_blob
  {
    //! The type of the values produced by this source.
    using value_type = Value;

    //! The kind of this source (a pointer-like placeholder so it can sit in a collection's
    //! options).
    using data_type  = void const*;

    //! Identity accessor : a generator-backed collection reaches its storage as the
    //! `constant_blob` base; `storage()` returns it unambiguously (this is the only option type
    //! defining `storage()` for a function-backed collection).
    constexpr constant_blob const& storage() const noexcept { return *this; }

    //==================================================================================================================
    //! Evaluate the generator at the given coordinates.
    //==================================================================================================================
    template<typename... Coords>
    [[nodiscard]] constexpr value_type operator()(Coords&&... coords) const
        noexcept(noexcept(Generator(KWK_FWD(coords)...)))
    {
      return Generator(KWK_FWD(coords)...);
    }
  };

  //! Type synonym for `constant_blob<Value, Generator>`.
  template<typename Value, auto Generator> using constant_blob_t = constant_blob<Value, Generator>;
}
