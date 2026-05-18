//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#include "test.hpp"
#include <kwk/utility/container/shape.hpp>
#include <kwk/utility/fixed.hpp>

TTS_CASE( "1D mixed shape constructor" )
{
  using namespace kwk;

  shape<width> x0;
  TTS_EQUAL(x0.order()          , 1   );
  TTS_EQUAL(x0[width]           , 0   );

  shape<width> x1( width = 77);
  TTS_EQUAL(x1.order()          , 1    );
  TTS_EQUAL(x1[width]           , 77   );

  shape x1b( width = 99);
  TTS_EQUAL(x1b.order()          , 1    );
  TTS_EQUAL(x1b[width]           , 99   );

  shape<width[55]> x3;
  TTS_EQUAL(x3.order()          , 1   );
  TTS_EQUAL(x3[width]           , 55  );

  shape<width[55]> x3b(55);
  TTS_EQUAL(x3b.order()          , 1  );
  TTS_EQUAL(x3b[width]           , 55 );

  shape<width[55]> x3c(width = 55);
  TTS_EQUAL(x3c.order()          , 1  );
  TTS_EQUAL(x3c[width]           , 55 );

  shape<width[55]> x3d(width = _);
  TTS_EQUAL(x3d.order()          , 1  );
  TTS_EQUAL(x3d[width]           , 55 );
};

TTS_CASE( "2D mixed shape constructor" )
{
  using namespace kwk;
  int n = 99;

  {
    shape<width, height> s0;
    TTS_EQUAL(s0[width] , 0);
    TTS_EQUAL(s0[height], 0);

    shape s1(height = n, width = 77);
    TTS_EQUAL(s1[width] , 77);
    TTS_EQUAL(s1[height],  n);

    shape<width, height> s2(height = n, width = 77);
    TTS_EQUAL(s2[width] , 77);
    TTS_EQUAL(s2[height],  n);

    shape<width, height>s3(width = 77, height = n);
    TTS_EQUAL(s3[width] , 77);
    TTS_EQUAL(s3[height],  n);

    shape<width[55], height> s4(height = n);
    TTS_EQUAL(s4[width] , 55);
    TTS_EQUAL(s4[height],  n);

    shape<width[55], height> s5(height = n, width = 55);
    TTS_EQUAL(s5[width] , 55);
    TTS_EQUAL(s5[height],  n);

    shape<width[55], height> s6(height = n, width = _);
    TTS_EQUAL(s6[width] , 55);
    TTS_EQUAL(s6[height],  n);

    shape<width, height> s7(width = n);
    TTS_EQUAL(s7[width] , n);
    TTS_EQUAL(s7[height], 0);
  }

  {
    shape<width, height> s0(n,77);
    TTS_EQUAL(s0[width] , n);
    TTS_EQUAL(s0[height], 77);

    shape<width, height> s1(77, n);
    TTS_EQUAL(s1[width] , 77);
    TTS_EQUAL(s1[height],  n);

    shape<width[55], height> s2(_,n);
    TTS_EQUAL(s2[width] , 55);
    TTS_EQUAL(s2[height],  n);

    shape<width, height[88]>s3(n,_);
    TTS_EQUAL(s3[width] ,  n);
    TTS_EQUAL(s3[height], 88);

    shape<width[33], height[88]> s4(_,_);
    TTS_EQUAL(s4[width] , 33);
    TTS_EQUAL(s4[height], 88);

    shape<width[55], height> s5(55,n);
    TTS_EQUAL(s5[width] , 55);
    TTS_EQUAL(s5[height],  n);

    shape<width[33], height[88]> s6;
    TTS_EQUAL(s6[width] , 33);
    TTS_EQUAL(s6[height], 88);
  }

  {
    shape<_,_> s0(n, 77);
    TTS_EQUAL(get<0>(s0),  n);
    TTS_EQUAL(get<1>(s0), 77);

    shape s1(n, 77);
    TTS_EQUAL(get<0>(s1),   n);
    TTS_EQUAL(get<1>(s1),  77);

    shape<55, _> s2(_,n);
    TTS_EQUAL(get<0>(s2), 55);
    TTS_EQUAL(get<1>(s2),  n);

    shape<55, _> s2b(55,n);
    TTS_EQUAL(get<0>(s2b), 55);
    TTS_EQUAL(get<1>(s2b),  n);

    shape<_,33> s3(n,_);
    TTS_EQUAL(get<0>(s3),  n);
    TTS_EQUAL(get<1>(s3), 33);

    shape<_,33> s3b(n,33);
    TTS_EQUAL(get<0>(s3b),  n);
    TTS_EQUAL(get<1>(s3b), 33);

    shape<44,33> s4(_,_);
    TTS_EQUAL(get<0>(s4), 44);
    TTS_EQUAL(get<1>(s4), 33);

    shape<44,33> s4b(_,33);
    TTS_EQUAL(get<0>(s4b), 44);
    TTS_EQUAL(get<1>(s4b), 33);

    shape<44,33> s4c(44,_);
    TTS_EQUAL(get<0>(s4c), 44);
    TTS_EQUAL(get<1>(s4c), 33);

    shape<44,33> s4d(44,33);
    TTS_EQUAL(get<0>(s4d), 44);
    TTS_EQUAL(get<1>(s4d), 33);

    shape<44,33> s4e;
    TTS_EQUAL(get<0>(s4e), 44);
    TTS_EQUAL(get<1>(s4e), 33);
  }
};
