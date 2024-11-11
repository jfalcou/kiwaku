//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <cstdlib>
#include <kwk/algorithm/algos/find.hpp>
#include <kwk/container.hpp>
#include "test.hpp"

TTS_CASE("Check for kwk::find(In, value) 1D")
{
  int data[2];
  auto vdata = kumi::iota<1>(1);

  fill_data(data, kwk::of_size(2), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

  auto res = kwk::find(d, 1);

  TTS_EQUAL(res, vdata);
};

TTS_CASE("Check for kwk::find(In, value) 2D")
{
  int data[2*3];
  auto vdata = kumi::iota<2>(1);

  fill_data(data, kwk::of_size(2,3), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

  auto res = kwk::find(d, 12);

  TTS_EQUAL(res, vdata);
};

TTS_CASE("Check for kwk::find(In, value) 3D")
{
  int data[2*3*4];
  auto vdata = kumi::iota<3>(1);

  fill_data(data, kwk::of_size(2,3,4), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

  auto res = kwk::find(d, 123);

  TTS_EQUAL(res, vdata);
};

TTS_CASE("Check for kwk::find(In, value) 4D")
{
  int data[2*3*4*5];
  auto vdata = kumi::iota<4>(1);

  fill_data(data, kwk::of_size(2,3,4,5), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

  auto res = kwk::find(d, 1234);

  TTS_EQUAL(res, vdata);
};

TTS_CASE("Check for kwk::find_if(In, func) 1D")
{
  int data[2];
  auto vdata = kumi::iota<1>(1);

  fill_data(data, kwk::of_size(2), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

  int count = 0;
  auto res = kwk::find_if(d, [&](auto e){ count++; return (e==1); });

  TTS_EQUAL(res, vdata);
  TTS_EQUAL(count, d.numel());
};

TTS_CASE("Check for kwk::find_if(In, func) 2D")
{
  int data[2*3];
  auto vdata = kumi::iota<2>(1);

  fill_data(data, kwk::of_size(2,3), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

  int count = 0;
  auto res = kwk::find_if(d, [&](auto e){ count++; return (e==12); });

  TTS_EQUAL(res, vdata);
  TTS_EQUAL(count, d.numel());
};

TTS_CASE("Check for kwk::find_if(In, func) 3D")
{
  int data[2*3*4];
  auto vdata = kumi::iota<3>(1);

  fill_data(data, kwk::of_size(2,3,4), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

  int count = 0;
  auto res = kwk::find_if(d, [&](auto e){ count++; return (e==123); });

  TTS_EQUAL(res, vdata);
  TTS_EQUAL(count, d.numel());
};

TTS_CASE("Check for kwk::find_if(In, func) 4D")
{
  int data[2*3*4*5];
  auto vdata = kumi::iota<4>(1);

  fill_data(data, kwk::of_size(2,3,4,5), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

  int count = 0;
  auto res = kwk::find_if(d, [&](auto e){ count++; return (e==1234); });

  TTS_EQUAL(res, vdata);
  TTS_EQUAL(count, d.numel());
};


TTS_CASE("Check for kwk::find_if_not(In, func) 1D")
{
  int data[2];
  auto vdata = kumi::iota<1>(1);

  fill_data(data, kwk::of_size(2), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

  int count = 0;
  auto res = kwk::find_if_not(d, [&](auto e){ count++; return (e<1); });

  TTS_EQUAL(res, vdata);
  TTS_EQUAL(count, d.numel());
};

TTS_CASE("Check for kwk::find_if_not(In, func) 2D")
{
  int data[2*3];
  auto vdata = kumi::iota<2>(1);

  fill_data(data, kwk::of_size(2,3), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

  int count = 0;
  auto res = kwk::find_if_not(d, [&](auto e){ count++; return (e<12); });

  TTS_EQUAL(res, vdata);
  TTS_EQUAL(count, d.numel());
};

TTS_CASE("Check for kwk::find_if_not(In, func) 3D")
{
  int data[2*3*4];
  auto vdata = kumi::iota<3>(1);

  fill_data(data, kwk::of_size(2,3,4), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

  int count = 0;
  auto res = kwk::find_if_not(d, [&](auto e){ count++; return (e<123); });

  TTS_EQUAL(res, vdata);
  TTS_EQUAL(count, d.numel());
};

TTS_CASE("Check for kwk::find_if_not(In, func) 4D")
{
  int data[2*3*4*5];
  auto vdata = kumi::iota<4>(1);

  fill_data(data, kwk::of_size(2,3,4,5), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

  int count = 0;
  auto res = kwk::find_if_not(d, [&](auto e){ count++; return (e<1234); });

  TTS_EQUAL(res, vdata);
  TTS_EQUAL(count, d.numel());
};


TTS_CASE("Check for kwk::find_first_of(In, func) 1D")
{
  int data[2];
  int value[] = {1111, 11111, 30, 1, 12};
  auto vdata = kumi::generate<1,int>(1);

  fill_data(data, kwk::of_size(2), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};
  auto v = kwk::view{kwk::source = value};

  auto res = kwk::find_first_of(d, v);

  TTS_EQUAL(res, vdata);
};

TTS_CASE("Check for kwk::find_first_of(In, func) 2D")
{
  int data[2*3];
  int value[] = {11, 12};
  auto vdata = kumi::generate<2, int>(1);

  fill_data(data, kwk::of_size(2,3), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};
  auto v = kwk::view{kwk::source = value};

  auto res = kwk::find_first_of(d, v);

  TTS_EQUAL(res, vdata);
};

TTS_CASE("Check for kwk::find_first_of(In, func) 3D")
{
  int data[2*3*4];
  int value[] = {111, 122};
  auto vdata = kumi::generate<3, int>(1);

  fill_data(data, kwk::of_size(2,3,4), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};
  auto v = kwk::view{kwk::source = value};

  auto res = kwk::find_first_of(d, v);

  TTS_EQUAL(res, vdata);
};

TTS_CASE("Check for kwk::find_first_of(In, func) 4D")
{
  int data[2*3*4*5];
  int value[] = {1111, 1222};
  auto vdata = kumi::generate<4, int>(1);

  fill_data(data, kwk::of_size(2,3,4,5), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};
  auto v = kwk::view{kwk::source = value};

  auto res = kwk::find_first_of(d, v);

  TTS_EQUAL(res, vdata);
};

TTS_CASE("Check for kwk::find_last(In, value) 1D")
{
  int data[2];
  auto vdata = kumi::iota<1>(1);

  fill_data(data, kwk::of_size(2), false);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

  auto res = kwk::find_last(d, 0);

  std::cout << res << std::endl;

  TTS_EQUAL(res, vdata);
};

// Wait for merge V2
// TTS_CASE("Check for kwk::find_last(In, value) 2D")
// {
//   int data[2*3];
//   auto vdata = kumi::iota<2>(1);

//   fill_data(data, kwk::of_size(2,3), false);

//   auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

//   auto res = kwk::find_last(d, 0);

//   std::cout << res << std::endl;

//   TTS_EQUAL(res, vdata);
// };

TTS_CASE("Check for kwk::find_last(In, value) 3D")
{
  int data[2*3*4];
  auto vdata = kumi::iota<3>(1);

  fill_data(data, kwk::of_size(2,3,4), false);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

  auto res = kwk::find_last(d, 0);

  std::cout << res << std::endl;

  TTS_EQUAL(res, vdata);
};

TTS_CASE("Check for kwk::find_last(In, value) 4D")
{
  int data[2*3*4*5];
  auto vdata = kumi::iota<4>(1);

  fill_data(data, kwk::of_size(2,3,4,5), false);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

  auto res = kwk::find_last(d, 0);

  std::cout << res << std::endl;

  TTS_EQUAL(res, vdata);
};

TTS_CASE("Check for kwk::find_last_if(In, value) 1D")
{
  int data[2];
  auto vdata = kumi::iota<1>(1);

  fill_data(data, kwk::of_size(2), false);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

  auto res = kwk::find_last_if(d, [](auto e){return e==0;});

  std::cout << res << std::endl;

  TTS_EQUAL(res, vdata);
};

// Wait merge shape V2
// TTS_CASE("Check for kwk::find_last_if(In, value) 2D")
// {
//   int data[2*3];
//   auto vdata = kumi::iota<2>(1);

//   fill_data(data, kwk::of_size(2,3), false);

//   auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

//   auto res = kwk::find_last_if(d, [](auto e){return e==0;});

//   std::cout << res << std::endl;

//   TTS_EQUAL(res, vdata);
// };

TTS_CASE("Check for kwk::find_last_if(In, value) 3D")
{
  int data[2*3*4];
  auto vdata = kumi::iota<3>(1);

  fill_data(data, kwk::of_size(2,3,4), false);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

  auto res = kwk::find_last_if(d, [](auto e){return e==0;});

  std::cout << res << std::endl;

  TTS_EQUAL(res, vdata);
};

TTS_CASE("Check for kwk::find_last_if(In, value) 4D")
{
  int data[2*3*4*5];
  auto vdata = kumi::iota<4>(1);

  fill_data(data, kwk::of_size(2,3,4,5), false);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

  auto res = kwk::find_last_if(d, [](auto e){return e==0;});

  std::cout << res << std::endl;

  TTS_EQUAL(res, vdata);
};

TTS_CASE("Check for kwk::find_last_if_not(In, value) 1D")
{
  int data[2];
  auto vdata = kumi::iota<1>(1);

  fill_data(data, kwk::of_size(2), false);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

  auto res = kwk::find_last_if_not(d, [](auto e){return e!=0;});

  std::cout << res << std::endl;

  TTS_EQUAL(res, vdata);
};

// TTS_CASE("Check for kwk::find_last_if_not(In, value) 2D")
// {
//   int data[2*3];
//   auto vdata = kumi::iota<2>(1);

//   fill_data(data, kwk::of_size(2,3), false);

//   auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

//   auto res = kwk::find_last_if_not(d, [](auto e){return e!=0;});

//   std::cout << res << std::endl;

//   TTS_EQUAL(res, vdata);
// };

TTS_CASE("Check for kwk::find_last_if_not(In, value) 3D")
{
  int data[2*3*4];
  auto vdata = kumi::iota<3>(1);

  fill_data(data, kwk::of_size(2,3,4), false);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

  auto res = kwk::find_last_if_not(d, [](auto e){return e!=0;});

  std::cout << res << std::endl;

  TTS_EQUAL(res, vdata);
};

TTS_CASE("Check for kwk::find_last_if_not(In, value) 4D")
{
  int data[2*3*4*5];
  auto vdata = kumi::iota<4>(1);

  fill_data(data, kwk::of_size(2,3,4,5), false);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

  auto res = kwk::find_last_if_not(d, [](auto e){return e!=0;});

  std::cout << res << std::endl;

  TTS_EQUAL(res, vdata);
};