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

using kwk::_;

using axis_base_type = decltype(kwk::along<0>)::base_type;

TTS_CASE( "Default constructed shape behavior - Mixed 1D")
{
  kwk::shape<_> shape_d;
  kwk::shape<7> shape_s;

  TTS_EQUAL(sizeof(shape_d)  ,1*sizeof(axis_base_type));
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
  kwk::shape<_,_> shape_dd;
  kwk::shape<_,7> shape_ds;
  kwk::shape<7,_> shape_sd;
  kwk::shape<7,5> shape_ss;

  TTS_EQUAL(sizeof(shape_dd), 2*sizeof(axis_base_type));
  TTS_EQUAL(sizeof(shape_ds), 1*sizeof(axis_base_type));
  TTS_EQUAL(sizeof(shape_sd), 1*sizeof(axis_base_type));
  TTS_EQUAL(sizeof(shape_ss), 1UL);

  TTS_EQUAL(shape_dd.nbdims() , 2L);
  TTS_EQUAL(shape_ds.nbdims() , 2L);
  TTS_EQUAL(shape_sd.nbdims() , 2L);
  TTS_EQUAL(shape_ss.nbdims() , 2L);

  TTS_EQUAL(shape_dd.order(), 2L);
  TTS_EQUAL(shape_ds.order(), 2L);
  TTS_EQUAL(shape_sd.order(), 2L);
  TTS_EQUAL(shape_ss.order(), 2L);

  TTS_EQUAL( shape_dd, kwk::of_size(0,0) );
  TTS_EQUAL( shape_ds, kwk::of_size(0,7) );
  TTS_EQUAL( shape_sd, kwk::of_size(7,0) );
  TTS_EQUAL( shape_ss, kwk::of_size(7,5) );
};

TTS_CASE( "Default constructed shape behavior - Mixed 3D")
{
  kwk::shape<_,_,_> shape_ddd;
  kwk::shape<_,_,7> shape_dds;
  kwk::shape<_,7,_> shape_dsd;
  kwk::shape<_,7,5> shape_dss;
  kwk::shape<7,_,_> shape_sdd;
  kwk::shape<7,_,5> shape_sds;
  kwk::shape<7,5,_> shape_ssd;
  kwk::shape<7,5,3> shape_sss;

  TTS_EQUAL(sizeof(shape_ddd) , 3*sizeof(axis_base_type));
  TTS_EQUAL(sizeof(shape_dds) , 2*sizeof(axis_base_type));
  TTS_EQUAL(sizeof(shape_dsd) , 2*sizeof(axis_base_type));
  TTS_EQUAL(sizeof(shape_dss) , 1*sizeof(axis_base_type));
  TTS_EQUAL(sizeof(shape_sdd) , 2*sizeof(axis_base_type));
  TTS_EQUAL(sizeof(shape_sds) , 1*sizeof(axis_base_type));
  TTS_EQUAL(sizeof(shape_ssd) , 1*sizeof(axis_base_type));
  TTS_EQUAL(sizeof(shape_sss) , 1UL);

  TTS_EQUAL(shape_ddd.nbdims(), 3L);
  TTS_EQUAL(shape_dds.nbdims(), 3L);
  TTS_EQUAL(shape_dsd.nbdims(), 3L);
  TTS_EQUAL(shape_dss.nbdims(), 3L);
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

  TTS_EQUAL(shape_ddd, kwk::of_size(0,0,0));
  TTS_EQUAL(shape_dds, kwk::of_size(0,0,7));
  TTS_EQUAL(shape_dsd, kwk::of_size(0,7,0));
  TTS_EQUAL(shape_dss, kwk::of_size(0,7,5));
  TTS_EQUAL(shape_sdd, kwk::of_size(7,0,0));
  TTS_EQUAL(shape_sds, kwk::of_size(7,0,5));
  TTS_EQUAL(shape_ssd, kwk::of_size(7,5,0));
  TTS_EQUAL(shape_sss, kwk::of_size(7,5,3));
};

TTS_CASE( "Default constructed shape behavior - Mixed 4D")
{
  kwk::shape<_,_,_,_> shape_dddd;
  kwk::shape<_,_,_,7> shape_ddds;
  kwk::shape<_,_,7,_> shape_ddsd;
  kwk::shape<_,_,7,5> shape_ddss;
  kwk::shape<_,7,_,_> shape_dsdd;
  kwk::shape<_,7,_,5> shape_dsds;
  kwk::shape<_,7,5,_> shape_dssd;
  kwk::shape<_,7,5,3> shape_dsss;
  kwk::shape<9,_,_,_> shape_sddd;
  kwk::shape<9,_,_,7> shape_sdds;
  kwk::shape<9,_,7,_> shape_sdsd;
  kwk::shape<9,_,7,5> shape_sdss;
  kwk::shape<9,7,_,_> shape_ssdd;
  kwk::shape<9,7,_,5> shape_ssds;
  kwk::shape<9,7,5,_> shape_sssd;
  kwk::shape<9,7,5,3> shape_ssss;

  TTS_EQUAL(sizeof(shape_dddd) , 4*sizeof(axis_base_type) );
  TTS_EQUAL(sizeof(shape_ddds) , 3*sizeof(axis_base_type) );
  TTS_EQUAL(sizeof(shape_ddsd) , 3*sizeof(axis_base_type) );
  TTS_EQUAL(sizeof(shape_ddss) , 2*sizeof(axis_base_type) );
  TTS_EQUAL(sizeof(shape_dsdd) , 3*sizeof(axis_base_type) );
  TTS_EQUAL(sizeof(shape_dsds) , 2*sizeof(axis_base_type) );
  TTS_EQUAL(sizeof(shape_dssd) , 2*sizeof(axis_base_type) );
  TTS_EQUAL(sizeof(shape_dsss) , 1*sizeof(axis_base_type) );
  TTS_EQUAL(sizeof(shape_sddd) , 3*sizeof(axis_base_type) );
  TTS_EQUAL(sizeof(shape_sdds) , 2*sizeof(axis_base_type) );
  TTS_EQUAL(sizeof(shape_sdsd) , 2*sizeof(axis_base_type) );
  TTS_EQUAL(sizeof(shape_sdss) , 1*sizeof(axis_base_type) );
  TTS_EQUAL(sizeof(shape_ssdd) , 2*sizeof(axis_base_type) );
  TTS_EQUAL(sizeof(shape_ssds) , 1*sizeof(axis_base_type) );
  TTS_EQUAL(sizeof(shape_sssd) , 1*sizeof(axis_base_type) );
  TTS_EQUAL(sizeof(shape_ssss) , 1UL );

  TTS_EQUAL(shape_dddd.nbdims(), 4L);
  TTS_EQUAL(shape_ddds.nbdims(), 4L);
  TTS_EQUAL(shape_ddsd.nbdims(), 4L);
  TTS_EQUAL(shape_ddss.nbdims(), 4L);
  TTS_EQUAL(shape_dsdd.nbdims(), 4L);
  TTS_EQUAL(shape_dsds.nbdims(), 4L);
  TTS_EQUAL(shape_dssd.nbdims(), 4L);
  TTS_EQUAL(shape_dsss.nbdims(), 4L);
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

  TTS_EQUAL(shape_dddd, kwk::of_size(0,0,0,0));
  TTS_EQUAL(shape_ddds, kwk::of_size(0,0,0,7));
  TTS_EQUAL(shape_ddsd, kwk::of_size(0,0,7,0));
  TTS_EQUAL(shape_ddss, kwk::of_size(0,0,7,5));
  TTS_EQUAL(shape_dsdd, kwk::of_size(0,7,0,0));
  TTS_EQUAL(shape_dsds, kwk::of_size(0,7,0,5));
  TTS_EQUAL(shape_dssd, kwk::of_size(0,7,5,0));
  TTS_EQUAL(shape_dsss, kwk::of_size(0,7,5,3));
  TTS_EQUAL(shape_sddd, kwk::of_size(9,0,0,0));
  TTS_EQUAL(shape_sdds, kwk::of_size(9,0,0,7));
  TTS_EQUAL(shape_sdsd, kwk::of_size(9,0,7,0));
  TTS_EQUAL(shape_sdss, kwk::of_size(9,0,7,5));
  TTS_EQUAL(shape_ssdd, kwk::of_size(9,7,0,0));
  TTS_EQUAL(shape_ssds, kwk::of_size(9,7,0,5));
  TTS_EQUAL(shape_sssd, kwk::of_size(9,7,5,0));
  TTS_EQUAL(shape_ssss, kwk::of_size(9,7,5,3));
};

// TTS_CASE_TPL( "Default constructed shape behavior - Dynamic 5D -> 10D", sizes<5>)
// <typename T>(::tts::type<T>)
// {
//   constexpr auto dims = 5+T::value;
//   kwk::shape<_nD<dims>> sh;

//   TTS_EQUAL(sizeof(sh)  , dims*sizeof(std::int32_t) );
//   TTS_EQUAL(sh.nbdims() , dims);
//   TTS_EQUAL(sh.numel()  , 0   );
//   TTS_EQUAL(sh.order()  , dims);

//   for(std::ptrdiff_t i = 0;i<sh.order();++i)
//     TTS_EQUAL(sh[i] , 0);
// };
