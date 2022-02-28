//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include "test.hpp"
#include <kiwaku/shape.hpp>

TTS_CASE( "Does a shape contains another" )
{
  TTS_EXPECT(  kwk::shape{3}       .contains(kwk::shape{2})       );
  TTS_EXPECT(  kwk::shape{3}       .contains(kwk::shape{3})       );
  TTS_EXPECT(  kwk::shape{3}       .contains(kwk::shape{3,1})     );
  TTS_EXPECT(  kwk::shape{3}       .contains(kwk::shape{3,1,1})   );
  TTS_EXPECT(  kwk::shape{3}       .contains(kwk::shape{3,1,1,1}) );

  TTS_EXPECT( (kwk::shape{3,5})    .contains(kwk::shape{2})       );
  TTS_EXPECT( (kwk::shape{3,5})    .contains(kwk::shape{2,2})     );
  TTS_EXPECT( (kwk::shape{3,5})    .contains(kwk::shape{3,5})     );
  TTS_EXPECT( (kwk::shape{3,5})    .contains(kwk::shape{3,5,1})   );
  TTS_EXPECT( (kwk::shape{3,5})    .contains(kwk::shape{3,5,1,1}) );

  TTS_EXPECT( (kwk::shape{3,5,7})  .contains(kwk::shape{2})       );
  TTS_EXPECT( (kwk::shape{3,5,7})  .contains(kwk::shape{3,5})     );
  TTS_EXPECT( (kwk::shape{3,5,7})  .contains(kwk::shape{2,2,1})   );
  TTS_EXPECT( (kwk::shape{3,5,7})  .contains(kwk::shape{3,5,1})   );
  TTS_EXPECT( (kwk::shape{3,5,7})  .contains(kwk::shape{3,5,7,1}) );

  TTS_EXPECT( (kwk::shape{3,5,7,9}).contains(kwk::shape{2})       );
  TTS_EXPECT( (kwk::shape{3,5,7,9}).contains(kwk::shape{2,4})     );
  TTS_EXPECT( (kwk::shape{3,5,7,9}).contains(kwk::shape{2,4,6})   );
  TTS_EXPECT( (kwk::shape{3,5,7,9}).contains(kwk::shape{2,4,6,8}) );
  TTS_EXPECT( (kwk::shape{3,5,7,9}).contains(kwk::shape{3,5,7,9}) );

  TTS_EXPECT_NOT(  kwk::shape{3}       .contains(kwk::shape{7})       );
  TTS_EXPECT_NOT(  kwk::shape{3}       .contains(kwk::shape{5,1})     );
  TTS_EXPECT_NOT(  kwk::shape{3}       .contains(kwk::shape{5,2})     );
  TTS_EXPECT_NOT(  kwk::shape{3}       .contains(kwk::shape{5,1,1})   );
  TTS_EXPECT_NOT(  kwk::shape{3}       .contains(kwk::shape{3,2,1})   );
  TTS_EXPECT_NOT(  kwk::shape{3}       .contains(kwk::shape{3,2,4})   );
  TTS_EXPECT_NOT(  kwk::shape{3}       .contains(kwk::shape{7,1,1,1}) );
  TTS_EXPECT_NOT(  kwk::shape{3}       .contains(kwk::shape{3,3,1,1}) );
  TTS_EXPECT_NOT(  kwk::shape{3}       .contains(kwk::shape{3,2,5,1}) );
  TTS_EXPECT_NOT(  kwk::shape{3}       .contains(kwk::shape{3,5,1,9}) );

  TTS_EXPECT_NOT( (kwk::shape{3,5})    .contains(kwk::shape{7})       );
  TTS_EXPECT_NOT( (kwk::shape{3,5})    .contains(kwk::shape{7,1})     );
  TTS_EXPECT_NOT( (kwk::shape{3,5})    .contains(kwk::shape{5,7})     );
  TTS_EXPECT_NOT( (kwk::shape{3,5})    .contains(kwk::shape{5,1,1})   );
  TTS_EXPECT_NOT( (kwk::shape{3,5})    .contains(kwk::shape{5,7,1})   );
  TTS_EXPECT_NOT( (kwk::shape{3,5})    .contains(kwk::shape{5,7,9})   );
  TTS_EXPECT_NOT( (kwk::shape{3,5})    .contains(kwk::shape{5,1,1,1}) );
  TTS_EXPECT_NOT( (kwk::shape{3,5})    .contains(kwk::shape{5,5,1,1}) );
  TTS_EXPECT_NOT( (kwk::shape{3,5})    .contains(kwk::shape{5,7,2,1}) );
  TTS_EXPECT_NOT( (kwk::shape{3,5})    .contains(kwk::shape{5,7,2,9}) );

  TTS_EXPECT_NOT( (kwk::shape{3,5,7})    .contains(kwk::shape{7})       );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7})    .contains(kwk::shape{7,1})     );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7})    .contains(kwk::shape{5,7})     );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7})    .contains(kwk::shape{5,1,1})   );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7})    .contains(kwk::shape{5,7,1})   );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7})    .contains(kwk::shape{5,7,9})   );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7})    .contains(kwk::shape{5,1,1,1}) );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7})    .contains(kwk::shape{5,5,1,1}) );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7})    .contains(kwk::shape{5,7,2,1}) );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7})    .contains(kwk::shape{5,7,2,9}) );

  TTS_EXPECT_NOT( (kwk::shape{3,5,7,9})    .contains(kwk::shape{7})       );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7,9})    .contains(kwk::shape{7,1})     );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7,9})    .contains(kwk::shape{5,7})     );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7,9})    .contains(kwk::shape{5,1,1})   );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7,9})    .contains(kwk::shape{5,7,1})   );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7,9})    .contains(kwk::shape{5,7,9})   );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7,9})    .contains(kwk::shape{5,1,1,1}) );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7,9})    .contains(kwk::shape{5,5,1,1}) );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7,9})    .contains(kwk::shape{5,7,2,1}) );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7,9})    .contains(kwk::shape{5,7,2,9}) );
};
