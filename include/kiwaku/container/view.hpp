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
  struct view_type
  {
    // A view cannot have an explicit allocator option
    template<kumi::concepts::field Source, kumi::concepts::field... Options>
    KWK_ABI constexpr auto operator()(Source&& s, Options&&... opts) const
    {
      return builder(kwk::options{KWK_FWD(s), KWK_FWD(opts)...});
    }
  } inline constexpr view;

  template<typename Source, kumi::concepts::field... Opts>
  using view_t = decltype(view(kwk::source = std::declval<Source>(), std::declval<Opts>()...));
}
