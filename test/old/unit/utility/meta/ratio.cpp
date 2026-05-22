//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/utility/ratio.hpp>
#include <kwk/concepts/values.hpp>


TTS_CASE( "Check ratio addition" )
{
  using kwk::ratio;
  using namespace kwk::literals;

  ratio rdd{1    , 3};
  ratio rsd{5_c  , 7};
  ratio rds{9    , 11_c};
  ratio rss{13_c , 15_c};

  TTS_EQUAL(rdd + rdd, (ratio{2, 3}));
  TTS_CONSTEXPR_EXPECT_NOT( kwk::concepts::static_constant<decltype((rdd+rdd).num)> );
  TTS_CONSTEXPR_EXPECT_NOT( kwk::concepts::static_constant<decltype((rdd+rdd).denum)> );

  TTS_EQUAL(rdd + rsd, (ratio{7*1+5*3, 3*7}));
  TTS_CONSTEXPR_EXPECT_NOT( kwk::concepts::static_constant<decltype((rdd+rsd).num)> );
  TTS_CONSTEXPR_EXPECT_NOT( kwk::concepts::static_constant<decltype((rdd+rsd).denum)> );

  TTS_EQUAL(rsd + rds, (ratio{5*11+9*7, 7*11}));
  TTS_CONSTEXPR_EXPECT_NOT( kwk::concepts::static_constant<decltype((rsd + rds).num)> );
  TTS_CONSTEXPR_EXPECT_NOT( kwk::concepts::static_constant<decltype((rsd + rds).denum)> );

  TTS_EQUAL(rds + rss, (ratio{9*15+13*11, 11*15}));
  TTS_CONSTEXPR_EXPECT_NOT( kwk::concepts::static_constant<decltype((rds + rss).num)> );
  TTS_CONSTEXPR_EXPECT    ( kwk::concepts::static_constant<decltype((rds + rss).denum)> );

  TTS_EQUAL(rss + rss, (ratio{13*15+13*15, 15*15}));
  TTS_CONSTEXPR_EXPECT    ( kwk::concepts::static_constant<decltype((rss + rss).num)> );
  TTS_CONSTEXPR_EXPECT    ( kwk::concepts::static_constant<decltype((rss + rss).denum)> );

};

TTS_CASE( "Check ratio subtraction" )
{
  using kwk::ratio;
  using namespace kwk::literals;

  ratio rdd{1    , 3};
  ratio rsd{5_c  , 7};
  ratio rds{9    , 11_c};
  ratio rss{13_c , 15_c};

  TTS_EQUAL(rdd - rdd, (ratio{0, 3}));
  TTS_CONSTEXPR_EXPECT_NOT( kwk::concepts::static_constant<decltype((rdd-rdd).num)> );
  TTS_CONSTEXPR_EXPECT_NOT( kwk::concepts::static_constant<decltype((rdd-rdd).denum)> );

  TTS_EQUAL(rdd - rsd, (ratio{7*1-5*3, 3*7}));
  TTS_CONSTEXPR_EXPECT_NOT( kwk::concepts::static_constant<decltype((rdd-rsd).num)> );
  TTS_CONSTEXPR_EXPECT_NOT( kwk::concepts::static_constant<decltype((rdd-rsd).denum)> );

  TTS_EQUAL(rsd - rds, (ratio{5*11-9*7, 7*11}));
  TTS_CONSTEXPR_EXPECT_NOT( kwk::concepts::static_constant<decltype((rsd - rds).num)> );
  TTS_CONSTEXPR_EXPECT_NOT( kwk::concepts::static_constant<decltype((rsd - rds).denum)> );

  TTS_EQUAL(rds - rss, (ratio{9*15-13*11, 11*15}));
  TTS_CONSTEXPR_EXPECT_NOT( kwk::concepts::static_constant<decltype((rds - rss).num)> );
  TTS_CONSTEXPR_EXPECT    ( kwk::concepts::static_constant<decltype((rds - rss).denum)> );

  TTS_EQUAL(rss - rss, (ratio{13*15-13*15, 15*15}));
  TTS_CONSTEXPR_EXPECT    ( kwk::concepts::static_constant<decltype((rss - rss).num)> );
  TTS_CONSTEXPR_EXPECT    ( kwk::concepts::static_constant<decltype((rss - rss).denum)> );
};

TTS_CASE( "Check ratio product" )
{
  using kwk::ratio;
  using namespace kwk::literals;

  ratio rdd{1    , 3};
  ratio rsd{5_c  , 7};
  ratio rds{9    , 11_c};
  ratio rss{13_c , 15_c};

  TTS_EQUAL(rdd * rdd, (ratio{1, 9}));
  TTS_CONSTEXPR_EXPECT_NOT( kwk::concepts::static_constant<decltype((rdd*rdd).num)> );
  TTS_CONSTEXPR_EXPECT_NOT( kwk::concepts::static_constant<decltype((rdd*rdd).denum)> );

  TTS_EQUAL(rdd * rsd, (ratio{1*5, 3*7}));
  TTS_CONSTEXPR_EXPECT_NOT( kwk::concepts::static_constant<decltype((rdd*rsd).num)> );
  TTS_CONSTEXPR_EXPECT_NOT( kwk::concepts::static_constant<decltype((rdd*rsd).denum)> );

  TTS_EQUAL(rsd * rds, (ratio{5*9, 7*11}));
  TTS_CONSTEXPR_EXPECT_NOT( kwk::concepts::static_constant<decltype((rsd * rds).num)> );
  TTS_CONSTEXPR_EXPECT_NOT( kwk::concepts::static_constant<decltype((rsd * rds).denum)> );

  TTS_EQUAL(rds * rss, (ratio{9*13, 11*15}));
  TTS_CONSTEXPR_EXPECT_NOT( kwk::concepts::static_constant<decltype((rds * rss).num)> );
  TTS_CONSTEXPR_EXPECT    ( kwk::concepts::static_constant<decltype((rds * rss).denum)> );

  TTS_EQUAL(rss * rss, (ratio{13*13, 15*15}));
  TTS_CONSTEXPR_EXPECT    ( kwk::concepts::static_constant<decltype((rss * rss).num)> );
  TTS_CONSTEXPR_EXPECT    ( kwk::concepts::static_constant<decltype((rss * rss).denum)> );
};

TTS_CASE( "Check ratio division" )
{
  using kwk::ratio;
  using namespace kwk::literals;

  ratio rdd{1    , 3};
  ratio rsd{5_c  , 7};
  ratio rds{9    , 11_c};
  ratio rss{13_c , 15_c};

  TTS_EQUAL(rdd / rdd, (ratio{1, 1}));
  TTS_CONSTEXPR_EXPECT_NOT( kwk::concepts::static_constant<decltype((rdd/rdd).num)> );
  TTS_CONSTEXPR_EXPECT_NOT( kwk::concepts::static_constant<decltype((rdd/rdd).denum)> );

  TTS_EQUAL(rdd / rsd, (ratio{1*7, 5*3}));
  TTS_CONSTEXPR_EXPECT_NOT( kwk::concepts::static_constant<decltype((rdd/rsd).num)> );
  TTS_CONSTEXPR_EXPECT_NOT( kwk::concepts::static_constant<decltype((rdd/rsd).denum)> );

  TTS_EQUAL(rsd / rds, (ratio{5*11, 9*7}));
  TTS_CONSTEXPR_EXPECT    ( kwk::concepts::static_constant<decltype((rsd / rds).num)> );
  TTS_CONSTEXPR_EXPECT_NOT( kwk::concepts::static_constant<decltype((rsd / rds).denum)> );

  TTS_EQUAL(rds / rss, (ratio{9*15, 11*13}));
  TTS_CONSTEXPR_EXPECT_NOT( kwk::concepts::static_constant<decltype((rds / rss).num)> );
  TTS_CONSTEXPR_EXPECT    ( kwk::concepts::static_constant<decltype((rds / rss).denum)> );

  TTS_EQUAL(rss / rss, (ratio{13*15, 15*13}));
  TTS_CONSTEXPR_EXPECT    ( kwk::concepts::static_constant<decltype((rss / rss).num)> );
  TTS_CONSTEXPR_EXPECT    ( kwk::concepts::static_constant<decltype((rss / rss).denum)> );
};