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
  template<kumi::concepts::field... Options> KWK_TRIVIAL constexpr auto table(Options&&... opts)
  {
    return builder<kwk::collection>(kwk::options{KWK_FWD(opts)..., kwk::__::deduce_allocator});
  }

  template<kumi::concepts::field... Opts> using table_t = decltype(table(std::declval<Opts>()...));
}
