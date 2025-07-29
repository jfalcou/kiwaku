//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#include <kwk/utility/position.hpp>
#include "test.hpp"

// for_each 1D - 4D

TTS_CASE("Check for kwk::position(std::array)")
{
  kwk::position<3> p1{{0, 1, 2}};

  TTS_EQUAL(p1[0], 0);
  TTS_EQUAL(p1[1], 1);
  TTS_EQUAL(p1[2], 2);

  kwk::position<3> p2;
  p2 = {0, 1, 2};

  TTS_EQUAL(p2[0], 0);
  TTS_EQUAL(p2[1], 1);
  TTS_EQUAL(p2[2], 2);

  // TTS_EQUAL(p1, p2);
  TTS_EQUAL(p1 == p2, true);

  std::stringstream ss;
  ss << p1;
  TTS_EQUAL(ss.str(), std::string{"position<3>(0, 1, 2)"});
};
