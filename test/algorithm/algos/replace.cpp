//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/algorithm/algos/replace.hpp>
#include <kwk/container.hpp>
#include "test.hpp"

TTS_CASE("Check for kwk::replace(value, new_value) 1D")
{
  int data[2];
  int vdata[2];

  fill_data(data, kwk::of_size(2), true);
  fill_data(vdata, kwk::of_size(2), true);

  vdata[1] = 10;

  auto v = kwk::view{kwk::source = data, kwk::of_size(2)};

  kwk::replace(v, 1, 10);

  TTS_ALL_EQUAL(data, vdata);
};

TTS_CASE("Check for kwk::replace(value, new_value) 2D")
{
  int data[2*3];
  int vdata[2*3];

  fill_data(data, kwk::of_size(2,3), true);
  fill_data(vdata, kwk::of_size(2,3), true);

  vdata[1*3+2] = 120;

  auto v = kwk::view{kwk::source = data, kwk::of_size(2,3)};

  kwk::replace(v, 12, 120);

  TTS_ALL_EQUAL(data, vdata);
};

TTS_CASE("Check for kwk::replace(value, new_value) 2D - with CPU context")
{
  int data[2*3];
  int vdata[2*3];

  fill_data(data, kwk::of_size(2,3), true);
  fill_data(vdata, kwk::of_size(2,3), true);

  vdata[1*3+2] = 120;

  auto v = kwk::view{kwk::source = data, kwk::of_size(2,3)};

  kwk::replace(kwk::cpu, v, 12, 120);

  TTS_ALL_EQUAL(data, vdata);
};

TTS_CASE("Check for kwk::replace(value, new_value) 3D")
{
  int data[2*3*4];
  int vdata[2*3*4];

  fill_data(data, kwk::of_size(2,3,4), true);
  fill_data(vdata, kwk::of_size(2,3,4), true);

  vdata[1*4*3+2*4+3] = 1230;

  auto v = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

  kwk::replace(v, 123, 1230);

  TTS_ALL_EQUAL(data, vdata);
};

TTS_CASE("Check for kwk::replace(value, new_value) 4D")
{
  int data[2*3*4*5];
  int vdata[2*3*4*5];

  fill_data(data, kwk::of_size(2,3,4,5), true);
  fill_data(vdata, kwk::of_size(2,3,4,5), true);

  vdata[1*5*4*3+2*5*4+3*5+4] = 12340;

  auto v = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

  kwk::replace(v, 1234, 12340);

  TTS_ALL_EQUAL(data, vdata);
};

TTS_CASE("Check for kwk::replace_if(func, new_value) 1D")
{
  int data[2];
  int vdata[2];

  fill_data(data, kwk::of_size(2), true);
  fill_data(vdata, kwk::of_size(2), false);

  auto v = kwk::view{kwk::source = data, kwk::of_size(2)};

  int count = 0;
  kwk::replace_if(v, [&](auto e)
  { 
    count++;
    return (e>=0);
  } 
  , 0);

  TTS_ALL_EQUAL(data, vdata);
  TTS_EQUAL(count,   v.numel());
};

TTS_CASE("Check for kwk::replace_if(func, new_value) 2D")
{
  int data[2*3];
  int vdata[2*3];

  fill_data(data, kwk::of_size(2,3), true);
  fill_data(vdata, kwk::of_size(2,3), false);

  auto v = kwk::view{kwk::source = data, kwk::of_size(2,3)};

  int count = 0;
  kwk::replace_if(v, [&](auto e)
  { 
    count++;
    return (e>=0);
  } 
  , 0);

  TTS_ALL_EQUAL(data, vdata);
  TTS_EQUAL(count,   v.numel());
};

TTS_CASE("Check for kwk::replace_if(func, new_value) 2D - with CPU context")
{
  int data[2*3];
  int vdata[2*3];

  fill_data(data, kwk::of_size(2,3), true);
  fill_data(vdata, kwk::of_size(2,3), false);

  auto v = kwk::view{kwk::source = data, kwk::of_size(2,3)};

  int count = 0;
  kwk::replace_if(kwk::cpu, v, [&](auto e)
  { 
    count++;
    return (e>=0);
  } 
  , 0);

  TTS_ALL_EQUAL(data, vdata);
  TTS_EQUAL(count,   v.numel());
};

TTS_CASE("Check for kwk::replace_if(func, new_value) 3D")
{
  int data[2*3*4];
  int vdata[2*3*4];

  fill_data(data, kwk::of_size(2,3,4), true);
  fill_data(vdata, kwk::of_size(2,3,4), false);

  auto v = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

  int count = 0;
  kwk::replace_if(v, [&](auto e)
  { 
    count++;
    return (e>=0);
  } 
  , 0);

  TTS_ALL_EQUAL(data, vdata);
  TTS_EQUAL(count,   v.numel());
};

TTS_CASE("Check for kwk::replace_if(func, new_value) 4D")
{
  int data[2*3*4*5];
  int vdata[2*3*4*5];

  fill_data(data, kwk::of_size(2,3,4,5), true);
  fill_data(vdata, kwk::of_size(2,3,4,5), false);

  auto v = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

  int count = 0;
  kwk::replace_if(v, [&](auto e)
  { 
    count++;
    return (e>=0);
  } 
  , 0);

  TTS_ALL_EQUAL(data, vdata);
  TTS_EQUAL(count,   v.numel());
};

TTS_CASE("Check for kwk::replace_if(func, new_value) 4D smaller view")
{
  int data[2*3*4*5];
  int vdata[2*3*4*5];

  fill_data(data, kwk::of_size(2,3,4,5), true);
  fill_data(vdata, kwk::of_size(2,3,4,5), false);

  for(int j = 0; j<3; j++)
    for(int k = 0; k<4; k++)
      for(int l = 0; l<5; l++)
        vdata[1*5*4*3+j*5*4+k*5+l] = 1000+100*j+10*k+l;

  auto v = kwk::view{kwk::source = data, kwk::of_size(1,3,4,5)};

  int count = 0;
  kwk::replace_if(v, [&](auto e)
  { 
    count++;
    return (e>=0);
  } 
  , 0);

  TTS_ALL_EQUAL(data, vdata);
  TTS_EQUAL(count,   v.numel());
};

TTS_CASE("Check for kwk::replace_if(func, new_value) 4D smaller view - with CPU context")
{
  int data[2*3*4*5];
  int vdata[2*3*4*5];

  fill_data(data, kwk::of_size(2,3,4,5), true);
  fill_data(vdata, kwk::of_size(2,3,4,5), false);

  for(int j = 0; j<3; j++)
    for(int k = 0; k<4; k++)
      for(int l = 0; l<5; l++)
        vdata[1*5*4*3+j*5*4+k*5+l] = 1000+100*j+10*k+l;

  auto v = kwk::view{kwk::source = data, kwk::of_size(1,3,4,5)};

  int count = 0;
  kwk::replace_if(kwk::cpu, v, [&](auto e)
  { 
    count++;
    return (e>=0);
  } 
  , 0);

  TTS_ALL_EQUAL(data, vdata);
  TTS_EQUAL(count,   v.numel());
};

// How to take a view from 2 3 4 5 to 2 3 4 4 ?

// TTS_CASE("Check for kwk::replace_if(func, new_value) 4D smaller view")
// {
//   int data[2*3*4*5];
//   int vdata[2*3*4*5];

//   fill_data(data, kwk::of_size(2,3,4,5), true);
//   fill_data(vdat, kwk::of_size(2,3,4,5), false);

//   for(int i = 0; i<2; i++)
//     for(int j = 0; j<3; j++)
//       for(int k = 0; k<4; k++)
//         vdata[i*j*k*4] = 1000*i+100*j+10*k+4;

//   auto v = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5), kwk::of_size(2,3,4,4)};

//   int count = 0;
//   kwk::replace_if(v, [&](auto e)
//   { 
//     count++;
//     return (e>=0);
//   } 
//   , 0);

//   TTS_ALL_EQUAL(data, vdata);
//   TTS_EQUAL(count,   v.numel());
// };