//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include "test.hpp"
#include <kiwaku/components/shape.hpp>

TTS_CASE( "nbdims behavior on nD shape" )
{
  TTS_EQUAL( (kwk::of_size(4,2,6,9).nbdims()), 4);
  TTS_EQUAL( (kwk::of_size(4,2,6,1).nbdims()), 3);
  TTS_EQUAL( (kwk::of_size(4,2,1,1).nbdims()), 2);
  TTS_EQUAL( (kwk::of_size(4,1,1,1).nbdims()), 1);
  TTS_EQUAL( (kwk::of_size(1,1,1,1).nbdims()), 0);
};

TTS_CASE( "numel behavior on nD shape" )
{
  TTS_EQUAL( (kwk::of_size(4,2,6,9).numel()), 432 );
  TTS_EQUAL( (kwk::of_size(4,2,6,1).numel()), 48  );
  TTS_EQUAL( (kwk::of_size(4,2,1,1).numel()), 8   );
  TTS_EQUAL( (kwk::of_size(4,1,1,1).numel()), 4   );
  TTS_EQUAL( (kwk::of_size(1,1,1,1).numel()), 1   );
};
