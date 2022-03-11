//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#include "test.hpp"
#include <kiwaku/components/shape.hpp>
#include <kiwaku/options/fixed.hpp>

TTS_CASE( "1D mixed shape constructor" )
{
  using namespace kwk::literals;

  auto s1 = kwk::of_size(7);
  TTS_CONSTEXPR_EQUAL(sizeof(s1), sizeof(7) );
  TTS_EQUAL(s1.size()           , 1 );
  TTS_EQUAL(s1.numel()          , 7 );
  TTS_EQUAL(get<0>(s1)          , 7 );

  auto s2 = kwk::of_size<std::int16_t>(33);
  TTS_CONSTEXPR_EQUAL(sizeof(s2), sizeof(std::int16_t) );
  TTS_EQUAL(s2.size()           , 1  );
  TTS_EQUAL(s2.numel()          , 33 );
  TTS_EQUAL(get<0>(s2)          , 33 );

  auto s2d = kwk::of_size(std::int16_t{33});
  TTS_CONSTEXPR_EQUAL(sizeof(s2d), sizeof(std::int16_t) );
  TTS_EQUAL(s2d.size()           , 1  );
  TTS_EQUAL(s2d.numel()          , 33 );
  TTS_EQUAL(get<0>(s2d)          , 33 );

  auto s3 = kwk::of_size(kwk::fixed<9>);
  TTS_CONSTEXPR_EQUAL(sizeof(s3), 1ULL);
  TTS_EQUAL(s3.size()           , 1   );
  TTS_EQUAL(s3.numel()          , 9   );
  TTS_EQUAL(get<0>(s3)          , 9   );

  auto s3d = kwk::of_size(kwk::fixed<std::int16_t{9}>);
  TTS_CONSTEXPR_EQUAL(sizeof(s3d), 1ULL);
  TTS_EQUAL(s3d.size()           , 1   );
  TTS_EQUAL(s3d.numel()          , 9   );
  TTS_EQUAL(get<0>(s3d)          , 9   );

  auto s4 = kwk::of_size<std::uint8_t>(11_c);
  TTS_CONSTEXPR_EQUAL(sizeof(s4), 1ULL);
  TTS_EQUAL(s4.size()           , 1   );
  TTS_EQUAL(s4.numel()          , 11  );
  TTS_EQUAL(get<0>(s4)          , 11  );
};

TTS_CASE( "2D partial shape constructor" )
{
  using namespace kwk::literals;

  auto s1 = kwk::of_size(7,9);

  TTS_CONSTEXPR_EQUAL(sizeof(s1), 2*sizeof(7) );
  TTS_EQUAL(s1.size()           , 2   );
  TTS_EQUAL(s1.numel()          , 63  );
  TTS_EQUAL(get<0>(s1)          , 7   );
  TTS_EQUAL(get<1>(s1)          , 9   );

  auto s2 = kwk::of_size<std::int16_t>(9,3);

  TTS_CONSTEXPR_EQUAL(sizeof(s2), 2*sizeof(std::int16_t) );
  TTS_EQUAL(s2.size()           , 2   );
  TTS_EQUAL(s2.numel()          , 27  );
  TTS_EQUAL(get<0>(s2)          , 9   );
  TTS_EQUAL(get<1>(s2)          , 3   );

  auto s3 = kwk::of_size(5_c, 8);

  TTS_CONSTEXPR_EQUAL(sizeof(s3), sizeof(std::ptrdiff_t) );
  TTS_EQUAL(s3.size()           , 2   );
  TTS_EQUAL(s3.numel()          , 40  );
  TTS_EQUAL(get<0>(s3)          , 5   );
  TTS_EQUAL(get<1>(s3)          , 8   );

  auto s4 = kwk::of_size<std::int16_t>(5, 9_c);

  TTS_CONSTEXPR_EQUAL(sizeof(s4), sizeof(std::int16_t) );
  TTS_EQUAL(s4.size()           , 2   );
  TTS_EQUAL(s4.numel()          , 45  );
  TTS_EQUAL(get<0>(s4)          , 5   );
  TTS_EQUAL(get<1>(s4)          , 9   );

  auto s5 = kwk::of_size(7_c, 9_c);

  TTS_CONSTEXPR_EQUAL(sizeof(s5), 1ULL);
  TTS_EQUAL(s5.size()           , 2   );
  TTS_EQUAL(s5.numel()          , 63  );
  TTS_EQUAL(get<0>(s5)          , 7   );
  TTS_EQUAL(get<1>(s5)          , 9   );
};

TTS_CASE( "3D partial shape constructor" )
{
  using namespace kwk::literals;

  auto s1 = kwk::of_size(7,9,11);

  TTS_CONSTEXPR_EQUAL(sizeof(s1), 3*sizeof(7) );
  TTS_EQUAL(s1.size()           , 3   );
  TTS_EQUAL(s1.numel()          , 693 );
  TTS_EQUAL(get<0>(s1)          , 7   );
  TTS_EQUAL(get<1>(s1)          , 9   );
  TTS_EQUAL(get<2>(s1)          , 11  );

  auto s2 = kwk::of_size<std::int16_t>(9,3,7);

  TTS_CONSTEXPR_EQUAL(sizeof(s2), 3*sizeof(std::int16_t) );
  TTS_EQUAL(s2.size()           , 3   );
  TTS_EQUAL(s2.numel()          , 189 );
  TTS_EQUAL(get<0>(s2)          , 9   );
  TTS_EQUAL(get<1>(s2)          , 3   );
  TTS_EQUAL(get<2>(s2)          , 7   );

  auto s3 = kwk::of_size(7_c,9,11);

  TTS_CONSTEXPR_EQUAL(sizeof(s3), 2*sizeof(std::ptrdiff_t) );
  TTS_EQUAL(s3.size()           , 3   );
  TTS_EQUAL(s3.numel()          , 693 );
  TTS_EQUAL(get<0>(s3)          , 7   );
  TTS_EQUAL(get<1>(s3)          , 9   );
  TTS_EQUAL(get<2>(s3)          , 11  );

  auto s4 = kwk::of_size(7,9_c,11);

  TTS_CONSTEXPR_EQUAL(sizeof(s4), 2*sizeof(std::ptrdiff_t) );
  TTS_EQUAL(s4.size()           , 3   );
  TTS_EQUAL(s4.numel()          , 693 );
  TTS_EQUAL(get<0>(s4)          , 7   );
  TTS_EQUAL(get<1>(s4)          , 9   );
  TTS_EQUAL(get<2>(s4)          , 11  );

  auto s5 = kwk::of_size(7,9,11_c);

  TTS_CONSTEXPR_EQUAL(sizeof(s5), 2*sizeof(std::ptrdiff_t) );
  TTS_EQUAL(s5.size()           , 3   );
  TTS_EQUAL(s5.numel()          , 693 );
  TTS_EQUAL(get<0>(s5)          , 7   );
  TTS_EQUAL(get<1>(s5)          , 9   );
  TTS_EQUAL(get<2>(s5)          , 11  );

  auto s6 = kwk::of_size(7_c,9_c,11);

  TTS_CONSTEXPR_EQUAL(sizeof(s6), sizeof(std::ptrdiff_t) );
  TTS_EQUAL(s6.size()           , 3   );
  TTS_EQUAL(s6.numel()          , 693 );
  TTS_EQUAL(get<0>(s6)          , 7   );
  TTS_EQUAL(get<1>(s6)          , 9   );
  TTS_EQUAL(get<2>(s6)          , 11  );

  auto s7 = kwk::of_size(7,9_c,11_c);

  TTS_CONSTEXPR_EQUAL(sizeof(s7), sizeof(std::ptrdiff_t) );
  TTS_EQUAL(s7.size()           , 3   );
  TTS_EQUAL(s7.numel()          , 693 );
  TTS_EQUAL(get<0>(s7)          , 7   );
  TTS_EQUAL(get<1>(s7)          , 9   );
  TTS_EQUAL(get<2>(s7)          , 11  );

  auto s8 = kwk::of_size<std::int16_t>(7_c,9,11_c);

  TTS_CONSTEXPR_EQUAL(sizeof(s8), sizeof(std::int16_t) );
  TTS_EQUAL(s8.size()           , 3   );
  TTS_EQUAL(s8.numel()          , 693 );
  TTS_EQUAL(get<0>(s8)          , 7   );
  TTS_EQUAL(get<1>(s8)          , 9   );
  TTS_EQUAL(get<2>(s8)          , 11  );

  auto s9 = kwk::of_size(7_c,9_c,11_c);

  TTS_CONSTEXPR_EQUAL(sizeof(s9), 1ULL );
  TTS_EQUAL(s9.size()           , 3   );
  TTS_EQUAL(s9.numel()          , 693 );
  TTS_EQUAL(get<0>(s9)          , 7   );
  TTS_EQUAL(get<1>(s9)          , 9   );
  TTS_EQUAL(get<2>(s9)          , 11  );
};