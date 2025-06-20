//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#include "test.hpp"
#include <kwk/utility/container/shape.hpp>

TTS_CASE( "Equality comparison between shapes" )
{
  using kwk::height;
  using kwk::width;
  using kwk::depth;
  using kwk::channel;
  using kwk::of_size;

  TTS_EQUAL( of_size(3), of_size(3)         );
  TTS_EQUAL( of_size(3), (of_size(1,3))     );
  TTS_EQUAL( of_size(3), (of_size(1,1,3))   );
  TTS_EQUAL( of_size(3), (of_size(1,1,1,3)) );

  TTS_EQUAL( of_size(height = 3),  of_size(height = 3)        );
  TTS_EQUAL( of_size(height = 3), (of_size(1,height = 3))     );
  TTS_EQUAL( of_size(height = 3), (of_size(1,1,height = 3))   );
  TTS_EQUAL( of_size(height = 3), (of_size(1,1,1,height = 3)) );

  TTS_EQUAL( (of_size(3, 5)), (of_size(15))      );
  TTS_EQUAL( (of_size(3, 5)), (of_size(3,5))     );
  TTS_EQUAL( (of_size(3, 5)), (of_size(1,3,5))   );
  TTS_EQUAL( (of_size(3, 5)), (of_size(1,1,3,5)) );

  TTS_EQUAL( (of_size(width = 15))          , (of_size(height = 3,width = 5))     );
  TTS_EQUAL( (of_size(height = 3,width = 5)), (of_size(width = 15))               );
  TTS_EQUAL( (of_size(height = 3,width = 5)), (of_size(height = 3,width = 5))     );
  TTS_EQUAL( (of_size(height = 3,width = 5)), (of_size(1,height = 3,width = 5))   );
  TTS_EQUAL( (of_size(height = 3,width = 5)), (of_size(1,1,height = 3,width = 5)) );

  TTS_EQUAL( (of_size(3,5,7))  , (of_size(105))     );
  TTS_EQUAL( (of_size(3,5,7))  , (of_size(15,7))    );
  TTS_EQUAL( (of_size(3,5,7))  , (of_size(3,5,7))   );
  TTS_EQUAL( (of_size(3,5,7))  , (of_size(1,3,5,7)) );

  TTS_EQUAL( (of_size(width = 105))                     , (of_size(depth = 3, height = 5, width = 7))   );
  TTS_EQUAL( (of_size(height = 15, width = 7))          , (of_size(depth = 3, height = 5, width = 7))   );
  TTS_EQUAL( (of_size(depth = 3, height = 5, width = 7)), (of_size(width = 105))                        );
  TTS_EQUAL( (of_size(depth = 3, height = 5, width = 7)), (of_size(height = 15, width = 7))             );
  TTS_EQUAL( (of_size(depth = 3, height = 5, width = 7)), (of_size(depth = 3, height = 5, width = 7))   );
  TTS_EQUAL( (of_size(depth = 3, height = 5, width = 7)), (of_size(1,depth = 3, height = 5, width = 7)) );

  TTS_EQUAL( (of_size(3,5,7,9)), (of_size(945))     );
  TTS_EQUAL( (of_size(3,5,7,9)), (of_size(105,9))   );
  TTS_EQUAL( (of_size(3,5,7,9)), (of_size(15,7,9))  );
  TTS_EQUAL( (of_size(3,5,7,9)), (of_size(3,5,7,9)) );

  TTS_EQUAL( (of_size(depth = 15,height =7,width = 9))            , (of_size(channel = 3,depth = 5,height = 7,width = 9)) );
  TTS_EQUAL( (of_size(height =105,width = 9))                     , (of_size(channel = 3,depth = 5,height = 7,width = 9)) );
  TTS_EQUAL( (of_size(width = 945))                               , (of_size(channel = 3,depth = 5,height = 7,width = 9)) );
  TTS_EQUAL( (of_size(channel = 3,depth = 5,height = 7,width = 9)), (of_size(width = 945))     );
  TTS_EQUAL( (of_size(channel = 3,depth = 5,height = 7,width = 9)), (of_size(height =105,width = 9))   );
  TTS_EQUAL( (of_size(channel = 3,depth = 5,height = 7,width = 9)), (of_size(depth = 15,height =7,width = 9))  );
  TTS_EQUAL( (of_size(channel = 3,depth = 5,height = 7,width = 9)), (of_size(channel = 3,depth = 5,height =7,width = 9)) );
};

TTS_CASE( "Inequality comparison between shapes" )
{
  using kwk::height;
  using kwk::width;
  using kwk::depth;
  using kwk::channel;
  using kwk::of_size;

  TTS_NOT_EQUAL( of_size(3),  of_size(7)        );
  TTS_NOT_EQUAL( of_size(3), (of_size(5,3))     );
  TTS_NOT_EQUAL( of_size(3), (of_size(1,2,3))   );
  TTS_NOT_EQUAL( of_size(3), (of_size(1,2,1,9)) );

  TTS_NOT_EQUAL( of_size(height = 3),  of_size(height = 9)        );
  TTS_NOT_EQUAL( of_size(height = 3), (of_size(6,height = 3))     );
  TTS_NOT_EQUAL( of_size(height = 3), (of_size(1,3,height = 3))   );
  TTS_NOT_EQUAL( of_size(height = 3), (of_size(2,9,1,height = 3)) );

  TTS_NOT_EQUAL( (of_size(3, 5)), (of_size(17))      );
  TTS_NOT_EQUAL( (of_size(3, 5)), (of_size(7,5))     );
  TTS_NOT_EQUAL( (of_size(3, 5)), (of_size(1,9,5))   );
  TTS_NOT_EQUAL( (of_size(3, 5)), (of_size(3,1,3,5)) );

  TTS_NOT_EQUAL( (of_size(width = 15))          , (of_size(height = 9,width = 5))     );
  TTS_NOT_EQUAL( (of_size(height = 3,width = 5)), (of_size(width = 5))                );
  TTS_NOT_EQUAL( (of_size(height = 3,width = 5)), (of_size(height = 3,width = 15))    );
  TTS_NOT_EQUAL( (of_size(height = 3,width = 5)), (of_size(1,height = 8,width = 5))   );
  TTS_NOT_EQUAL( (of_size(height = 3,width = 5)), (of_size(1,7,height = 3,width = 5)) );

  TTS_NOT_EQUAL( (of_size(3,5,7))  , (of_size(107))     );
  TTS_NOT_EQUAL( (of_size(3,5,7))  , (of_size(35,7))    );
  TTS_NOT_EQUAL( (of_size(3,5,7))  , (of_size(3,95,7))  );
  TTS_NOT_EQUAL( (of_size(3,5,7))  , (of_size(3,3,5,7)) );

  TTS_NOT_EQUAL( (of_size(width = 15))                      , (of_size(depth = 3, height = 5, width = 7))   );
  TTS_NOT_EQUAL( (of_size(height = 15, width = 77))         , (of_size(depth = 3, height = 5, width = 7))   );
  TTS_NOT_EQUAL( (of_size(depth = 3, height = 5, width = 7)), (of_size(width = 175))                        );
  TTS_NOT_EQUAL( (of_size(depth = 3, height = 5, width = 7)), (of_size(height = 75, width = 7))             );
  TTS_NOT_EQUAL( (of_size(depth = 3, height = 5, width = 7)), (of_size(depth = 3, height = 35, width = 7))  );
  TTS_NOT_EQUAL( (of_size(depth = 3, height = 5, width = 7)), (of_size(3,depth = 33, height = 5, width = 7)));

  TTS_NOT_EQUAL( (of_size(3,5,7,9)), (of_size(975))     );
  TTS_NOT_EQUAL( (of_size(3,5,7,9)), (of_size(105,11))  );
  TTS_NOT_EQUAL( (of_size(3,5,7,9)), (of_size(1,7,9))   );
  TTS_NOT_EQUAL( (of_size(3,5,7,9)), (of_size(3,1,77,9)));

  TTS_NOT_EQUAL( (of_size(depth = 19,height =7,width = 9))            , (of_size(channel = 3,depth = 5,height = 7,width = 9)) );
  TTS_NOT_EQUAL( (of_size(height =115,width = 9))                     , (of_size(channel = 3,depth = 5,height = 7,width = 9)) );
  TTS_NOT_EQUAL( (of_size(width = 995))                               , (of_size(channel = 3,depth = 5,height = 7,width = 9)) );
  TTS_NOT_EQUAL( (of_size(channel = 3,depth = 5,height = 7,width = 9)), (of_size(width = 975))                                );
  TTS_NOT_EQUAL( (of_size(channel = 3,depth = 5,height = 7,width = 9)), (of_size(height =135,width = 9))                      );
  TTS_NOT_EQUAL( (of_size(channel = 3,depth = 5,height = 7,width = 9)), (of_size(depth = 15,height = 8,width = 9))            );
  TTS_NOT_EQUAL( (of_size(channel = 3,depth = 5,height = 7,width = 9)), (of_size(channel = 11,depth = 80,height =7,width = 9)));
};
