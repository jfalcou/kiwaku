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
  struct table_type
  {
    template<kumi::concepts::field... Options> KWK_ABI constexpr auto operator()(Options&&... opts) const
    {
      // Appending `deduce_allocator` makes the builder allocate a fresh owning region
      // (an inline stack blob for fully-static geometry, a heap blob otherwise) and copy
      // the source's bytes into it. The resulting collection's element type keeps the
      // source's constness, which is why the copy is a raw byte copy (not an element-wise
      // assignment through a — possibly const — accessor).
      return builder(kwk::options{KWK_FWD(opts)..., kwk::__::deduce_allocator});
    }
  } inline constexpr table;

  template<kumi::concepts::field... Opts> using table_t = decltype(table(std::declval<Opts>()...));
}
