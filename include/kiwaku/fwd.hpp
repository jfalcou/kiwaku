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
  /*
    @brief Forward declaration of the concrete container-view object.

    `collection` is the concrete output type produced by the builder and every sub-view. It is a
    variadic template over its component property types `Storage, Shape, Stride, Scheme, Accessor,
    Kind` (the exact argument order of its constructor, deduced by CTAD). It is forward-declared
    here — and only here — so that traits and the builder can reference the (incomplete) type
    without pulling in the full definition and creating an include cycle.
  */
  template<typename... Opts> struct collection;

  //====================================================================================================================
  /*
    Internal machinery shared between the builder and the container's sub-view builders.
  */
  namespace __
  {
    // The function object that assembles a `kwk::collection` from a bag of user options.
    struct builder_t;

    //==================================================================================================================
    /*
      @brief The single construction authority for `kwk::collection`.

      Assembles a `kwk::collection` from its six fully-resolved components (storage, shape, stride,
      scheme, accessor, kind). Every concrete collection in KIWAKU is created here and only here —
      the top-level builder (`builder_t::operator()`) and the sub-view builders (`get<I>`,
      slice/tile `make_view` in `collection.hpp`) both route their construction through it, so there
      is exactly one place that knows how to assemble the component pack.

      The function is declared here so the sub-view builders in `collection.hpp` can reach it
      without an include cycle (its definition lives in `builder.hpp`, which includes
      `collection.hpp`). Sub-views pass their resolved scheme/accessor/kind directly, so their
      byte-exact semantics (e.g. a product kind's per-field offset scheme) are preserved — this is
      the scheme-aware entry point that a plain re-derivation through the options bag could not
      express.
    */
    template<typename Storage,
             typename Shape,
             typename Stride,
             typename Scheme,
             typename Accessor,
             typename Kind>
    constexpr auto make_collection(Storage&&  store,
                                   Shape&&    shp,
                                   Stride&&   strd,
                                   Scheme&&   scheme,
                                   Accessor&& acc,
                                   Kind&&     kind);
  }
}
