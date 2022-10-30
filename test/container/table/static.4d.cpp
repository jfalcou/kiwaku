//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/container.hpp>
#include <algorithm>
#include <array>
#include <vector>

TTS_CASE( "Build a 4D table from a C array" )
{
  using namespace kwk::literals;

  float ref[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12
                ,13,14,15,16,17,18,19,20,21,22,23,24
                };

  auto v = kwk::table{ kwk::source = ref
                    , kwk::of_size(2_c, 3_c, 2_c, 2_c)
                    };

  TTS_EQUAL ( v.size() , 24                       );
  TTS_EQUAL ( sizeof(v), sizeof(ref[0]) * v.size());
  TTS_EQUAL ( v.shape(), kwk::of_size(2,3,2,2)    );
  TTS_EXPECT( v.shape().is_fully_static           );

  TTS_NOT_EQUAL(&v(0,0,0,0),&ref[0]);
  kwk::for_each_index([&](auto e, auto i0, auto i1, auto i2, auto i3)
                      {
                        TTS_EQUAL(e, ref[i0+2*(i1+3*(i2+2*i3))]);
                      }
                      , v
                      );

  auto w = kwk::table{ kwk::source = ref
                    , kwk::of_size(3_c, 2_c, 2_c, 2_c)
                    };

  TTS_EQUAL ( w.size() , 24                       );
  TTS_EQUAL ( sizeof(w), sizeof(ref[0]) * w.size());
  TTS_EQUAL ( w.shape(), kwk::of_size(3,2,2,2)    );
  TTS_EXPECT( w.shape().is_fully_static           );

  TTS_NOT_EQUAL(&w(0,0,0,0),&ref[0]);
  kwk::for_each_index([&](auto e, auto i0, auto i1, auto i2, auto i3)
                      {
                        TTS_EQUAL(e, ref[i0+3*(i1+2*(i2+2*i3))]);
                      }
                      , w
                      );
};

TTS_CASE( "Build a 4D table from std::array" )
{
  using namespace kwk::literals;

  std::array<float,24> ref  = { 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12
                              ,13,14,15,16,17,18,19,20,21,22,23,24
                              };


  auto v = kwk::table{ kwk::source = ref
                    , kwk::of_size(2_c, 3_c, 2_c, 2_c)
                    };

  TTS_EQUAL ( v.size() , 24                     );
  TTS_EQUAL ( sizeof(v), sizeof(ref[0])*v.size());
  TTS_EQUAL ( v.shape(), kwk::of_size(2,3,2,2)  );
  TTS_EXPECT( v.shape().is_fully_static         );

  TTS_NOT_EQUAL(&v(0,0,0,0),&ref[0]);
  kwk::for_each_index([&](auto e, auto i0, auto i1, auto i2, auto i3)
                      {
                        TTS_EQUAL(e, ref[i0+2*(i1+3*(i2+2*i3))]);
                      }
                      , v
                      );

  auto w = kwk::table{ kwk::source = ref
                    , kwk::of_size(3_c, 2_c, 2_c, 2_c)
                    };

  TTS_EQUAL ( w.size() , 24                     );
  TTS_EQUAL ( sizeof(w), sizeof(ref[0])*w.size());
  TTS_EQUAL ( w.shape(), kwk::of_size(3,2,2,2)  );
  TTS_EXPECT( w.shape().is_fully_static         );

  TTS_NOT_EQUAL(&w(0,0,0,0),&ref[0]);
  kwk::for_each_index([&](auto e, auto i0, auto i1, auto i2, auto i3)
                      {
                        TTS_EQUAL(e, ref[i0+3*(i1+2*(i2+2*i3))]);
                      }
                      , w
                      );
};

TTS_CASE( "Build a 4D table with constexpr shape settings from a ContiguousRange" )
{
  using namespace kwk::literals;

  std::vector<float> ref  = { 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12
                            ,13,14,15,16,17,18,19,20,21,22,23,24
                            };

  auto v = kwk::table{ kwk::source = ref
                    , kwk::of_size(2_c, 3_c, 2_c, 2_c)
                    };

  TTS_EQUAL ( v.size() , 24                     );
  TTS_EQUAL ( sizeof(v), sizeof(ref[0])*v.size());
  TTS_EQUAL ( v.shape(), kwk::of_size(2,3,2,2)  );
  TTS_EXPECT( v.shape().is_fully_static         );

  TTS_NOT_EQUAL(&v(0,0,0,0),&ref[0]);
  kwk::for_each_index([&](auto e, auto i0, auto i1, auto i2, auto i3)
                      {
                        TTS_EQUAL(e, ref[i0+2*(i1+3*(i2+2*i3))]);
                      }
                      , v
                      );

  auto w = kwk::table { kwk::source = ref
                      , kwk::of_size(3_c, 2_c, 2_c, 2_c)
                      };

  TTS_EQUAL ( w.size() , 24                     );
  TTS_EQUAL ( sizeof(w), sizeof(ref[0])*w.size());
  TTS_EQUAL ( w.shape(), kwk::of_size(3,2,2,2)  );
  TTS_EXPECT( w.shape().is_fully_static         );

  TTS_NOT_EQUAL(&w(0,0,0,0),&ref[0]);
  kwk::for_each_index([&](auto e, auto i0, auto i1, auto i2, auto i3)
                      {
                        TTS_EQUAL(e, ref[i0+3*(i1+2*(i2+2*i3))]);
                      }
                      , w
                      );
};

TTS_CASE( "Build a 4D table with constexpr shape settings from a pointer" )
{
  using namespace kwk::literals;

  std::vector<float> ref  = { 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12
                            ,13,14,15,16,17,18,19,20,21,22,23,24
                            };

  auto v = kwk::table { kwk::source = ref
                      , kwk::of_size(2_c, 3_c, 2_c, 2_c)
                      };

  TTS_EQUAL ( v.size() , 24                     );
  TTS_EQUAL ( sizeof(v), sizeof(ref[0])*v.size());
  TTS_EQUAL ( v.shape(), kwk::of_size(2,3,2,2)  );
  TTS_EXPECT( v.shape().is_fully_static         );

  TTS_NOT_EQUAL(&v(0,0,0,0),&ref[0]);
  kwk::for_each_index([&](auto e, auto i0, auto i1, auto i2, auto i3)
                      {
                        TTS_EQUAL(e, ref[i0+2*(i1+3*(i2+2*i3))]);
                      }
                      , v
                      );

  auto w = kwk::table { kwk::source = ref
                      , kwk::of_size(3_c, 2_c, 2_c, 2_c)
                      };

  TTS_EQUAL ( w.size() , 24                     );
  TTS_EQUAL ( sizeof(w), sizeof(ref[0])*w.size());
  TTS_EQUAL ( w.shape(), kwk::of_size(3,2,2,2)  );
  TTS_EXPECT( w.shape().is_fully_static         );

  TTS_NOT_EQUAL(&w(0,0,0,0),&ref[0]);
  kwk::for_each_index([&](auto e, auto i0, auto i1, auto i2, auto i3)
                      {
                        TTS_EQUAL(e, ref[i0+3*(i1+2*(i2+2*i3))]);
                      }
                      , w
                      );
};
