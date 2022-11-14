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

TTS_CASE( "Checks concepts::container behavior" )
{
  using table_2D_f32_t    = kwk::make_table_t< kwk::as<float>, kwk::_2D>;
  using table_4xn_f32_t   = kwk::make_table_t< kwk::as<float>, kwk::extent[4][6]>;
  using view_3D_i32_t     = kwk::make_view_t< kwk::_3D, kwk::as<int>>;
  using view_5x5xn_i32_t  = kwk::make_view_t< kwk::extent[5][5](), kwk::as<int>>;

  TTS_CONSTEXPR_EXPECT    (kwk::concepts::container<table_2D_f32_t>);
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::container<table_2D_f32_t, kwk::as<float>>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::container<table_2D_f32_t, kwk::as<int>>));
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::container<table_2D_f32_t, kwk::_2D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::container<table_2D_f32_t, kwk::_3D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::container<table_2D_f32_t, kwk::extent[4]()>));

  TTS_CONSTEXPR_EXPECT    (kwk::concepts::container<table_4xn_f32_t>);
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::container<table_2D_f32_t, kwk::as<float>>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::container<table_2D_f32_t, kwk::as<int>>));
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::container<table_4xn_f32_t, kwk::_2D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::container<table_4xn_f32_t, kwk::_3D>));
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::container<table_4xn_f32_t, kwk::extent[4]()>));

  TTS_CONSTEXPR_EXPECT    (kwk::concepts::container<view_3D_i32_t>);
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::container<view_3D_i32_t, kwk::as<float>>));
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::container<view_3D_i32_t, kwk::as<int>>));
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::container<view_3D_i32_t, kwk::_3D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::container<view_3D_i32_t, kwk::_2D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::container<view_3D_i32_t, kwk::extent[4]()>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::container<view_3D_i32_t, kwk::extent[5]()()>));

  TTS_CONSTEXPR_EXPECT    (kwk::concepts::container<view_5x5xn_i32_t>);
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::container<view_5x5xn_i32_t, kwk::as<int>>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::container<view_5x5xn_i32_t, kwk::as<float>>));
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::container<view_5x5xn_i32_t, kwk::_3D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::container<view_5x5xn_i32_t, kwk::_2D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::container<view_5x5xn_i32_t, kwk::extent[4]()>));
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::container<view_5x5xn_i32_t, kwk::extent[5]()()>));
};

TTS_CASE( "Checks concepts::table behavior" )
{
  using table_2D_f32_t    = kwk::make_table_t< kwk::as<float>, kwk::_2D>;
  using table_4xn_f32_t   = kwk::make_table_t< kwk::as<float>, kwk::extent[4][6]>;
  using view_3D_i32_t     = kwk::make_view_t< kwk::_3D, kwk::as<int>>;
  using view_5x5xn_i32_t  = kwk::make_view_t< kwk::extent[5][5](), kwk::as<int>>;

  TTS_CONSTEXPR_EXPECT    (kwk::concepts::table<table_2D_f32_t>);
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::table<table_2D_f32_t, kwk::_2D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::table<table_2D_f32_t, kwk::_3D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::table<table_2D_f32_t, kwk::extent[4]()>));

  TTS_CONSTEXPR_EXPECT    (kwk::concepts::table<table_4xn_f32_t>);
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::table<table_4xn_f32_t, kwk::_2D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::table<table_4xn_f32_t, kwk::_3D>));
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::table<table_4xn_f32_t, kwk::extent[4]()>));

  TTS_CONSTEXPR_EXPECT_NOT(kwk::concepts::table<view_3D_i32_t>);
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::table<view_3D_i32_t, kwk::_3D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::table<view_3D_i32_t, kwk::_2D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::table<view_3D_i32_t, kwk::extent[4]()>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::table<view_3D_i32_t, kwk::extent[5]()()>));

  TTS_CONSTEXPR_EXPECT_NOT(kwk::concepts::table<view_5x5xn_i32_t>);
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::table<view_5x5xn_i32_t, kwk::_3D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::table<view_5x5xn_i32_t, kwk::_2D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::table<view_5x5xn_i32_t, kwk::extent[4]()>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::table<view_5x5xn_i32_t, kwk::extent[5]()()>));
};

TTS_CASE( "Checks concepts::view behavior" )
{
  using table_2D_f32_t    = kwk::make_table_t< kwk::as<float>, kwk::_2D>;
  using table_4xn_f32_t   = kwk::make_table_t< kwk::as<float>, kwk::extent[4][6]>;
  using view_3D_i32_t     = kwk::make_view_t< kwk::_3D, kwk::as<int>>;
  using view_5x5xn_i32_t  = kwk::make_view_t< kwk::extent[5][5](), kwk::as<int>>;

  TTS_CONSTEXPR_EXPECT_NOT(kwk::concepts::view<table_2D_f32_t>);
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::view<table_2D_f32_t, kwk::_2D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::view<table_2D_f32_t, kwk::_3D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::view<table_2D_f32_t, kwk::extent[4]()>));

  TTS_CONSTEXPR_EXPECT_NOT(kwk::concepts::view<table_4xn_f32_t>);
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::view<table_4xn_f32_t, kwk::_2D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::view<table_4xn_f32_t, kwk::_3D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::view<table_4xn_f32_t, kwk::extent[4]()>));

  TTS_CONSTEXPR_EXPECT    (kwk::concepts::view<view_3D_i32_t>);
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::view<view_3D_i32_t, kwk::_3D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::view<view_3D_i32_t, kwk::_2D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::view<view_3D_i32_t, kwk::extent[4]()>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::view<view_3D_i32_t, kwk::extent[5]()()>));

  TTS_CONSTEXPR_EXPECT    (kwk::concepts::view<view_5x5xn_i32_t>);
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::view<view_5x5xn_i32_t, kwk::_3D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::view<view_5x5xn_i32_t, kwk::_2D>));
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::view<view_5x5xn_i32_t, kwk::extent[4]()>));
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::view<view_5x5xn_i32_t, kwk::extent[5]()()>));
};
