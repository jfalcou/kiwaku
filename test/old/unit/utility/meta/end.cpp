//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/utility/end.hpp>
#include <kwk/utility/fixed.hpp>

TTS_CASE( "Check end default state" )
{
  using kwk::end;

  TTS_EQUAL(end.offset()   , 0);
  TTS_EQUAL(end.factor() , 1);
  TTS_EQUAL(end.divisor(), 1);
};

TTS_CASE( "Check end + k and k + end" )
{
  using kwk::end;

  TTS_EQUAL((end + 3).offset() ,  3);
  TTS_EQUAL((end + 3).ratio(), (kwk::ratio{1,1}));

  TTS_EQUAL((5 + end).offset() ,  5);
  TTS_EQUAL((5 + end).ratio(), (kwk::ratio{1,1}));
};

TTS_CASE( "Check end - k and k - end" )
{
  using kwk::end;

  TTS_EQUAL((end - 3).offset() ,  -3);
  TTS_EQUAL((end - 3).ratio(), (kwk::ratio{1,1}));

  TTS_EQUAL((5 - end).offset() ,  5);
  TTS_EQUAL((5 - end).ratio(), (kwk::ratio{-1,1}));
};

TTS_CASE( "Check end * k and k * end" )
{
  using kwk::end;

  TTS_EQUAL((end * 3).offset() ,  0);
  TTS_EQUAL((end * 3).ratio(), (kwk::ratio{3,1}));

  TTS_EQUAL(((end + 2) * 3).offset() ,  2*3);
  TTS_EQUAL(((end + 2) * 3).ratio(), (kwk::ratio{3,1}));

  TTS_EQUAL((5 * end).offset() ,  0);
  TTS_EQUAL((5 * end).ratio(), (kwk::ratio{5,1}));

  TTS_EQUAL((5 * (end + 7)).offset() , 5 * 7);
  TTS_EQUAL((5 * (end + 7)).ratio(), (kwk::ratio{5,1}));
};

TTS_CASE( "Check end / k and k / end and corner cases" )
{
  using kwk::end;

  TTS_EQUAL((end / 3).offset(),  0);
  TTS_EQUAL((end / 3).ratio() , (kwk::ratio{1,3}));

  TTS_EQUAL(((end + 2) / 3).offset(),  2);
  TTS_EQUAL(((end + 2) / 3).ratio() , (kwk::ratio{1,3}));

  TTS_EQUAL(((2 * end) / 3 + 1).offset(),  3);
  TTS_EQUAL(((2 * end) / 3 + 1).ratio() , (kwk::ratio{2,3}));

  TTS_EQUAL((((2 * end) / 3 + 1) / 2).offset(),  3);
  TTS_EQUAL((((2 * end) / 3 + 1) / 2).ratio() , (kwk::ratio{2,6}));
};

