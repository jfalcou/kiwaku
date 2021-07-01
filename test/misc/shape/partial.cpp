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

TTS_CASE( "1D partial shape constructor" )
{
  using kwk::_;
  kwk::shape<kwk::_1D> sh( _[0] = 7 );

  TTS_EQUAL(sh.size()       , 1 );
  TTS_EQUAL(sh.numel()       , 7 );
  TTS_EQUAL(kwk::get<0>(sh) , 7 );
};

TTS_CASE( "2D partial shape constructor" )
{
  using kwk::_;
  kwk::shape<kwk::_2D> sh_dd( _[1] = 7, _[0] =3 );

  TTS_EQUAL(sh_dd.size()       , 2  );
  TTS_EQUAL(sh_dd.numel()      , 21 );
  TTS_EQUAL(kwk::get<0>(sh_dd) , 3  );
  TTS_EQUAL(kwk::get<1>(sh_dd) , 7  );

  kwk::shape<kwk::extent()[5]> sh_ds( _[0] =3 );

  TTS_EQUAL(sh_ds.size()       , 2  );
  TTS_EQUAL(sh_ds.numel()      , 15 );
  TTS_EQUAL(kwk::get<0>(sh_ds) , 3  );
  TTS_EQUAL(kwk::get<1>(sh_ds) , 5  );

  kwk::shape<kwk::extent[5]()> sh_sd( _[1] =3 );

  TTS_EQUAL(sh_sd.size()       , 2  );
  TTS_EQUAL(sh_sd.numel()      , 15 );
  TTS_EQUAL(kwk::get<0>(sh_sd) , 5  );
  TTS_EQUAL(kwk::get<1>(sh_sd) , 3  );
};

TTS_CASE( "3D partial shape constructor" )
{
  using kwk::_;
  kwk::shape<kwk::_3D> sh_ddd( _[1] = 7, _[0] = 3, _[2] = 4 );

  TTS_EQUAL(sh_ddd.size()       , 3  );
  TTS_EQUAL(sh_ddd.numel()      , 84 );
  TTS_EQUAL(kwk::get<0>(sh_ddd) , 3  );
  TTS_EQUAL(kwk::get<1>(sh_ddd) , 7  );
  TTS_EQUAL(kwk::get<2>(sh_ddd) , 4  );

  kwk::shape<kwk::extent()()[5]> sh_dds( _[0] = 3, _[1] = 6 );

  TTS_EQUAL(sh_dds.size()       , 3  );
  TTS_EQUAL(sh_dds.numel()      , 90 );
  TTS_EQUAL(kwk::get<0>(sh_dds) , 3  );
  TTS_EQUAL(kwk::get<1>(sh_dds) , 6  );
  TTS_EQUAL(kwk::get<2>(sh_dds) , 5  );

  kwk::shape<kwk::extent()[5]()> sh_dsd( _[0] = 3, _[2] = 4 );

  TTS_EQUAL(sh_dsd.size()       , 3  );
  TTS_EQUAL(sh_dsd.numel()      , 60 );
  TTS_EQUAL(kwk::get<0>(sh_dsd) , 3  );
  TTS_EQUAL(kwk::get<1>(sh_dsd) , 5  );
  TTS_EQUAL(kwk::get<2>(sh_dsd) , 4  );

  kwk::shape<kwk::extent[5]()()> sh_sdd( _[1] = 3, _[2] = 4 );

  TTS_EQUAL(sh_sdd.size()       , 3  );
  TTS_EQUAL(sh_sdd.numel()      , 60 );
  TTS_EQUAL(kwk::get<0>(sh_sdd) , 5  );
  TTS_EQUAL(kwk::get<1>(sh_sdd) , 3  );
  TTS_EQUAL(kwk::get<2>(sh_sdd) , 4  );

  kwk::shape<kwk::extent()[5][4]> sh_dss( _[0] = 3 );

  TTS_EQUAL(sh_dss.size()       , 3  );
  TTS_EQUAL(sh_dss.numel()      , 60 );
  TTS_EQUAL(kwk::get<0>(sh_dss) , 3  );
  TTS_EQUAL(kwk::get<1>(sh_dss) , 5  );
  TTS_EQUAL(kwk::get<2>(sh_dss) , 4  );

  kwk::shape<kwk::extent[5]()[4]> sh_sds( _[1] = 3 );

  TTS_EQUAL(sh_sds.size()       , 3  );
  TTS_EQUAL(sh_sds.numel()      , 60 );
  TTS_EQUAL(kwk::get<0>(sh_sds) , 5  );
  TTS_EQUAL(kwk::get<1>(sh_sds) , 3  );
  TTS_EQUAL(kwk::get<2>(sh_sds) , 4  );

  kwk::shape<kwk::extent[5][4]()> sh_ssd( _[2] = 3 );

  TTS_EQUAL(sh_ssd.size()       , 3  );
  TTS_EQUAL(sh_ssd.numel()      , 60 );
  TTS_EQUAL(kwk::get<0>(sh_ssd) , 5  );
  TTS_EQUAL(kwk::get<1>(sh_ssd) , 4  );
  TTS_EQUAL(kwk::get<2>(sh_ssd) , 3  );
};
