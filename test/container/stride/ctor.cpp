//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include "test.hpp"
#include <kwk/container/stride.hpp>

TTS_CASE( "Building a 1D stride using stride" )
{
  TTS_EQUAL( kwk::with_stride(kwk::unit_).size()   , 1 );
  TTS_EQUAL( kwk::with_stride(kwk::unit_).get<0>() , 1 );
  TTS_CONSTEXPR_EXPECT( kwk::with_stride(kwk::unit_).is_unit  );
  TTS_CONSTEXPR_EXPECT_NOT( kwk::with_stride(kwk::unit_).is_implicit  );

  TTS_EQUAL( kwk::with_stride(9).size()   , 1 );
  TTS_EQUAL( kwk::with_stride(9).get<0>() , 9 );
  TTS_CONSTEXPR_EXPECT_NOT( kwk::with_stride(9).is_unit     );
  TTS_CONSTEXPR_EXPECT_NOT( kwk::with_stride(9).is_implicit );
};

TTS_CASE( "Building a 2D stride using stride" )
{
  TTS_EQUAL ( (kwk::with_stride(kwk::unit_,kwk::unit_).size()  ) , 2 );
  TTS_EQUAL ( (kwk::with_stride(kwk::unit_,kwk::unit_).get<0>()) , 1 );
  TTS_EQUAL ( (kwk::with_stride(kwk::unit_,kwk::unit_).get<1>()) , 1 );
  TTS_CONSTEXPR_EXPECT( (kwk::with_stride(kwk::unit_, kwk::unit_).is_unit) );
  TTS_CONSTEXPR_EXPECT_NOT( (kwk::with_stride(kwk::unit_, kwk::unit_).is_implicit) );

  TTS_EQUAL ( (kwk::with_stride(kwk::unit_, 42).size()  ) , 2 );
  TTS_EQUAL ( (kwk::with_stride(kwk::unit_, 42).get<0>()) , 1 );
  TTS_EQUAL ( (kwk::with_stride(kwk::unit_, 42).get<1>()) , 42);
  TTS_CONSTEXPR_EXPECT( (kwk::with_stride(kwk::unit_, 42).is_unit) );
  TTS_CONSTEXPR_EXPECT_NOT( (kwk::with_stride(kwk::unit_, 42).is_implicit) );

  TTS_EQUAL               ( (kwk::with_stride(6, 18).size()  ) ,  2 );
  TTS_EQUAL               ( (kwk::with_stride(6, 18).get<0>()) ,  6 );
  TTS_EQUAL               ( (kwk::with_stride(6, 18).get<1>()) , 18 );
  TTS_CONSTEXPR_EXPECT_NOT( (kwk::with_stride(6, 18).is_unit)       );
  TTS_CONSTEXPR_EXPECT_NOT( (kwk::with_stride(6, 18).is_implicit)   );
};

TTS_CASE( "Building a 3D stride using stride" )
{
  TTS_EQUAL ( (kwk::with_stride(kwk::unit_,kwk::unit_,kwk::unit_).size()  ) , 3 );
  TTS_EQUAL ( (kwk::with_stride(kwk::unit_,kwk::unit_,kwk::unit_).get<0>()) , 1 );
  TTS_EQUAL ( (kwk::with_stride(kwk::unit_,kwk::unit_,kwk::unit_).get<1>()) , 1 );
  TTS_EQUAL ( (kwk::with_stride(kwk::unit_,kwk::unit_,kwk::unit_).get<2>()) , 1 );
  TTS_CONSTEXPR_EXPECT( (kwk::with_stride(kwk::unit_,kwk::unit_,kwk::unit_).is_unit) );
  TTS_CONSTEXPR_EXPECT_NOT( (kwk::with_stride(kwk::unit_,kwk::unit_,kwk::unit_).is_implicit) );

  TTS_EQUAL( (kwk::with_stride(kwk::unit_,kwk::unit_,16).size()  ) , 3  );
  TTS_EQUAL( (kwk::with_stride(kwk::unit_,kwk::unit_,16).get<0>()) , 1  );
  TTS_EQUAL( (kwk::with_stride(kwk::unit_,kwk::unit_,16).get<1>()) , 1  );
  TTS_EQUAL( (kwk::with_stride(kwk::unit_,kwk::unit_,16).get<2>()) , 16 );
  TTS_CONSTEXPR_EXPECT( (kwk::with_stride(kwk::unit_,kwk::unit_,16).is_unit) );
  TTS_CONSTEXPR_EXPECT_NOT( (kwk::with_stride(kwk::unit_,kwk::unit_,16).is_implicit) );

  TTS_EQUAL( (kwk::with_stride(kwk::unit_,8,16).size()  ) , 3  );
  TTS_EQUAL( (kwk::with_stride(kwk::unit_,8,16).get<0>()) , 1  );
  TTS_EQUAL( (kwk::with_stride(kwk::unit_,8,16).get<1>()) , 8  );
  TTS_EQUAL( (kwk::with_stride(kwk::unit_,8,16).get<2>()) , 16 );
  TTS_CONSTEXPR_EXPECT( (kwk::with_stride(kwk::unit_,8,16).is_unit) );
  TTS_CONSTEXPR_EXPECT_NOT( (kwk::with_stride(kwk::unit_,8,16).is_implicit) );

  TTS_EQUAL( (kwk::with_stride(2,8,16).size()  ) , 3  );
  TTS_EQUAL( (kwk::with_stride(2,8,16).get<0>()) , 2  );
  TTS_EQUAL( (kwk::with_stride(2,8,16).get<1>()) , 8  );
  TTS_EQUAL( (kwk::with_stride(2,8,16).get<2>()) , 16 );
  TTS_CONSTEXPR_EXPECT_NOT( (kwk::with_stride(2,8,16).is_unit) );
  TTS_CONSTEXPR_EXPECT_NOT( (kwk::with_stride(2,8,16).is_implicit) );
};

TTS_CASE( "Building a 4D stride using stride" )
{
  TTS_EQUAL( (kwk::with_stride(kwk::unit_,kwk::unit_,kwk::unit_,kwk::unit_).size()  ), 4 );
  TTS_EQUAL( (kwk::with_stride(kwk::unit_,kwk::unit_,kwk::unit_,kwk::unit_).get<0>()), 1 );
  TTS_EQUAL( (kwk::with_stride(kwk::unit_,kwk::unit_,kwk::unit_,kwk::unit_).get<1>()), 1 );
  TTS_EQUAL( (kwk::with_stride(kwk::unit_,kwk::unit_,kwk::unit_,kwk::unit_).get<2>()), 1 );
  TTS_EQUAL( (kwk::with_stride(kwk::unit_,kwk::unit_,kwk::unit_,kwk::unit_).get<3>()), 1 );
  TTS_CONSTEXPR_EXPECT( (kwk::with_stride(kwk::unit_,kwk::unit_,kwk::unit_,kwk::unit_).is_unit) );
  TTS_CONSTEXPR_EXPECT_NOT( (kwk::with_stride(kwk::unit_,kwk::unit_,kwk::unit_,kwk::unit_).is_implicit) );

  TTS_EQUAL( (kwk::with_stride(kwk::unit_,kwk::unit_,kwk::unit_,32).size()  ), 4  );
  TTS_EQUAL( (kwk::with_stride(kwk::unit_,kwk::unit_,kwk::unit_,32).get<0>()), 1  );
  TTS_EQUAL( (kwk::with_stride(kwk::unit_,kwk::unit_,kwk::unit_,32).get<1>()), 1  );
  TTS_EQUAL( (kwk::with_stride(kwk::unit_,kwk::unit_,kwk::unit_,32).get<2>()), 1  );
  TTS_EQUAL( (kwk::with_stride(kwk::unit_,kwk::unit_,kwk::unit_,32).get<3>()), 32 );
  TTS_CONSTEXPR_EXPECT( (kwk::with_stride(kwk::unit_,kwk::unit_,kwk::unit_,32).is_unit) );
  TTS_CONSTEXPR_EXPECT_NOT( (kwk::with_stride(kwk::unit_,kwk::unit_,kwk::unit_,32).is_implicit) );

  TTS_EQUAL( (kwk::with_stride(kwk::unit_,kwk::unit_,16,32).size()  ), 4 );
  TTS_EQUAL( (kwk::with_stride(kwk::unit_,kwk::unit_,16,32).get<0>()), 1 );
  TTS_EQUAL( (kwk::with_stride(kwk::unit_,kwk::unit_,16,32).get<1>()), 1 );
  TTS_EQUAL( (kwk::with_stride(kwk::unit_,kwk::unit_,16,32).get<2>()), 16);
  TTS_EQUAL( (kwk::with_stride(kwk::unit_,kwk::unit_,16,32).get<3>()), 32);
  TTS_CONSTEXPR_EXPECT( (kwk::with_stride(kwk::unit_,kwk::unit_,16,32).is_unit) );
  TTS_CONSTEXPR_EXPECT_NOT( (kwk::with_stride(kwk::unit_,kwk::unit_,16,32).is_implicit) );

  TTS_EQUAL( (kwk::with_stride(kwk::unit_,8,16,32).size()  ), 4 );
  TTS_EQUAL( (kwk::with_stride(kwk::unit_,8,16,32).get<0>()), 1 );
  TTS_EQUAL( (kwk::with_stride(kwk::unit_,8,16,32).get<1>()), 8 );
  TTS_EQUAL( (kwk::with_stride(kwk::unit_,8,16,32).get<2>()), 16);
  TTS_EQUAL( (kwk::with_stride(kwk::unit_,8,16,32).get<3>()), 32);
  TTS_CONSTEXPR_EXPECT( (kwk::with_stride(kwk::unit_,8,16,32).is_unit) );
  TTS_CONSTEXPR_EXPECT_NOT( (kwk::with_stride(kwk::unit_,8,16,32).is_implicit) );

  TTS_EQUAL( (kwk::with_stride(2,8,16,32).size()  ), 4 );
  TTS_EQUAL( (kwk::with_stride(2,8,16,32).get<0>()), 2 );
  TTS_EQUAL( (kwk::with_stride(2,8,16,32).get<1>()), 8 );
  TTS_EQUAL( (kwk::with_stride(2,8,16,32).get<2>()), 16);
  TTS_EQUAL( (kwk::with_stride(2,8,16,32).get<3>()), 32);
  TTS_CONSTEXPR_EXPECT_NOT( (kwk::with_stride(2,8,16,32).is_unit) );
  TTS_CONSTEXPR_EXPECT_NOT( (kwk::with_stride(2,8,16,32).is_implicit) );
};
