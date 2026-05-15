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

  /*
    @brief  View factory taking user options and passing them in the correct order to the collection class to build
            the concrete object. Options are always passed in the same order thus two views with the same options
            in different orders are guaranteed to yield the same type.
  */
  template<kumi::concepts::field Source, kumi::concepts::field... Options>
  KWK_TRIVIAL constexpr auto view(Source&& s, Options&&... opts)
  {
    return builder<kwk::collection>(kwk::options{KWK_FWD(s), KWK_FWD(opts)...});
  }

  template<typename Source, kumi::concepts::field... Opts>
  using view_t = decltype(view(kwk::source = std::declval<Source>(), std::declval<Opts>()...));
}
