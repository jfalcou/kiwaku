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
  template<kumi::concepts::field Source, kumi::concepts::field... Options>
  KWK_TRIVIAL constexpr auto table(Source&& s, Options&&... opts)
  {
    auto settings = kwk::options{KWK_FWD(opts)...};

    auto shp = auto order auto def_stride auto strd auto allocator
  };

  template<typename Source, kumi::concepts::field... Opts>
  using table_t = decltype(table(kwk::source = std::declval<Source>(), std::declval<Opts>()...));
}
