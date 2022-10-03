//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#include "test.hpp"
#include "tts.hpp"
#include <kwk/utility/container/stride.hpp>
#include <kwk/utility/fixed.hpp>

TTS_CASE( "Building a 1D stride using with_strides" )
{
  using namespace kwk::literals;

  TTS_EQUAL( kwk::with_strides(1_c).size()    , 1U    );
  TTS_EQUAL( get<0>(kwk::with_strides(1_c))   , 1     );
  TTS_CONSTEXPR_EXPECT( kwk::with_strides(1_c).is_unit);

  TTS_EQUAL( kwk::with_strides(9).size()        , 1U    );
  TTS_EQUAL( get<0>(kwk::with_strides(9))       , 9     );
  TTS_CONSTEXPR_EXPECT_NOT( kwk::with_strides(9).is_unit);
};

TTS_CASE( "Building a 2D stride using with_strides" )
{
  using namespace kwk::literals;

  TTS_EQUAL ( (kwk::with_strides(1_c,7_c).size() )  , 2U      );
  TTS_EQUAL ( get<0>((kwk::with_strides(1_c,7_c)))  , 1       );
  TTS_EQUAL ( get<1>((kwk::with_strides(1_c,7_c)))  , 7       );
  TTS_CONSTEXPR_EXPECT( (kwk::with_strides(1_c, 7_c).is_unit) );

  TTS_EQUAL ( (kwk::with_strides(1_c, 42).size()  ), 2U     );
  TTS_EQUAL ( get<0>( (kwk::with_strides(1_c, 42))), 1      );
  TTS_EQUAL ( get<1>((kwk::with_strides(1_c, 42))) , 42     );
  TTS_CONSTEXPR_EXPECT( (kwk::with_strides(1_c, 42).is_unit));

  TTS_EQUAL ( (kwk::with_strides(1, 42_c).size()  )    , 2U     );
  TTS_EQUAL ( get<0>( (kwk::with_strides(1, 42_c)))    , 1      );
  TTS_EQUAL ( get<1>((kwk::with_strides(1, 42_c)))     , 42     );
  TTS_CONSTEXPR_EXPECT_NOT( (kwk::with_strides(1, 42_c).is_unit));

  TTS_EQUAL ( (kwk::with_strides(6, 18).size() )    ,  2U     );
  TTS_EQUAL ( get<0>((kwk::with_strides(6, 18)))    ,  6      );
  TTS_EQUAL ( get<1>((kwk::with_strides(6, 18)))    , 18      );
  TTS_CONSTEXPR_EXPECT_NOT((kwk::with_strides(6, 18).is_unit) );
};

TTS_CASE( "Building a 3D stride using with_strides" )
{
  using namespace kwk::literals;

  TTS_EQUAL ( (kwk::with_strides(1_c,4_c,20_c).size()  ), 3U     );
  TTS_EQUAL ( get<0>( (kwk::with_strides(1_c,4_c,20_c))), 1      );
  TTS_EQUAL ( get<1>((kwk::with_strides(1_c,4_c,20_c))) , 4      );
  TTS_EQUAL ( get<2>( (kwk::with_strides(1_c,4_c,20_c))), 20     );
  TTS_CONSTEXPR_EXPECT( (kwk::with_strides(1_c,4_c,20_c).is_unit));

  TTS_EQUAL( (kwk::with_strides(1_c,8_c,16).size() )  , 3U      );
  TTS_EQUAL( get<0>((kwk::with_strides(1_c,8_c,16)))  , 1       );
  TTS_EQUAL( get<1>((kwk::with_strides(1_c,8_c,16)))  , 8       );
  TTS_EQUAL( get<2>((kwk::with_strides(1_c,8_c,16)))  , 16      );
  TTS_CONSTEXPR_EXPECT( (kwk::with_strides(1_c,8_c,16).is_unit) );

  TTS_EQUAL( (kwk::with_strides(1_c,8,16_c).size() )  , 3U      );
  TTS_EQUAL( get<0>((kwk::with_strides(1_c,8,16_c)))  , 1       );
  TTS_EQUAL( get<1>((kwk::with_strides(1_c,8,16_c)))  , 8       );
  TTS_EQUAL( get<2>((kwk::with_strides(1_c,8,16_c)))  , 16      );
  TTS_CONSTEXPR_EXPECT( (kwk::with_strides(1_c,8,16_c).is_unit) );

  TTS_EQUAL( (kwk::with_strides(1_c,8,16).size() )  , 3U      );
  TTS_EQUAL( get<0>((kwk::with_strides(1_c,8,16)))  , 1       );
  TTS_EQUAL( get<1>((kwk::with_strides(1_c,8,16)))  , 8       );
  TTS_EQUAL( get<2>((kwk::with_strides(1_c,8,16)))  , 16      );
  TTS_CONSTEXPR_EXPECT( (kwk::with_strides(1_c,8,16).is_unit) );

  TTS_EQUAL ( (kwk::with_strides(3,6_c,12_c).size()  ), 3U          );
  TTS_EQUAL ( get<0>( (kwk::with_strides(3,6_c,12_c))), 3           );
  TTS_EQUAL ( get<1>((kwk::with_strides(3,6_c,12_c))) , 6           );
  TTS_EQUAL ( get<2>( (kwk::with_strides(3,6_c,12_c))), 12          );
  TTS_CONSTEXPR_EXPECT_NOT( (kwk::with_strides(3,6_c,12_c).is_unit) );

  TTS_EQUAL( (kwk::with_strides(3,6_c,12).size() )  , 3U          );
  TTS_EQUAL( get<0>((kwk::with_strides(3,6_c,12)))  , 3           );
  TTS_EQUAL( get<1>((kwk::with_strides(3,6_c,12)))  , 6           );
  TTS_EQUAL( get<2>((kwk::with_strides(3,6_c,12)))  , 12          );
  TTS_CONSTEXPR_EXPECT_NOT( (kwk::with_strides(3,6_c,12).is_unit) );

  TTS_EQUAL( (kwk::with_strides(3,6,12_c).size() )  , 3U          );
  TTS_EQUAL( get<0>((kwk::with_strides(3,6,12_c)))  , 3           );
  TTS_EQUAL( get<1>((kwk::with_strides(3,6,12_c)))  , 6           );
  TTS_EQUAL( get<2>((kwk::with_strides(3,6,12_c)))  , 12          );
  TTS_CONSTEXPR_EXPECT_NOT( (kwk::with_strides(3,6,12_c).is_unit) );

  TTS_EQUAL( (kwk::with_strides(3,6,12).size() )  , 3U          );
  TTS_EQUAL( get<0>((kwk::with_strides(3,6,12)))  , 3           );
  TTS_EQUAL( get<1>((kwk::with_strides(3,6,12)))  , 6           );
  TTS_EQUAL( get<2>((kwk::with_strides(3,6,12)))  , 12          );
  TTS_CONSTEXPR_EXPECT_NOT( (kwk::with_strides(3,6,12).is_unit) );
};

TTS_CASE( "Building a 4D stride using with_strides" )
{
  using namespace kwk::literals;

  TTS_EQUAL ( (kwk::with_strides(1_c,2_c,4_c,20_c).size()  ), 4U     );
  TTS_EQUAL ( get<0>( (kwk::with_strides(1_c,2_c,4_c,20_c))), 1      );
  TTS_EQUAL ( get<1>( (kwk::with_strides(1_c,2_c,4_c,20_c))), 2      );
  TTS_EQUAL ( get<2>( (kwk::with_strides(1_c,2_c,4_c,20_c))), 4      );
  TTS_EQUAL ( get<3>( (kwk::with_strides(1_c,2_c,4_c,20_c))), 20     );
  TTS_CONSTEXPR_EXPECT( (kwk::with_strides(1_c,2_c,4_c,20_c).is_unit));

  TTS_EQUAL ( (kwk::with_strides(1_c,2_c,8_c,16).size() ) , 4U      );
  TTS_EQUAL ( get<0>((kwk::with_strides(1_c,2_c,8_c,16))) , 1       );
  TTS_EQUAL ( get<1>((kwk::with_strides(1_c,2_c,8_c,16))) , 2       );
  TTS_EQUAL ( get<2>((kwk::with_strides(1_c,2_c,8_c,16))) , 8       );
  TTS_EQUAL ( get<3>((kwk::with_strides(1_c,2_c,8_c,16))) , 16      );
  TTS_CONSTEXPR_EXPECT( (kwk::with_strides(1_c,2_c,8_c,16).is_unit) );

  TTS_EQUAL ( (kwk::with_strides(1_c,2_c,8,16_c).size() ) , 4U      );
  TTS_EQUAL ( get<0>((kwk::with_strides(1_c,2_c,8,16_c))) , 1       );
  TTS_EQUAL ( get<1>((kwk::with_strides(1_c,2_c,8,16_c))) , 2       );
  TTS_EQUAL ( get<2>((kwk::with_strides(1_c,2_c,8,16_c))) , 8       );
  TTS_EQUAL ( get<3>((kwk::with_strides(1_c,2_c,8,16_c))) , 16      );
  TTS_CONSTEXPR_EXPECT( (kwk::with_strides(1_c,2_c,8,16_c).is_unit) );

  TTS_EQUAL ( (kwk::with_strides(1_c,2_c,8,16).size() ) , 4U      );
  TTS_EQUAL ( get<0>((kwk::with_strides(1_c,2_c,8,16))) , 1       );
  TTS_EQUAL ( get<1>((kwk::with_strides(1_c,2_c,8,16))) , 2       );
  TTS_EQUAL ( get<2>((kwk::with_strides(1_c,2_c,8,16))) , 8       );
  TTS_EQUAL ( get<3>((kwk::with_strides(1_c,2_c,8,16))) , 16      );
  TTS_CONSTEXPR_EXPECT( (kwk::with_strides(1_c,2_c,8,16).is_unit) );

  TTS_EQUAL ( (kwk::with_strides(1_c,3,6_c,12_c).size() ) , 4U      );
  TTS_EQUAL ( get<0>((kwk::with_strides(1_c,3,6_c,12_c))) , 1       );
  TTS_EQUAL ( get<1>((kwk::with_strides(1_c,3,6_c,12_c))) , 3       );
  TTS_EQUAL ( get<2>((kwk::with_strides(1_c,3,6_c,12_c))) , 6       );
  TTS_EQUAL ( get<3>((kwk::with_strides(1_c,3,6_c,12_c))) , 12      );
  TTS_CONSTEXPR_EXPECT( (kwk::with_strides(1_c,3,6_c,12_c).is_unit) );

  TTS_EQUAL ( (kwk::with_strides(1_c,3,6_c,12).size() ) , 4U      );
  TTS_EQUAL ( get<0>((kwk::with_strides(1_c,3,6_c,12))) , 1       );
  TTS_EQUAL ( get<1>((kwk::with_strides(1_c,3,6_c,12))) , 3       );
  TTS_EQUAL ( get<2>((kwk::with_strides(1_c,3,6_c,12))) , 6       );
  TTS_EQUAL ( get<3>((kwk::with_strides(1_c,3,6_c,12))) , 12      );
  TTS_CONSTEXPR_EXPECT( (kwk::with_strides(1_c,3,6_c,12).is_unit) );

  TTS_EQUAL ( (kwk::with_strides(1_c,3,6,12_c).size() ) , 4U      );
  TTS_EQUAL ( get<0>((kwk::with_strides(1_c,3,6,12_c))) , 1       );
  TTS_EQUAL ( get<1>((kwk::with_strides(1_c,3,6,12_c))) , 3       );
  TTS_EQUAL ( get<2>((kwk::with_strides(1_c,3,6,12_c))) , 6       );
  TTS_EQUAL ( get<3>((kwk::with_strides(1_c,3,6,12_c))) , 12      );
  TTS_CONSTEXPR_EXPECT( (kwk::with_strides(1_c,3,6,12_c).is_unit) );

  TTS_EQUAL ( (kwk::with_strides(1_c,3,6,12).size() )     , 4U  );
  TTS_EQUAL ( get<0>((kwk::with_strides(1_c,3,6,12)))     , 1   );
  TTS_EQUAL ( get<1>((kwk::with_strides(1_c,3,6,12)))     , 3   );
  TTS_EQUAL ( get<2>((kwk::with_strides(1_c,3,6,12)))     , 6   );
  TTS_EQUAL ( get<3>((kwk::with_strides(1_c,3,6,12)))     , 12  );
  TTS_CONSTEXPR_EXPECT( (kwk::with_strides(1_c,3,6,12).is_unit) );

  TTS_EQUAL ( (kwk::with_strides(1,2_c,4_c,20_c).size()  ), 4U     );
  TTS_EQUAL ( get<0>( (kwk::with_strides(1,2_c,4_c,20_c))), 1      );
  TTS_EQUAL ( get<1>( (kwk::with_strides(1,2_c,4_c,20_c))), 2      );
  TTS_EQUAL ( get<2>( (kwk::with_strides(1,2_c,4_c,20_c))), 4      );
  TTS_EQUAL ( get<3>( (kwk::with_strides(1,2_c,4_c,20_c))), 20     );
  TTS_CONSTEXPR_EXPECT_NOT( (kwk::with_strides(1,2_c,4_c,20_c).is_unit));

  TTS_EQUAL ( (kwk::with_strides(1,2_c,8_c,16).size() ) , 4U      );
  TTS_EQUAL ( get<0>((kwk::with_strides(1,2_c,8_c,16))) , 1       );
  TTS_EQUAL ( get<1>((kwk::with_strides(1,2_c,8_c,16))) , 2       );
  TTS_EQUAL ( get<2>((kwk::with_strides(1,2_c,8_c,16))) , 8       );
  TTS_EQUAL ( get<3>((kwk::with_strides(1,2_c,8_c,16))) , 16      );
  TTS_CONSTEXPR_EXPECT_NOT( (kwk::with_strides(1,2_c,8_c,16).is_unit) );

  TTS_EQUAL ( (kwk::with_strides(1,2_c,8,16_c).size() ) , 4U      );
  TTS_EQUAL ( get<0>((kwk::with_strides(1,2_c,8,16_c))) , 1       );
  TTS_EQUAL ( get<1>((kwk::with_strides(1,2_c,8,16_c))) , 2       );
  TTS_EQUAL ( get<2>((kwk::with_strides(1,2_c,8,16_c))) , 8       );
  TTS_EQUAL ( get<3>((kwk::with_strides(1,2_c,8,16_c))) , 16      );
  TTS_CONSTEXPR_EXPECT_NOT( (kwk::with_strides(1,2_c,8,16_c).is_unit) );

  TTS_EQUAL ( (kwk::with_strides(1,2_c,8,16).size() ) , 4U      );
  TTS_EQUAL ( get<0>((kwk::with_strides(1,2_c,8,16))) , 1       );
  TTS_EQUAL ( get<1>((kwk::with_strides(1,2_c,8,16))) , 2       );
  TTS_EQUAL ( get<2>((kwk::with_strides(1,2_c,8,16))) , 8       );
  TTS_EQUAL ( get<3>((kwk::with_strides(1,2_c,8,16))) , 16      );
  TTS_CONSTEXPR_EXPECT_NOT( (kwk::with_strides(1,2_c,8,16).is_unit) );

  TTS_EQUAL ( (kwk::with_strides(1,3,6_c,12_c).size() ) , 4U      );
  TTS_EQUAL ( get<0>((kwk::with_strides(1,3,6_c,12_c))) , 1       );
  TTS_EQUAL ( get<1>((kwk::with_strides(1,3,6_c,12_c))) , 3       );
  TTS_EQUAL ( get<2>((kwk::with_strides(1,3,6_c,12_c))) , 6       );
  TTS_EQUAL ( get<3>((kwk::with_strides(1,3,6_c,12_c))) , 12      );
  TTS_CONSTEXPR_EXPECT_NOT( (kwk::with_strides(1,3,6_c,12_c).is_unit) );

  TTS_EQUAL ( (kwk::with_strides(1,3,6_c,12).size() ) , 4U      );
  TTS_EQUAL ( get<0>((kwk::with_strides(1,3,6_c,12))) , 1       );
  TTS_EQUAL ( get<1>((kwk::with_strides(1,3,6_c,12))) , 3       );
  TTS_EQUAL ( get<2>((kwk::with_strides(1,3,6_c,12))) , 6       );
  TTS_EQUAL ( get<3>((kwk::with_strides(1,3,6_c,12))) , 12      );
  TTS_CONSTEXPR_EXPECT_NOT( (kwk::with_strides(1,3,6_c,12).is_unit) );

  TTS_EQUAL ( (kwk::with_strides(1,3,6,12_c).size() ) , 4U      );
  TTS_EQUAL ( get<0>((kwk::with_strides(1,3,6,12_c))) , 1       );
  TTS_EQUAL ( get<1>((kwk::with_strides(1,3,6,12_c))) , 3       );
  TTS_EQUAL ( get<2>((kwk::with_strides(1,3,6,12_c))) , 6       );
  TTS_EQUAL ( get<3>((kwk::with_strides(1,3,6,12_c))) , 12      );
  TTS_CONSTEXPR_EXPECT_NOT( (kwk::with_strides(1,3,6,12_c).is_unit) );

  TTS_EQUAL ( (kwk::with_strides(1,3,6,12).size() )     , 4U  );
  TTS_EQUAL ( get<0>((kwk::with_strides(1,3,6,12)))     , 1   );
  TTS_EQUAL ( get<1>((kwk::with_strides(1,3,6,12)))     , 3   );
  TTS_EQUAL ( get<2>((kwk::with_strides(1,3,6,12)))     , 6   );
  TTS_EQUAL ( get<3>((kwk::with_strides(1,3,6,12)))     , 12  );
  TTS_CONSTEXPR_EXPECT_NOT( (kwk::with_strides(1,3,6,12).is_unit) );
};
