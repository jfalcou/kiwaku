//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/detail/kumi.hpp>
#include <kwk/utility/container/shape.hpp>
#include "utility/slicer/slicer.hpp"

TTS_CASE("Check 4D -> 1D slicing")
{
  auto in = kumi::cartesian_product(in1, in2, in3, in4);
  auto sls = kumi::cartesian_product(sl);
  
  test_slice(in, compress_out<1>(in), sls);
};

TTS_CASE("Check 4D -> 2D slicing")
{
  auto in = kumi::cartesian_product(in1, in2, in3, in4);
  auto sls = kumi::cartesian_product(sl, sl);
  
  test_slice(in, compress_out<2>(in), sls);
};

TTS_CASE("Check 4D -> 3D slicing")
{
  auto in = kumi::cartesian_product(in1, in2, in3, in4);
  auto sls = kumi::cartesian_product(sl, sl, sl);
  
  test_slice(in, compress_out<3>(in), sls);
};

TTS_CASE("Check 4D -> 4D slicing")
{
  auto o1 = kumi::cartesian_product ( kumi::tuple{out<0>(in1), out<1>(in1)}
                                    , kumi::tuple{out<0>(in2), out<1>(in2)}
                                    , kumi::tuple{out<0>(in3), out<1>(in3)}
                                    , kumi::tuple{out<0>(in4), out<1>(in4)}
                                    );

  auto outs = kumi::map ([](auto p)
                          {
                            return kumi::cartesian_product( get<0>(p)
                                                          , get<1>(p)
                                                          , get<2>(p)
                                                          , get<3>(p)
                                                          );
                          }, o1
                        );
                        
  auto in = kumi::cartesian_product(in1, in2, in3, in4);
  auto sls = kumi::cartesian_product(sl, sl, sl, sl);
  
  test_slice(in, outs, sls);
};
