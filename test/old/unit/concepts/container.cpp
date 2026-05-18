//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/concepts/container.hpp>
#include <kwk/container.hpp>

using table_2D_f32_t    = kwk::make_table_t< kwk::type<float>       , kwk::_2D        >;
using table_4x6_f32_t   = kwk::make_table_t< kwk::type<float>       , kwk::shape<4,6> >;
using view_3D_i32_t     = kwk::make_view_t < kwk::_3D               , kwk::type<int>  >;
using view_5x5xn_i32_t  = kwk::make_view_t < kwk::shape<5,5,kwk::_> , kwk::type<int>  >;

TTS_CASE( "Checks concepts::container behavior" )
{
  TTS_CONSTEXPR_EXPECT    (kwk::concepts::container<table_2D_f32_t>);
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::container<table_2D_f32_t, kwk::type<float>>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::container<table_2D_f32_t, kwk::type<int>>));
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::container<table_2D_f32_t, kwk::_2D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::container<table_2D_f32_t, kwk::_3D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::container<table_2D_f32_t, kwk::shape<4,kwk::_>>));

  TTS_CONSTEXPR_EXPECT    (kwk::concepts::container<table_4x6_f32_t>);
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::container<table_2D_f32_t, kwk::type<float>>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::container<table_2D_f32_t, kwk::type<int>>));
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::container<table_4x6_f32_t, kwk::_2D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::container<table_4x6_f32_t, kwk::_3D>));
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::container<table_4x6_f32_t, kwk::shape<4,kwk::_>>));

  TTS_CONSTEXPR_EXPECT    (kwk::concepts::container<view_3D_i32_t>);
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::container<view_3D_i32_t, kwk::type<float>>));
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::container<view_3D_i32_t, kwk::type<int>>));
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::container<view_3D_i32_t, kwk::_3D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::container<view_3D_i32_t, kwk::_2D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::container<view_3D_i32_t, kwk::shape<4,kwk::_>>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::container<view_3D_i32_t, kwk::shape<5,kwk::_,kwk::_>>));

  TTS_CONSTEXPR_EXPECT    (kwk::concepts::container<view_5x5xn_i32_t>);
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::container<view_5x5xn_i32_t, kwk::type<int>>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::container<view_5x5xn_i32_t, kwk::type<float>>));
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::container<view_5x5xn_i32_t, kwk::_3D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::container<view_5x5xn_i32_t, kwk::_2D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::container<view_5x5xn_i32_t, kwk::shape<4,kwk::_>>));
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::container<view_5x5xn_i32_t, kwk::shape<5,kwk::_,kwk::_>>));
};

TTS_CASE( "Checks concepts::table behavior" )
{
  TTS_CONSTEXPR_EXPECT    (kwk::concepts::table<table_2D_f32_t>);
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::table<table_2D_f32_t, kwk::_2D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::table<table_2D_f32_t, kwk::_3D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::table<table_2D_f32_t, kwk::shape<4,kwk::_>>));

  TTS_CONSTEXPR_EXPECT    (kwk::concepts::table<table_4x6_f32_t>);
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::table<table_4x6_f32_t, kwk::_2D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::table<table_4x6_f32_t, kwk::_3D>));
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::table<table_4x6_f32_t, kwk::shape<4,kwk::_>>));

  TTS_CONSTEXPR_EXPECT_NOT(kwk::concepts::table<view_3D_i32_t>);
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::table<view_3D_i32_t, kwk::_3D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::table<view_3D_i32_t, kwk::_2D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::table<view_3D_i32_t, kwk::shape<4,kwk::_>>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::table<view_3D_i32_t, kwk::shape<5,kwk::_,kwk::_>>));

  TTS_CONSTEXPR_EXPECT_NOT(kwk::concepts::table<view_5x5xn_i32_t>);
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::table<view_5x5xn_i32_t, kwk::_3D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::table<view_5x5xn_i32_t, kwk::_2D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::table<view_5x5xn_i32_t, kwk::shape<4,kwk::_>>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::table<view_5x5xn_i32_t, kwk::shape<5,kwk::_,kwk::_>>));
};

TTS_CASE( "Checks concepts::view behavior" )
{
  TTS_CONSTEXPR_EXPECT_NOT(kwk::concepts::view<table_2D_f32_t>);
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::view<table_2D_f32_t, kwk::_2D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::view<table_2D_f32_t, kwk::_3D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::view<table_2D_f32_t, kwk::shape<4,kwk::_>>));

  TTS_CONSTEXPR_EXPECT_NOT(kwk::concepts::view<table_4x6_f32_t>);
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::view<table_4x6_f32_t, kwk::_2D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::view<table_4x6_f32_t, kwk::_3D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::view<table_4x6_f32_t, kwk::shape<4,kwk::_>>));

  TTS_CONSTEXPR_EXPECT    (kwk::concepts::view<view_3D_i32_t>);
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::view<view_3D_i32_t, kwk::_3D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::view<view_3D_i32_t, kwk::_2D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::view<view_3D_i32_t, kwk::shape<4,kwk::_>>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::view<view_3D_i32_t, kwk::shape<5,kwk::_,kwk::_>>));

  TTS_CONSTEXPR_EXPECT    (kwk::concepts::view<view_5x5xn_i32_t>);
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::view<view_5x5xn_i32_t, kwk::_3D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::view<view_5x5xn_i32_t, kwk::_2D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::view<view_5x5xn_i32_t, kwk::shape<4,kwk::_>>));
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::view<view_5x5xn_i32_t, kwk::shape<5,kwk::_,kwk::_>>));
};

