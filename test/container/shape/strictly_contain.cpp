//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#include "test.hpp"
#include <kiwaku/container/shape.hpp>

TTS_CASE( "Does a shape strictly-contains another" )
{
  TTS_EXPECT(  kwk::shape{3}       .strictly_contains(kwk::shape{2})       );
  TTS_EXPECT(  kwk::shape{3}       .strictly_contains(kwk::shape{2,1})     );
  TTS_EXPECT(  kwk::shape{3}       .strictly_contains(kwk::shape{2,1,1})   );
  TTS_EXPECT(  kwk::shape{3}       .strictly_contains(kwk::shape{2,1,1,1}) );

  TTS_EXPECT( (kwk::shape{3,5})    .strictly_contains(kwk::shape{2})       );
  TTS_EXPECT( (kwk::shape{3,5})    .strictly_contains(kwk::shape{2,2})     );
  TTS_EXPECT( (kwk::shape{3,5})    .strictly_contains(kwk::shape{2,4,1})   );
  TTS_EXPECT( (kwk::shape{3,5})    .strictly_contains(kwk::shape{2,4,1,1}) );

  TTS_EXPECT( (kwk::shape{3,5,7})  .strictly_contains(kwk::shape{2})       );
  TTS_EXPECT( (kwk::shape{3,5,7})  .strictly_contains(kwk::shape{2,4})     );
  TTS_EXPECT( (kwk::shape{3,5,7})  .strictly_contains(kwk::shape{2,2,1})   );
  TTS_EXPECT( (kwk::shape{3,5,7})  .strictly_contains(kwk::shape{2,4,1})   );
  TTS_EXPECT( (kwk::shape{3,5,7})  .strictly_contains(kwk::shape{2,4,6,1}) );

  TTS_EXPECT( (kwk::shape{3,5,7,9}).strictly_contains(kwk::shape{2})       );
  TTS_EXPECT( (kwk::shape{3,5,7,9}).strictly_contains(kwk::shape{2,4})     );
  TTS_EXPECT( (kwk::shape{3,5,7,9}).strictly_contains(kwk::shape{2,4,6})   );
  TTS_EXPECT( (kwk::shape{3,5,7,9}).strictly_contains(kwk::shape{2,1,1,1}) );
  TTS_EXPECT( (kwk::shape{3,5,7,9}).strictly_contains(kwk::shape{2,4,1,1}) );
  TTS_EXPECT( (kwk::shape{3,5,7,9}).strictly_contains(kwk::shape{2,4,6,1}) );
  TTS_EXPECT( (kwk::shape{3,5,7,9}).strictly_contains(kwk::shape{2,4,6,8}) );

  TTS_EXPECT_NOT(  kwk::shape{3}       .strictly_contains(kwk::shape{3})       );
  TTS_EXPECT_NOT(  kwk::shape{3}       .strictly_contains(kwk::shape{7})       );
  TTS_EXPECT_NOT(  kwk::shape{3}       .strictly_contains(kwk::shape{5,1})     );
  TTS_EXPECT_NOT(  kwk::shape{3}       .strictly_contains(kwk::shape{5,2})     );
  TTS_EXPECT_NOT(  kwk::shape{3}       .strictly_contains(kwk::shape{5,1,1})   );
  TTS_EXPECT_NOT(  kwk::shape{3}       .strictly_contains(kwk::shape{3,2,1})   );
  TTS_EXPECT_NOT(  kwk::shape{3}       .strictly_contains(kwk::shape{3,2,4})   );
  TTS_EXPECT_NOT(  kwk::shape{3}       .strictly_contains(kwk::shape{7,1,1,1}) );
  TTS_EXPECT_NOT(  kwk::shape{3}       .strictly_contains(kwk::shape{3,3,1,1}) );
  TTS_EXPECT_NOT(  kwk::shape{3}       .strictly_contains(kwk::shape{3,2,5,1}) );
  TTS_EXPECT_NOT(  kwk::shape{3}       .strictly_contains(kwk::shape{3,5,1,9}) );

  TTS_EXPECT_NOT( (kwk::shape{3,5})    .strictly_contains(kwk::shape{7})       );
  TTS_EXPECT_NOT( (kwk::shape{3,5})    .strictly_contains(kwk::shape{7,1})     );
  TTS_EXPECT_NOT( (kwk::shape{3,5})    .strictly_contains(kwk::shape{3,5})     );
  TTS_EXPECT_NOT( (kwk::shape{3,5})    .strictly_contains(kwk::shape{5,7})     );
  TTS_EXPECT_NOT( (kwk::shape{3,5})    .strictly_contains(kwk::shape{5,1,1})   );
  TTS_EXPECT_NOT( (kwk::shape{3,5})    .strictly_contains(kwk::shape{5,7,1})   );
  TTS_EXPECT_NOT( (kwk::shape{3,5})    .strictly_contains(kwk::shape{5,7,9})   );
  TTS_EXPECT_NOT( (kwk::shape{3,5})    .strictly_contains(kwk::shape{5,1,1,1}) );
  TTS_EXPECT_NOT( (kwk::shape{3,5})    .strictly_contains(kwk::shape{5,5,1,1}) );
  TTS_EXPECT_NOT( (kwk::shape{3,5})    .strictly_contains(kwk::shape{5,7,2,1}) );
  TTS_EXPECT_NOT( (kwk::shape{3,5})    .strictly_contains(kwk::shape{5,7,2,9}) );

  TTS_EXPECT_NOT( (kwk::shape{3,5,7})    .strictly_contains(kwk::shape{7})       );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7})    .strictly_contains(kwk::shape{7,1})     );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7})    .strictly_contains(kwk::shape{5,7})     );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7})    .strictly_contains(kwk::shape{5,1,1})   );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7})    .strictly_contains(kwk::shape{5,7,1})   );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7})    .strictly_contains(kwk::shape{5,7,9})   );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7})    .strictly_contains(kwk::shape{3,5,7})   );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7})    .strictly_contains(kwk::shape{5,1,1,1}) );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7})    .strictly_contains(kwk::shape{5,5,1,1}) );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7})    .strictly_contains(kwk::shape{5,7,2,1}) );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7})    .strictly_contains(kwk::shape{5,7,2,9}) );

  TTS_EXPECT_NOT( (kwk::shape{3,5,7,9})    .strictly_contains(kwk::shape{7})       );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7,9})    .strictly_contains(kwk::shape{7,1})     );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7,9})    .strictly_contains(kwk::shape{5,7})     );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7,9})    .strictly_contains(kwk::shape{5,1,1})   );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7,9})    .strictly_contains(kwk::shape{5,7,1})   );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7,9})    .strictly_contains(kwk::shape{5,7,9})   );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7,9})    .strictly_contains(kwk::shape{5,1,1,1}) );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7,9})    .strictly_contains(kwk::shape{5,5,1,1}) );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7,9})    .strictly_contains(kwk::shape{5,7,2,1}) );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7,9})    .strictly_contains(kwk::shape{3,5,7,9}) );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7,9})    .strictly_contains(kwk::shape{5,7,2,9}) );
};
