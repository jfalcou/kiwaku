//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#include "kwk/detail/kumi.hpp"
#include "test.hpp"
#include <kwk/container/shape.hpp>
#include <kwk/options/fixed.hpp>

TTS_CASE( "1D shape constructor with joker indexes" )
{
  using namespace kwk::literals;

  kwk::shape<kwk::_1D> d1(kwk::_);
  TTS_EQUAL( get<0>(d1), 0);

  kwk::shape<kwk::_1D> d2(3);
  TTS_EQUAL( get<0>(d2), 3);

  kwk::shape<kwk::_1D> d3(3_c);
  TTS_EQUAL( get<0>(d3), 3);

  kwk::shape<kwk::extent[3]> s1(kwk::_);
  TTS_EQUAL( get<0>(s1), 3);

  kwk::shape<kwk::extent[3]> s2(3);
  TTS_EQUAL( get<0>(s2), 3);

  kwk::shape<kwk::extent[3]> s3(3_c);
  TTS_EQUAL( get<0>(s3), 3);
};

// ------------------- Generalized cartesian product for testing
auto generate_dataset(auto d0, auto d1)
{
  return kumi::flatten(kumi::apply( [&](auto... m)
                                      {
                                        constexpr auto n = d0.size();
                                        return kumi::tuple{ kumi::zip ( kumi::generate<n>(m)
                                                                      , d1
                                                                      )...
                                                          };
                                      }
                                      , d0
                                      )
                      );
}

auto generate_dataset(auto d0, auto... ds)
{
  auto i = generate_dataset(ds...);
  auto o = kumi::flatten(kumi::apply( [&](auto... m)
                                            {
                                              constexpr auto n = i.size();
                                              return kumi::tuple{ kumi::zip( kumi::generate<n>(m)
                                                                          , i
                                                                          )...
                                                                };
                                            }
                                        , d0
                                        )
                        );

  return kumi::apply( [](auto... m) { return kumi::tuple{kumi::flatten(m)...}; }
                    , o
                    );
}

void test_sizes(auto computed, auto expected)
{
  kumi::for_each( [](auto e)
                    {
                      auto comp = kwk::of_size(get<0>(e));
                      auto ref  = kwk::of_size(get<1>(e));
                      TTS_EQUAL(comp, ref);
                    }
                  , kumi::zip(computed,expected)
                  );
}

// ------------------- Data to reuse
using namespace kwk::literals;

// size elements
inline auto d0 = kumi::tuple{kwk::_,3,42_c};
inline auto d1 = kumi::tuple{kwk::_,7,61_c};
inline auto d2 = kumi::tuple{kwk::_,5,93_c};
inline auto d3 = kumi::tuple{kwk::_,9,33_c};

// expected dimensions
inline auto v0 = kumi::tuple{0,3,42};
inline auto v1 = kumi::tuple{1,7,61};
inline auto v2 = kumi::tuple{1,5,93};
inline auto v3 = kumi::tuple{1,9,33};

TTS_CASE( "2D shape constructor with joker indexes" )
{
  test_sizes( generate_dataset(d0,d1), generate_dataset(v0,v1));
};

TTS_CASE( "3D shape constructor with joker indexes" )
{
  test_sizes( generate_dataset(d0,d1,d2), generate_dataset(v0,v1, v2));
};

TTS_CASE( "4D shape constructor with joker indexes" )
{
  test_sizes( generate_dataset(d0,d1,d2,d3), generate_dataset(v0,v1,v2,v3));
};
