//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#include "test.hpp"
#include <kiwaku/memory/blob/constant_blob.hpp>

using kwk::constant_blob;

// A constexpr-generated constant source
constexpr auto one   = [](auto...) { return 1.0; };

// A constexpr position-dependent source
constexpr auto id_of = [](auto... xs) { return (xs + ...); };

TTS_CASE("constant_blob - produces its value on demand, storing no data")
{
  constant_blob<double, one> constant;

  // Same value regardless of coordinates
  TTS_EQUAL(constant(), 1.0);
  TTS_EQUAL(constant(2), 1.0);
  TTS_EQUAL(constant(2, 3, 4), 1.0);

  // No data member: the generator is a non-type template parameter
  TTS_EQUAL(sizeof(constant), 1u);
};

TTS_CASE("constant_blob - position-dependent generator")
{
  constant_blob<std::size_t, id_of> idx;

  TTS_EQUAL(idx(0u), 0u);
  TTS_EQUAL(idx(2u, 3u), 5u);
  TTS_EQUAL(idx(1u, 2u, 4u), 7u);
};
