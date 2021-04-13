//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include "test.hpp"
#include <kiwaku/stride.hpp>

TTS_CASE( "Building a 1D stride using with_stride" )
{
  TTS_EQUAL( kwk::with_stride(kwk::_1).size()   , 1 );
  TTS_EQUAL( kwk::with_stride(kwk::_1).get<0>() , 1 );
  TTS_EXPECT( kwk::with_stride(kwk::_1).is_unit     );

  TTS_EQUAL( kwk::with_stride(9).size()   , 1 );
  TTS_EQUAL( kwk::with_stride(9).get<0>() , 9 );
  TTS_EXPECT_NOT( kwk::with_stride(9).is_unit );
}

TTS_CASE( "Building a 2D stride using with_stride" )
{
  TTS_EQUAL( kwk::with_stride(kwk::_1,kwk::_1).size()   , 2 );
  TTS_EQUAL( kwk::with_stride(kwk::_1,kwk::_1).get<0>() , 1 );
  TTS_EQUAL( kwk::with_stride(kwk::_1,kwk::_1).get<1>() , 1 );
  TTS_EXPECT( kwk::with_stride(kwk::_1, kwk::_1).is_unit    );

  TTS_EQUAL( kwk::with_stride(kwk::_1, 42).size()   , 2 );
  TTS_EQUAL( kwk::with_stride(kwk::_1, 42).get<0>() , 1 );
  TTS_EQUAL( kwk::with_stride(kwk::_1, 42).get<1>() , 42);
  TTS_EXPECT( kwk::with_stride(kwk::_1, 42).is_unit     );

  TTS_EQUAL( kwk::with_stride(6, 18).size()   ,  2);
  TTS_EQUAL( kwk::with_stride(6, 18).get<0>() ,  6);
  TTS_EQUAL( kwk::with_stride(6, 18).get<1>() , 18);
  TTS_EXPECT_NOT( kwk::with_stride(6, 18).is_unit );
}

TTS_CASE( "Building a 3D stride using with_stride" )
{
  TTS_EQUAL( kwk::with_stride(kwk::_1,kwk::_1,kwk::_1).size()   , 3 );
  TTS_EQUAL( kwk::with_stride(kwk::_1,kwk::_1,kwk::_1).get<0>() , 1 );
  TTS_EQUAL( kwk::with_stride(kwk::_1,kwk::_1,kwk::_1).get<1>() , 1 );
  TTS_EQUAL( kwk::with_stride(kwk::_1,kwk::_1,kwk::_1).get<2>() , 1 );
  TTS_EXPECT( kwk::with_stride(kwk::_1,kwk::_1,kwk::_1).is_unit     );

  TTS_EQUAL( kwk::with_stride(kwk::_1,kwk::_1,16).size()   , 3  );
  TTS_EQUAL( kwk::with_stride(kwk::_1,kwk::_1,16).get<0>() , 1  );
  TTS_EQUAL( kwk::with_stride(kwk::_1,kwk::_1,16).get<1>() , 1  );
  TTS_EQUAL( kwk::with_stride(kwk::_1,kwk::_1,16).get<2>() , 16 );
  TTS_EXPECT( kwk::with_stride(kwk::_1,kwk::_1,16).is_unit      );

  TTS_EQUAL( kwk::with_stride(kwk::_1,8,16).size()   , 3  );
  TTS_EQUAL( kwk::with_stride(kwk::_1,8,16).get<0>() , 1  );
  TTS_EQUAL( kwk::with_stride(kwk::_1,8,16).get<1>() , 8  );
  TTS_EQUAL( kwk::with_stride(kwk::_1,8,16).get<2>() , 16 );
  TTS_EXPECT( kwk::with_stride(kwk::_1,8,16).is_unit      );

  TTS_EQUAL( kwk::with_stride(2,8,16).size()   , 3  );
  TTS_EQUAL( kwk::with_stride(2,8,16).get<0>() , 2  );
  TTS_EQUAL( kwk::with_stride(2,8,16).get<1>() , 8  );
  TTS_EQUAL( kwk::with_stride(2,8,16).get<2>() , 16 );
  TTS_EXPECT_NOT( kwk::with_stride(2,8,16).is_unit  );
}

TTS_CASE( "Building a 4D stride using with_stride" )
{
  TTS_EQUAL( kwk::with_stride(kwk::_1,kwk::_1,kwk::_1,kwk::_1).size()   , 4 );
  TTS_EQUAL( kwk::with_stride(kwk::_1,kwk::_1,kwk::_1,kwk::_1).get<0>() , 1 );
  TTS_EQUAL( kwk::with_stride(kwk::_1,kwk::_1,kwk::_1,kwk::_1).get<1>() , 1 );
  TTS_EQUAL( kwk::with_stride(kwk::_1,kwk::_1,kwk::_1,kwk::_1).get<2>() , 1 );
  TTS_EQUAL( kwk::with_stride(kwk::_1,kwk::_1,kwk::_1,kwk::_1).get<3>() , 1 );
  TTS_EXPECT( kwk::with_stride(kwk::_1,kwk::_1,kwk::_1,kwk::_1).is_unit     );

  TTS_EQUAL( kwk::with_stride(kwk::_1,kwk::_1,kwk::_1,32).size()   , 4  );
  TTS_EQUAL( kwk::with_stride(kwk::_1,kwk::_1,kwk::_1,32).get<0>() , 1  );
  TTS_EQUAL( kwk::with_stride(kwk::_1,kwk::_1,kwk::_1,32).get<1>() , 1  );
  TTS_EQUAL( kwk::with_stride(kwk::_1,kwk::_1,kwk::_1,32).get<2>() , 1  );
  TTS_EQUAL( kwk::with_stride(kwk::_1,kwk::_1,kwk::_1,32).get<3>() , 32 );
  TTS_EXPECT( kwk::with_stride(kwk::_1,kwk::_1,kwk::_1,32).is_unit      );

  TTS_EQUAL( kwk::with_stride(kwk::_1,kwk::_1,16,32).size()   , 4 );
  TTS_EQUAL( kwk::with_stride(kwk::_1,kwk::_1,16,32).get<0>() , 1 );
  TTS_EQUAL( kwk::with_stride(kwk::_1,kwk::_1,16,32).get<1>() , 1 );
  TTS_EQUAL( kwk::with_stride(kwk::_1,kwk::_1,16,32).get<2>() , 16);
  TTS_EQUAL( kwk::with_stride(kwk::_1,kwk::_1,16,32).get<3>() , 32);
  TTS_EXPECT( kwk::with_stride(kwk::_1,kwk::_1,16,32).is_unit     );

  TTS_EQUAL( kwk::with_stride(kwk::_1,8,16,32).size()   , 4 );
  TTS_EQUAL( kwk::with_stride(kwk::_1,8,16,32).get<0>() , 1 );
  TTS_EQUAL( kwk::with_stride(kwk::_1,8,16,32).get<1>() , 8 );
  TTS_EQUAL( kwk::with_stride(kwk::_1,8,16,32).get<2>() , 16);
  TTS_EQUAL( kwk::with_stride(kwk::_1,8,16,32).get<3>() , 32);
  TTS_EXPECT( kwk::with_stride(kwk::_1,8,16,32).is_unit     );

  TTS_EQUAL( kwk::with_stride(2,8,16,32).size()   , 4 );
  TTS_EQUAL( kwk::with_stride(2,8,16,32).get<0>() , 2 );
  TTS_EQUAL( kwk::with_stride(2,8,16,32).get<1>() , 8 );
  TTS_EQUAL( kwk::with_stride(2,8,16,32).get<2>() , 16);
  TTS_EQUAL( kwk::with_stride(2,8,16,32).get<3>() , 32);
  TTS_EXPECT_NOT( kwk::with_stride(2,8,16,32).is_unit );
}
