//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#include "test.hpp"
#include <kwk/utility/container/shape.hpp>
#include "utility/shape/shape.hpp"

TTS_CASE( "Default constructed shape behavior - Mixed 1D")
{
  kwk::shape<kwk::extent()>   shape_d;
  kwk::shape<kwk::extent[7]>  shape_s;

  TTS_EQUAL(sizeof(shape_d)  ,1*sizeof(int) );
  TTS_EQUAL(sizeof(shape_s)  , 1UL );

  TTS_EQUAL(shape_d.nbdims() , 1   );
  TTS_EQUAL(shape_s.nbdims() , 1   );

  TTS_EQUAL(shape_d.numel()  , 0L  );
  TTS_EQUAL(shape_s.numel()  , 7L  );

  TTS_EQUAL(shape_d.order()  , 1L  );
  TTS_EQUAL(shape_s.order()  , 1L  );

  TTS_EQUAL(shape_d, kwk::of_size(0)  );
  TTS_EQUAL(shape_s, kwk::of_size(7)  );
};

TTS_CASE( "Default constructed shape behavior - Mixed 2D")
{
  kwk::shape<kwk::extent( )( )> shape_dd;
  kwk::shape<kwk::extent( )[7]> shape_ds;
  kwk::shape<kwk::extent[7]( )> shape_sd;
  kwk::shape<kwk::extent[7][5]> shape_ss;

  TTS_EQUAL(sizeof(shape_dd), 2*sizeof(int));
  TTS_EQUAL(sizeof(shape_ds), 1*sizeof(int));
  TTS_EQUAL(sizeof(shape_sd), 1*sizeof(int));
  TTS_EQUAL(sizeof(shape_ss),  1UL);

  TTS_EQUAL(shape_dd.nbdims() , 1L);
  TTS_EQUAL(shape_ds.nbdims() , 1L);
  TTS_EQUAL(shape_sd.nbdims() , 2L);
  TTS_EQUAL(shape_ss.nbdims() , 2L);

  TTS_EQUAL(shape_dd.order(), 2L);
  TTS_EQUAL(shape_ds.order(), 2L);
  TTS_EQUAL(shape_sd.order(), 2L);
  TTS_EQUAL(shape_ss.order(), 2L);

  TTS_EQUAL( shape_dd, kwk::of_size(1,0) );
  TTS_EQUAL( shape_ds, kwk::of_size(1,7) );
  TTS_EQUAL( shape_sd, kwk::of_size(7,0) );
  TTS_EQUAL( shape_ss, kwk::of_size(7,5) );
};

TTS_CASE( "Default constructed shape behavior - Mixed 3D")
{
  kwk::shape<kwk::extent( )( )( )> shape_ddd;
  kwk::shape<kwk::extent( )( )[7]> shape_dds;
  kwk::shape<kwk::extent( )[7]( )> shape_dsd;
  kwk::shape<kwk::extent( )[7][5]> shape_dss;
  kwk::shape<kwk::extent[7]( )( )> shape_sdd;
  kwk::shape<kwk::extent[7]( )[5]> shape_sds;
  kwk::shape<kwk::extent[7][5]( )> shape_ssd;
  kwk::shape<kwk::extent[7][5][3]> shape_sss;

  TTS_EQUAL(sizeof(shape_ddd) , 3*sizeof(int));
  TTS_EQUAL(sizeof(shape_dds) , 2*sizeof(int));
  TTS_EQUAL(sizeof(shape_dsd) , 2*sizeof(int));
  TTS_EQUAL(sizeof(shape_dss) , 1*sizeof(int));
  TTS_EQUAL(sizeof(shape_sdd) , 2*sizeof(int));
  TTS_EQUAL(sizeof(shape_sds) , 1*sizeof(int));
  TTS_EQUAL(sizeof(shape_ssd) , 1*sizeof(int));
  TTS_EQUAL(sizeof(shape_sss) ,  1UL);

  TTS_EQUAL(shape_ddd.nbdims(), 1L);
  TTS_EQUAL(shape_dds.nbdims(), 1L);
  TTS_EQUAL(shape_dsd.nbdims(), 2L);
  TTS_EQUAL(shape_dss.nbdims(), 2L);
  TTS_EQUAL(shape_sdd.nbdims(), 3L);
  TTS_EQUAL(shape_sds.nbdims(), 3L);
  TTS_EQUAL(shape_ssd.nbdims(), 3L);
  TTS_EQUAL(shape_sss.nbdims(), 3L);

  TTS_EQUAL(shape_ddd.order(), 3L);
  TTS_EQUAL(shape_dds.order(), 3L);
  TTS_EQUAL(shape_dsd.order(), 3L);
  TTS_EQUAL(shape_dss.order(), 3L);
  TTS_EQUAL(shape_sdd.order(), 3L);
  TTS_EQUAL(shape_sds.order(), 3L);
  TTS_EQUAL(shape_ssd.order(), 3L);
  TTS_EQUAL(shape_sss.order(), 3L);

  TTS_EQUAL(shape_ddd, kwk::of_size(1,1,0));
  TTS_EQUAL(shape_dds, kwk::of_size(1,1,7));
  TTS_EQUAL(shape_dsd, kwk::of_size(1,7,0));
  TTS_EQUAL(shape_dss, kwk::of_size(1,7,5));
  TTS_EQUAL(shape_sdd, kwk::of_size(7,1,0));
  TTS_EQUAL(shape_sds, kwk::of_size(7,1,5));
  TTS_EQUAL(shape_ssd, kwk::of_size(7,5,0));
  TTS_EQUAL(shape_sss, kwk::of_size(7,5,3));
};

TTS_CASE( "Default constructed shape behavior - Mixed 4D")
{
  kwk::shape<kwk::extent( )( )( )( )> shape_dddd;
  kwk::shape<kwk::extent( )( )( )[7]> shape_ddds;
  kwk::shape<kwk::extent( )( )[7]( )> shape_ddsd;
  kwk::shape<kwk::extent( )( )[7][5]> shape_ddss;
  kwk::shape<kwk::extent( )[7]( )( )> shape_dsdd;
  kwk::shape<kwk::extent( )[7]( )[5]> shape_dsds;
  kwk::shape<kwk::extent( )[7][5]( )> shape_dssd;
  kwk::shape<kwk::extent( )[7][5][3]> shape_dsss;
  kwk::shape<kwk::extent[9]( )( )( )> shape_sddd;
  kwk::shape<kwk::extent[9]( )( )[7]> shape_sdds;
  kwk::shape<kwk::extent[9]( )[7]( )> shape_sdsd;
  kwk::shape<kwk::extent[9]( )[7][5]> shape_sdss;
  kwk::shape<kwk::extent[9][7]( )( )> shape_ssdd;
  kwk::shape<kwk::extent[9][7]( )[5]> shape_ssds;
  kwk::shape<kwk::extent[9][7][5]( )> shape_sssd;
  kwk::shape<kwk::extent[9][7][5][3]> shape_ssss;

  TTS_EQUAL(sizeof(shape_dddd) , 4*sizeof(int) );
  TTS_EQUAL(sizeof(shape_ddds) , 3*sizeof(int) );
  TTS_EQUAL(sizeof(shape_ddsd) , 3*sizeof(int) );
  TTS_EQUAL(sizeof(shape_ddss) , 2*sizeof(int) );
  TTS_EQUAL(sizeof(shape_dsdd) , 3*sizeof(int) );
  TTS_EQUAL(sizeof(shape_dsds) , 2*sizeof(int) );
  TTS_EQUAL(sizeof(shape_dssd) , 2*sizeof(int) );
  TTS_EQUAL(sizeof(shape_dsss) , 1*sizeof(int) );
  TTS_EQUAL(sizeof(shape_sddd) , 3*sizeof(int) );
  TTS_EQUAL(sizeof(shape_sdds) , 2*sizeof(int) );
  TTS_EQUAL(sizeof(shape_sdsd) , 2*sizeof(int) );
  TTS_EQUAL(sizeof(shape_sdss) , 1*sizeof(int) );
  TTS_EQUAL(sizeof(shape_ssdd) , 2*sizeof(int) );
  TTS_EQUAL(sizeof(shape_ssds) , 1*sizeof(int) );
  TTS_EQUAL(sizeof(shape_sssd) , 1*sizeof(int) );
  TTS_EQUAL(sizeof(shape_ssss) ,  1UL );

  TTS_EQUAL(shape_dddd.nbdims(), 1L);
  TTS_EQUAL(shape_ddds.nbdims(), 1L);
  TTS_EQUAL(shape_ddsd.nbdims(), 2L);
  TTS_EQUAL(shape_ddss.nbdims(), 2L);
  TTS_EQUAL(shape_dsdd.nbdims(), 3L);
  TTS_EQUAL(shape_dsds.nbdims(), 3L);
  TTS_EQUAL(shape_dssd.nbdims(), 3L);
  TTS_EQUAL(shape_dsss.nbdims(), 3L);
  TTS_EQUAL(shape_sddd.nbdims(), 4L);
  TTS_EQUAL(shape_sdds.nbdims(), 4L);
  TTS_EQUAL(shape_sdsd.nbdims(), 4L);
  TTS_EQUAL(shape_sdss.nbdims(), 4L);
  TTS_EQUAL(shape_ssdd.nbdims(), 4L);
  TTS_EQUAL(shape_ssds.nbdims(), 4L);
  TTS_EQUAL(shape_sssd.nbdims(), 4L);
  TTS_EQUAL(shape_ssss.nbdims(), 4L);

  TTS_EQUAL(shape_dddd.order(), 4L);
  TTS_EQUAL(shape_ddds.order(), 4L);
  TTS_EQUAL(shape_ddsd.order(), 4L);
  TTS_EQUAL(shape_ddss.order(), 4L);
  TTS_EQUAL(shape_dsdd.order(), 4L);
  TTS_EQUAL(shape_dsds.order(), 4L);
  TTS_EQUAL(shape_dssd.order(), 4L);
  TTS_EQUAL(shape_dsss.order(), 4L);
  TTS_EQUAL(shape_sddd.order(), 4L);
  TTS_EQUAL(shape_sdds.order(), 4L);
  TTS_EQUAL(shape_sdsd.order(), 4L);
  TTS_EQUAL(shape_sdss.order(), 4L);
  TTS_EQUAL(shape_ssdd.order(), 4L);
  TTS_EQUAL(shape_ssds.order(), 4L);
  TTS_EQUAL(shape_sssd.order(), 4L);
  TTS_EQUAL(shape_ssss.order(), 4L);

  TTS_EQUAL(shape_dddd, kwk::of_size(1,1,1,0));
  TTS_EQUAL(shape_ddds, kwk::of_size(1,1,1,7));
  TTS_EQUAL(shape_ddsd, kwk::of_size(1,1,7,0));
  TTS_EQUAL(shape_ddss, kwk::of_size(1,1,7,5));
  TTS_EQUAL(shape_dsdd, kwk::of_size(1,7,1,0));
  TTS_EQUAL(shape_dsds, kwk::of_size(1,7,1,5));
  TTS_EQUAL(shape_dssd, kwk::of_size(1,7,5,0));
  TTS_EQUAL(shape_dsss, kwk::of_size(1,7,5,3));
  TTS_EQUAL(shape_sddd, kwk::of_size(9,1,1,0));
  TTS_EQUAL(shape_sdds, kwk::of_size(9,1,1,7));
  TTS_EQUAL(shape_sdsd, kwk::of_size(9,1,7,0));
  TTS_EQUAL(shape_sdss, kwk::of_size(9,1,7,5));
  TTS_EQUAL(shape_ssdd, kwk::of_size(9,7,1,0));
  TTS_EQUAL(shape_ssds, kwk::of_size(9,7,1,5));
  TTS_EQUAL(shape_sssd, kwk::of_size(9,7,5,0));
  TTS_EQUAL(shape_ssss, kwk::of_size(9,7,5,3));
};

TTS_CASE_TPL( "Default constructed shape behavior - Dynamic 5D -> 10D", sizes<5>)
<typename T>(::tts::type<T>)
{
  constexpr auto dims = 5+T::value;
  kwk::shape<kwk::_nD<dims>> sh;

  TTS_EQUAL(sizeof(sh)  , dims*sizeof(int) );
  TTS_EQUAL(sh.nbdims() , 1   );
  TTS_EQUAL(sh.numel()  , 0   );
  TTS_EQUAL(sh.order()  , dims);

  for(std::ptrdiff_t i = 0;i<sh.order()-1;++i)
    TTS_EQUAL(sh[i] , 1);
  TTS_EQUAL(get<sh.order()-1>(sh)  , 0);
};
