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

  TTS_EQUAL(sizeof(shape_d)  , 8UL );
  TTS_EQUAL(sizeof(shape_s)  , 1UL );

  TTS_EQUAL(shape_d.nbdims() , 1   );
  TTS_EQUAL(shape_s.nbdims() , 1   );

  TTS_EQUAL(shape_d.numel()  , 0L  );
  TTS_EQUAL(shape_s.numel()  , 7L  );

  TTS_EQUAL(shape_d.order()  , 1L  );
  TTS_EQUAL(shape_s.order()  , 1L  );

  TTS_EQUAL(get<0>(shape_d)  , 0L  );
  TTS_EQUAL(get<0>(shape_s)  , 7UL );
};

TTS_CASE( "Default constructed shape behavior - Mixed 2D")
{
  kwk::shape<kwk::extent( )( )> shape_dd;
  kwk::shape<kwk::extent( )[7]> shape_ds;
  kwk::shape<kwk::extent[7]( )> shape_sd;
  kwk::shape<kwk::extent[7][5]> shape_ss;

  TTS_EQUAL(sizeof(shape_dd), 16UL);
  TTS_EQUAL(sizeof(shape_ds),  8UL);
  TTS_EQUAL(sizeof(shape_sd),  8UL);
  TTS_EQUAL(sizeof(shape_ss),  1UL);

  TTS_EQUAL(shape_dd.nbdims() , 1L);
  TTS_EQUAL(shape_ds.nbdims() , 2L);
  TTS_EQUAL(shape_sd.nbdims() , 1L);
  TTS_EQUAL(shape_ss.nbdims() , 2L);

  TTS_EQUAL(shape_dd.order(), 2L);
  TTS_EQUAL(shape_ds.order(), 2L);
  TTS_EQUAL(shape_sd.order(), 2L);
  TTS_EQUAL(shape_ss.order(), 2L);

  TTS_EQUAL(get<0>(shape_dd), 0L  );
  TTS_EQUAL(get<0>(shape_ds), 0L  );
  TTS_EQUAL(get<0>(shape_sd), 7UL );
  TTS_EQUAL(get<0>(shape_ss), 7UL );

  TTS_EQUAL(get<1>(shape_dd), 1L  );
  TTS_EQUAL(get<1>(shape_ds), 7UL );
  TTS_EQUAL(get<1>(shape_sd), 1L  );
  TTS_EQUAL(get<1>(shape_ss), 5UL );
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

  TTS_EQUAL(sizeof(shape_ddd) , 24UL);
  TTS_EQUAL(sizeof(shape_dds) , 16UL);
  TTS_EQUAL(sizeof(shape_dsd) , 16UL);
  TTS_EQUAL(sizeof(shape_dss) ,  8UL);
  TTS_EQUAL(sizeof(shape_sdd) , 16UL);
  TTS_EQUAL(sizeof(shape_sds) ,  8UL);
  TTS_EQUAL(sizeof(shape_ssd) ,  8UL);
  TTS_EQUAL(sizeof(shape_sss) ,  1UL);

  TTS_EQUAL(shape_ddd.nbdims(), 1L);
  TTS_EQUAL(shape_dds.nbdims(), 3L);
  TTS_EQUAL(shape_dsd.nbdims(), 2L);
  TTS_EQUAL(shape_dss.nbdims(), 3L);
  TTS_EQUAL(shape_sdd.nbdims(), 1L);
  TTS_EQUAL(shape_sds.nbdims(), 3L);
  TTS_EQUAL(shape_ssd.nbdims(), 2L);
  TTS_EQUAL(shape_sss.nbdims(), 3L);

  TTS_EQUAL(shape_ddd.order(), 3L);
  TTS_EQUAL(shape_dds.order(), 3L);
  TTS_EQUAL(shape_dsd.order(), 3L);
  TTS_EQUAL(shape_dss.order(), 3L);
  TTS_EQUAL(shape_sdd.order(), 3L);
  TTS_EQUAL(shape_sds.order(), 3L);
  TTS_EQUAL(shape_ssd.order(), 3L);
  TTS_EQUAL(shape_sss.order(), 3L);

  TTS_EQUAL(get<0>(shape_ddd), 0L );
  TTS_EQUAL(get<0>(shape_dds), 0L );
  TTS_EQUAL(get<0>(shape_dsd), 0L );
  TTS_EQUAL(get<0>(shape_dss), 0L );
  TTS_EQUAL(get<0>(shape_sdd), 7UL);
  TTS_EQUAL(get<0>(shape_sds), 7UL);
  TTS_EQUAL(get<0>(shape_ssd), 7UL);
  TTS_EQUAL(get<0>(shape_sss), 7UL);

  TTS_EQUAL(get<1>(shape_ddd), 1L );
  TTS_EQUAL(get<1>(shape_dds), 1L );
  TTS_EQUAL(get<1>(shape_dsd), 7UL);
  TTS_EQUAL(get<1>(shape_dss), 7UL);
  TTS_EQUAL(get<1>(shape_sdd), 1L );
  TTS_EQUAL(get<1>(shape_sds), 1L );
  TTS_EQUAL(get<1>(shape_ssd), 5UL);
  TTS_EQUAL(get<1>(shape_sss), 5UL);

  TTS_EQUAL(get<2>(shape_ddd), 1L );
  TTS_EQUAL(get<2>(shape_dds), 7UL);
  TTS_EQUAL(get<2>(shape_dsd), 1L );
  TTS_EQUAL(get<2>(shape_dss), 5UL);
  TTS_EQUAL(get<2>(shape_sdd), 1L );
  TTS_EQUAL(get<2>(shape_sds), 5UL);
  TTS_EQUAL(get<2>(shape_ssd), 1L );
  TTS_EQUAL(get<2>(shape_sss), 3UL);
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

  TTS_EQUAL(sizeof(shape_dddd) , 32UL);
  TTS_EQUAL(sizeof(shape_ddds) , 24UL);
  TTS_EQUAL(sizeof(shape_ddsd) , 24UL);
  TTS_EQUAL(sizeof(shape_ddss) , 16UL);
  TTS_EQUAL(sizeof(shape_dsdd) , 24UL);
  TTS_EQUAL(sizeof(shape_dsds) , 16UL);
  TTS_EQUAL(sizeof(shape_dssd) , 16UL);
  TTS_EQUAL(sizeof(shape_dsss) ,  8UL);
  TTS_EQUAL(sizeof(shape_sddd) , 24UL);
  TTS_EQUAL(sizeof(shape_sdds) , 16UL);
  TTS_EQUAL(sizeof(shape_sdsd) , 16UL);
  TTS_EQUAL(sizeof(shape_sdss) ,  8UL);
  TTS_EQUAL(sizeof(shape_ssdd) , 16UL);
  TTS_EQUAL(sizeof(shape_ssds) ,  8UL);
  TTS_EQUAL(sizeof(shape_sssd) ,  8UL);
  TTS_EQUAL(sizeof(shape_ssss) ,  1UL);

  TTS_EQUAL(shape_dddd.nbdims(), 1L);
  TTS_EQUAL(shape_ddds.nbdims(), 4L);
  TTS_EQUAL(shape_ddsd.nbdims(), 3L);
  TTS_EQUAL(shape_ddss.nbdims(), 4L);
  TTS_EQUAL(shape_dsdd.nbdims(), 2L);
  TTS_EQUAL(shape_dsds.nbdims(), 4L);
  TTS_EQUAL(shape_dssd.nbdims(), 3L);
  TTS_EQUAL(shape_dsss.nbdims(), 4L);
  TTS_EQUAL(shape_sddd.nbdims(), 1L);
  TTS_EQUAL(shape_sdds.nbdims(), 4L);
  TTS_EQUAL(shape_sdsd.nbdims(), 3L);
  TTS_EQUAL(shape_sdss.nbdims(), 4L);
  TTS_EQUAL(shape_ssdd.nbdims(), 2L);
  TTS_EQUAL(shape_ssds.nbdims(), 4L);
  TTS_EQUAL(shape_sssd.nbdims(), 3L);
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

  TTS_EQUAL(get<0>(shape_dddd), 0L  );
  TTS_EQUAL(get<0>(shape_ddds), 0L  );
  TTS_EQUAL(get<0>(shape_ddsd), 0L  );
  TTS_EQUAL(get<0>(shape_ddss), 0L  );
  TTS_EQUAL(get<0>(shape_dsdd), 0L  );
  TTS_EQUAL(get<0>(shape_dsds), 0L  );
  TTS_EQUAL(get<0>(shape_dssd), 0L  );
  TTS_EQUAL(get<0>(shape_dsss), 0L  );
  TTS_EQUAL(get<0>(shape_sddd), 9UL );
  TTS_EQUAL(get<0>(shape_sdds), 9UL );
  TTS_EQUAL(get<0>(shape_sdsd), 9UL );
  TTS_EQUAL(get<0>(shape_sdss), 9UL );
  TTS_EQUAL(get<0>(shape_ssdd), 9UL );
  TTS_EQUAL(get<0>(shape_ssds), 9UL );
  TTS_EQUAL(get<0>(shape_sssd), 9UL );
  TTS_EQUAL(get<0>(shape_ssss), 9UL );

  TTS_EQUAL(get<1>(shape_dddd), 1L  );
  TTS_EQUAL(get<1>(shape_ddds), 1L  );
  TTS_EQUAL(get<1>(shape_ddsd), 1L  );
  TTS_EQUAL(get<1>(shape_ddss), 1L  );
  TTS_EQUAL(get<1>(shape_dsdd), 7UL );
  TTS_EQUAL(get<1>(shape_dsds), 7UL );
  TTS_EQUAL(get<1>(shape_dssd), 7UL );
  TTS_EQUAL(get<1>(shape_dsss), 7UL );
  TTS_EQUAL(get<1>(shape_sddd), 1L  );
  TTS_EQUAL(get<1>(shape_sdds), 1L  );
  TTS_EQUAL(get<1>(shape_sdsd), 1L  );
  TTS_EQUAL(get<1>(shape_sdss), 1L  );
  TTS_EQUAL(get<1>(shape_ssdd), 7UL );
  TTS_EQUAL(get<1>(shape_ssds), 7UL );
  TTS_EQUAL(get<1>(shape_sssd), 7UL );
  TTS_EQUAL(get<1>(shape_ssss), 7UL );

  TTS_EQUAL(get<2>(shape_dddd), 1L  );
  TTS_EQUAL(get<2>(shape_ddds), 1L  );
  TTS_EQUAL(get<2>(shape_ddsd), 7UL );
  TTS_EQUAL(get<2>(shape_ddss), 7UL );
  TTS_EQUAL(get<2>(shape_dsdd), 1L  );
  TTS_EQUAL(get<2>(shape_dsds), 1L  );
  TTS_EQUAL(get<2>(shape_dssd), 5UL );
  TTS_EQUAL(get<2>(shape_dsss), 5UL );
  TTS_EQUAL(get<2>(shape_sddd), 1L  );
  TTS_EQUAL(get<2>(shape_sdds), 1L  );
  TTS_EQUAL(get<2>(shape_sdsd), 7UL );
  TTS_EQUAL(get<2>(shape_sdss), 7UL );
  TTS_EQUAL(get<2>(shape_ssdd), 1L  );
  TTS_EQUAL(get<2>(shape_ssds), 1L  );
  TTS_EQUAL(get<2>(shape_sssd), 5UL );
  TTS_EQUAL(get<2>(shape_ssss), 5UL );

  TTS_EQUAL(get<3>(shape_dddd), 1L  );
  TTS_EQUAL(get<3>(shape_ddds), 7UL );
  TTS_EQUAL(get<3>(shape_ddsd), 1L  );
  TTS_EQUAL(get<3>(shape_ddss), 5UL );
  TTS_EQUAL(get<3>(shape_dsdd), 1L  );
  TTS_EQUAL(get<3>(shape_dsds), 5UL );
  TTS_EQUAL(get<3>(shape_dssd), 1L  );
  TTS_EQUAL(get<3>(shape_dsss), 3UL );
  TTS_EQUAL(get<2>(shape_ssss), 5UL );
  TTS_EQUAL(get<3>(shape_sddd), 1L  );
  TTS_EQUAL(get<3>(shape_sdds), 7UL );
  TTS_EQUAL(get<3>(shape_sdsd), 1L  );
  TTS_EQUAL(get<3>(shape_sdss), 5UL );
  TTS_EQUAL(get<3>(shape_ssdd), 1L  );
  TTS_EQUAL(get<3>(shape_ssds), 5UL );
  TTS_EQUAL(get<3>(shape_sssd), 1L  );
  TTS_EQUAL(get<3>(shape_ssss), 3UL );
};

TTS_CASE_TPL( "Default constructed shape behavior - Dynamic 5D -> 10D", sizes<5>)
<typename T>(::tts::type<T>)
{
  constexpr auto dims = 5+T::value;
  kwk::shape<kwk::_nD<dims>> sh;

  TTS_EQUAL(sizeof(sh)  , dims*sizeof(std::ptrdiff_t) );
  TTS_EQUAL(sh.nbdims() , 1   );
  TTS_EQUAL(sh.numel()  , 0   );
  TTS_EQUAL(sh.order()  , dims);

  TTS_EQUAL(get<0>(sh)  , 0);
  for(int i = 1;i<sh.order();++i)
    TTS_EQUAL(sh[i] , 1);
};

