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

TTS_CASE("Check 2D -> 1D slicing")
{
  auto in = kumi::cartesian_product(in1, in2);
  auto sls = kumi::cartesian_product(sl);
  
  test_slice(in, compress_out<1>(in), sls);
};


TTS_CASE("Check 2D -> 2D slicing")
{
  auto o1 = kumi::cartesian_product ( kumi::tuple{out<0>(in1), out<1>(in1)}
                                    , kumi::tuple{out<0>(in2), out<1>(in2)}
                                    );

  auto outs = kumi::map ([](auto p)
                          {
                            return kumi::cartesian_product(get<0>(p), get<1>(p));
                          }, o1
                        );
  auto in = kumi::cartesian_product(in1, in2);
  auto sls = kumi::cartesian_product(sl, sl);
  
  test_slice(in, outs, sls);
};