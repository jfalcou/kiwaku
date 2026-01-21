//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/settings/type.hpp>

TTS_CASE("Predefined type settings")
{
  TTS_TYPE_IS(decltype(kwk::real32)::element_type, float);
  TTS_TYPE_IS(decltype(kwk::real64)::element_type, double);
  TTS_TYPE_IS(decltype(kwk::int8)::element_type, std::int8_t);
  TTS_TYPE_IS(decltype(kwk::int16)::element_type, std::int16_t);
  TTS_TYPE_IS(decltype(kwk::int32)::element_type, std::int32_t);
  TTS_TYPE_IS(decltype(kwk::int64)::element_type, std::int64_t);
  TTS_TYPE_IS(decltype(kwk::uint8)::element_type, std::uint8_t);
  TTS_TYPE_IS(decltype(kwk::uint16)::element_type, std::uint16_t);
  TTS_TYPE_IS(decltype(kwk::uint32)::element_type, std::uint32_t);
  TTS_TYPE_IS(decltype(kwk::uint64)::element_type, std::uint64_t);
};
