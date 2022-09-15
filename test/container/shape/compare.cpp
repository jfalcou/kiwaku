//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier:   KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#include "test.hpp"
#include <kwk/container/shape.hpp>

TTS_CASE( "Equality comparison between shapes" )
{
  TTS_EQUAL(  kwk::of_size(3)       ,  kwk::of_size(3)        );
  TTS_EQUAL(  kwk::of_size(3)       , (kwk::of_size(3,1))     );
  TTS_EQUAL(  kwk::of_size(3)       , (kwk::of_size(3,1,1))   );
  TTS_EQUAL(  kwk::of_size(3)       , (kwk::of_size(3,1,1,1)) );

  TTS_EQUAL( (kwk::of_size(3,5))    , (kwk::of_size(3,5))     );
  TTS_EQUAL( (kwk::of_size(3,5))    , (kwk::of_size(3,5,1))   );
  TTS_EQUAL( (kwk::of_size(3,5))    , (kwk::of_size(3,5,1,1)) );

  TTS_EQUAL( (kwk::of_size(3,5,7))  , (kwk::of_size(3,5,7))   );
  TTS_EQUAL( (kwk::of_size(3,5,7))  , (kwk::of_size(3,5,7,1)) );

  TTS_EQUAL( (kwk::of_size(3,5,7,9)), (kwk::of_size(3,5,7,9)) );

  TTS_EXPECT_NOT(  kwk::of_size(3)        == kwk::of_size(7)          );
  TTS_EXPECT_NOT(  kwk::of_size(3)        == (kwk::of_size(3,2))      );
  TTS_EXPECT_NOT(  kwk::of_size(3)        == (kwk::of_size(3,2,4))    );
  TTS_EXPECT_NOT(  kwk::of_size(3)        == (kwk::of_size(3,3,5,7))  );

  TTS_EXPECT_NOT( (kwk::of_size(3,5))     == (kwk::of_size(7))        );
  TTS_EXPECT_NOT( (kwk::of_size(3,5))     == (kwk::of_size(3,7))      );
  TTS_EXPECT_NOT( (kwk::of_size(3,5))     == (kwk::of_size(3,5,7))    );
  TTS_EXPECT_NOT( (kwk::of_size(3,5))     == (kwk::of_size(3,5,7,9))  );

  TTS_EXPECT_NOT( (kwk::of_size(3,5,7))   == (kwk::of_size(3))        );
  TTS_EXPECT_NOT( (kwk::of_size(3,5,7))   == (kwk::of_size(3,5))      );
  TTS_EXPECT_NOT( (kwk::of_size(3,5,7))   == (kwk::of_size(3,5,9))    );
  TTS_EXPECT_NOT( (kwk::of_size(3,5,7))   == (kwk::of_size(3,5,7,9))  );

  TTS_EXPECT_NOT( (kwk::of_size(3,5,7,9)) == (kwk::of_size(3))        );
  TTS_EXPECT_NOT( (kwk::of_size(3,5,7,9)) == (kwk::of_size(3,5))      );
  TTS_EXPECT_NOT( (kwk::of_size(3,5,7,9)) == (kwk::of_size(3,5,7))    );
  TTS_EXPECT_NOT( (kwk::of_size(3,5,7,9)) == (kwk::of_size(3,5,7,2))  );
};

TTS_CASE( "Inequality comparison between shapes" )
{
  TTS_NOT_EQUAL(  kwk::of_size(3)       ,  kwk::of_size(7)        );
  TTS_NOT_EQUAL(  kwk::of_size(3)       , (kwk::of_size(3,2))     );
  TTS_NOT_EQUAL(  kwk::of_size(3)       , (kwk::of_size(3,2,4))   );
  TTS_NOT_EQUAL(  kwk::of_size(3)       , (kwk::of_size(3,3,5,7)) );

  TTS_NOT_EQUAL( (kwk::of_size(3,5))    , (kwk::of_size(7))       );
  TTS_NOT_EQUAL( (kwk::of_size(3,5))    , (kwk::of_size(3,7))     );
  TTS_NOT_EQUAL( (kwk::of_size(3,5))    , (kwk::of_size(3,5,7))   );
  TTS_NOT_EQUAL( (kwk::of_size(3,5))    , (kwk::of_size(3,5,7,9)) );

  TTS_NOT_EQUAL( (kwk::of_size(3,5,7))  , (kwk::of_size(3))       );
  TTS_NOT_EQUAL( (kwk::of_size(3,5,7))  , (kwk::of_size(3,5))     );
  TTS_NOT_EQUAL( (kwk::of_size(3,5,7))  , (kwk::of_size(3,5,9))   );
  TTS_NOT_EQUAL( (kwk::of_size(3,5,7))  , (kwk::of_size(3,5,7,9)) );

  TTS_NOT_EQUAL( (kwk::of_size(3,5,7,9)), (kwk::of_size(3))        );
  TTS_NOT_EQUAL( (kwk::of_size(3,5,7,9)), (kwk::of_size(3,5))      );
  TTS_NOT_EQUAL( (kwk::of_size(3,5,7,9)), (kwk::of_size(3,5,7))    );
  TTS_NOT_EQUAL( (kwk::of_size(3,5,7,9)), (kwk::of_size(3,5,7,2))  );

  TTS_EXPECT_NOT(  kwk::of_size(3)        !=  kwk::of_size(3)        );
  TTS_EXPECT_NOT(  kwk::of_size(3)        != (kwk::of_size(3,1))     );
  TTS_EXPECT_NOT(  kwk::of_size(3)        != (kwk::of_size(3,1,1))   );
  TTS_EXPECT_NOT(  kwk::of_size(3)        != (kwk::of_size(3,1,1,1)) );

  TTS_EXPECT_NOT( (kwk::of_size(3,5))     != (kwk::of_size(3,5))     );
  TTS_EXPECT_NOT( (kwk::of_size(3,5))     != (kwk::of_size(3,5,1))   );
  TTS_EXPECT_NOT( (kwk::of_size(3,5))     != (kwk::of_size(3,5,1,1)) );

  TTS_EXPECT_NOT( (kwk::of_size(3,5,7))   != (kwk::of_size(3,5,7))   );
  TTS_EXPECT_NOT( (kwk::of_size(3,5,7))   != (kwk::of_size(3,5,7,1)) );

  TTS_EXPECT_NOT( (kwk::of_size(3,5,7,9)) != (kwk::of_size(3,5,7,9)) );
};
