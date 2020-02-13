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
#include <tts/tests/basic.hpp>

TTS_CASE( "Equality comparison between shapes" )
{
  TTS_EXPECT(  kwk::shape{3}        ==  kwk::shape{3}        );
  TTS_EXPECT(  kwk::shape{3}        == (kwk::shape{3,1})     );
  TTS_EXPECT(  kwk::shape{3}        == (kwk::shape{3,1,1})   );
  TTS_EXPECT(  kwk::shape{3}        == (kwk::shape{3,1,1,1}) );

  TTS_EXPECT( (kwk::shape{3,5})     == (kwk::shape{3,5})     );
  TTS_EXPECT( (kwk::shape{3,5})     == (kwk::shape{3,5,1})   );
  TTS_EXPECT( (kwk::shape{3,5})     == (kwk::shape{3,5,1,1}) );

  TTS_EXPECT( (kwk::shape{3,5,7})   == (kwk::shape{3,5,7})   );
  TTS_EXPECT( (kwk::shape{3,5,7})   == (kwk::shape{3,5,7,1}) );

  TTS_EXPECT( (kwk::shape{3,5,7,9}) == (kwk::shape{3,5,7,9}) );

  TTS_EXPECT_NOT(  kwk::shape{3}        ==  kwk::shape{7}         );
  TTS_EXPECT_NOT(  kwk::shape{3}        == (kwk::shape{3,2})      );
  TTS_EXPECT_NOT(  kwk::shape{3}        == (kwk::shape{3,2,4})    );
  TTS_EXPECT_NOT(  kwk::shape{3}        == (kwk::shape{3,3,5,7})  );

  TTS_EXPECT_NOT( (kwk::shape{3,5})     == (kwk::shape{7})        );
  TTS_EXPECT_NOT( (kwk::shape{3,5})     == (kwk::shape{3,7})      );
  TTS_EXPECT_NOT( (kwk::shape{3,5})     == (kwk::shape{3,5,7})    );
  TTS_EXPECT_NOT( (kwk::shape{3,5})     == (kwk::shape{3,5,7,9})  );

  TTS_EXPECT_NOT( (kwk::shape{3,5,7})   == (kwk::shape{3})        );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7})   == (kwk::shape{3,5})      );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7})   == (kwk::shape{3,5,9})    );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7})   == (kwk::shape{3,5,7,9})  );

  TTS_EXPECT_NOT( (kwk::shape{3,5,7,9}) == (kwk::shape{3})        );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7,9}) == (kwk::shape{3,5})      );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7,9}) == (kwk::shape{3,5,7})    );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7,9}) == (kwk::shape{3,5,7,2})  );
}

TTS_CASE( "Inequality comparison between shapes" )
{
  TTS_EXPECT(  kwk::shape{3}        !=  kwk::shape{7}        );
  TTS_EXPECT(  kwk::shape{3}        != (kwk::shape{3,2})     );
  TTS_EXPECT(  kwk::shape{3}        != (kwk::shape{3,2,4})   );
  TTS_EXPECT(  kwk::shape{3}        != (kwk::shape{3,3,5,7}) );

  TTS_EXPECT( (kwk::shape{3,5})     != (kwk::shape{7})       );
  TTS_EXPECT( (kwk::shape{3,5})     != (kwk::shape{3,7})     );
  TTS_EXPECT( (kwk::shape{3,5})     != (kwk::shape{3,5,7})   );
  TTS_EXPECT( (kwk::shape{3,5})     != (kwk::shape{3,5,7,9}) );

  TTS_EXPECT( (kwk::shape{3,5,7})   != (kwk::shape{3})       );
  TTS_EXPECT( (kwk::shape{3,5,7})   != (kwk::shape{3,5})     );
  TTS_EXPECT( (kwk::shape{3,5,7})   != (kwk::shape{3,5,9})   );
  TTS_EXPECT( (kwk::shape{3,5,7})   != (kwk::shape{3,5,7,9}) );

  TTS_EXPECT( (kwk::shape{3,5,7,9}) != (kwk::shape{3})        );
  TTS_EXPECT( (kwk::shape{3,5,7,9}) != (kwk::shape{3,5})      );
  TTS_EXPECT( (kwk::shape{3,5,7,9}) != (kwk::shape{3,5,7})    );
  TTS_EXPECT( (kwk::shape{3,5,7,9}) != (kwk::shape{3,5,7,2})  );

  TTS_EXPECT_NOT(  kwk::shape{3}        !=  kwk::shape{3}        );
  TTS_EXPECT_NOT(  kwk::shape{3}        != (kwk::shape{3,1})     );
  TTS_EXPECT_NOT(  kwk::shape{3}        != (kwk::shape{3,1,1})   );
  TTS_EXPECT_NOT(  kwk::shape{3}        != (kwk::shape{3,1,1,1}) );

  TTS_EXPECT_NOT( (kwk::shape{3,5})     != (kwk::shape{3,5})     );
  TTS_EXPECT_NOT( (kwk::shape{3,5})     != (kwk::shape{3,5,1})   );
  TTS_EXPECT_NOT( (kwk::shape{3,5})     != (kwk::shape{3,5,1,1}) );

  TTS_EXPECT_NOT( (kwk::shape{3,5,7})   != (kwk::shape{3,5,7})   );
  TTS_EXPECT_NOT( (kwk::shape{3,5,7})   != (kwk::shape{3,5,7,1}) );

  TTS_EXPECT_NOT( (kwk::shape{3,5,7,9}) != (kwk::shape{3,5,7,9}) );
}
