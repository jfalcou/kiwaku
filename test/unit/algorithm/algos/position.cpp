//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#include <kwk/utility/position.hpp>
#include "test.hpp"
#include <kumi/tuple.hpp>

// for_each 1D - 4D

TTS_CASE("Check for kwk::position(std::array)")
{
  kwk::position<3> p1{{0, 1, 2}};

  TTS_EQUAL(p1[0], 0);
  TTS_EQUAL(p1[1], 1);
  TTS_EQUAL(p1[2], 2);

  kwk::position<3> p2;
  p2 = {0, 1, 2};

  TTS_EQUAL(p2[0], 0);
  TTS_EQUAL(p2[1], 1);
  TTS_EQUAL(p2[2], 2);

  // TTS_EQUAL(p1, p2);
  TTS_EQUAL(p1 == p2, true);

  std::stringstream ss;
  ss << p1;
  TTS_EQUAL(ss.str(), std::string{"position<3>(0, 1, 2)"});
};


// === Kumi interface ===
// Every test fails or raise a compile-time error
// TTS_CASE("Check adapted types model kumi::product_type concept")
// {
//   TTS_EXPECT    ( kumi::product_type<kwk::position<8>>      );
//   TTS_EXPECT    ( (kumi::product_type<std::array<int,4>>   ));
//   TTS_EXPECT_NOT( (kumi::product_type<std::pair<int,float>>));
// };

// TTS_CASE("Check adapted type behave like a product_type")
// {
//   auto r = kumi::apply( [](auto... x) { return (x + ...); }, kwk::position<4>{{2, 4, 7, 12}});
//   TTS_EQUAL(r, 25);

//   auto zz = kumi::zip( kwk::position<4>{{11, 12, 13, 14}}, kwk::position<4>{{21, 22, 23, 24}});
//   TTS_EQUAL ( zz, ( kumi::tuple { kumi::tuple{11, 21}
//                                 , kumi::tuple{12, 22}
//                                 , kumi::tuple{13, 23}
//                                 , kumi::tuple{14, 24}
//                                 })
//             );

//   auto cc = kumi::cat( kwk::position<2>{{11, 12}}, kwk::position<2>{{13, 14}});
//   TTS_EQUAL ( cc, ( kumi::tuple {11, 12, 13, 14}) );
// };


// =======

// Generic Kumi tests, unrelated with kwk::position (these tests are passing)
// TTS_CASE("Check adapted external type behave like a product_type")
// {
//   auto r = kumi::apply( [](auto... x) { return (x + ...); }, std::array{1,2,5,4});
//   TTS_EQUAL(r, 12);

//   auto zz = kumi::zip( std::array{1,2,5}, std::array{0.1,2.3,4.5});
//   TTS_EQUAL ( zz, ( kumi::tuple { kumi::tuple{1 , 0.1 }
//                                 , kumi::tuple{2 , 2.3 }
//                                 , kumi::tuple{5 , 4.5 }
//                                 })
//             );

//   auto cc = kumi::cat( std::array{1,2,5}, std::array{0.1,2.3,4.5} );
//   TTS_EQUAL ( cc, ( kumi::tuple {1, 2, 5, 0.1, 2.3, 4.5}) );
// };


