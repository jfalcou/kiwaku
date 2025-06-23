//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <kwk/context/eve/context.hpp>
#include <kwk/algorithm/algos/generator.hpp>
#include <kwk/container.hpp>
#include "test.hpp"

TTS_CASE("Check for kwk::fill(out, value) 1D")
{
  int data[2];

  int vdata[2]  =  { 1, 1 };

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};
  
  kwk::fill(kwk::simd, d, 1);

  TTS_ALL_EQUAL(data, vdata);
};

TTS_CASE("Check for kwk::fill(out, value) 2D")
{
  int data[2*3];

  int vdata[2*3];

  for(int i = 0; i<2; i++)
    for(int j = 0; j<3; j++)
      vdata[i*3+j] = 2;

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};
  
  kwk::fill(kwk::simd,d, 2);

  TTS_ALL_EQUAL(data, vdata);
};

TTS_CASE("Check for kwk::fill(out, value) 3D")
{
  int data[2*3*4];

  int vdata[2*3*4];

  for(int i = 0; i<2; i++)
    for(int j = 0; j<3; j++)
      for(int k = 0; k<4; k++)
        vdata[i*4*3+j*4+k] = 3;

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};
  
  kwk::fill(kwk::simd, d, 3);

  TTS_ALL_EQUAL(data, vdata);
};

TTS_CASE("Check for kwk::fill(out, value) 4D")
{
  int data[2*3*4*5];

  int vdata[2*3*4*5];

  for(int i = 0; i<2; i++)
    for(int j = 0; j<3; j++)
      for(int k = 0; k<4; k++)
        for(int l = 0; l<5; l++)
          vdata[i*5*4*3+j*5*4+k*5+l] = 4;

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};
  
  kwk::fill(kwk::simd, d, 4);

  TTS_ALL_EQUAL(data, vdata);
};

TTS_CASE("Check for kwk::iota(out, value) 1D")
{
  int data[2];
  int vdata[2] = {1,2};

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};
  
  kwk::iota(kwk::simd,d, 1);

  TTS_ALL_EQUAL(data, vdata);
};


TTS_CASE("Check for kwk::iota(out, value) 2D")
{
  int data[2*3];
  int vdata[2*3];

  int iot = 2;
  for(int i = 0; i<2; i++)
    for(int j = 0; j<3; j++)
    {
      vdata[i*3+j] = iot;
      iot++;
    }

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};
  
  kwk::iota(kwk::simd,d,2);

  TTS_ALL_EQUAL(data, vdata);
};

TTS_CASE("Check for kwk::iota(out, value) 3D")
{
  int data[2*3*4];
  int vdata[2*3*4];

  int iot = 3;
  for(int i = 0; i<2; i++)
    for(int j = 0; j<3; j++)
      for(int k = 0; k<4; k++)
      {
        vdata[i*4*3+j*4+k] = iot;
        iot++;
      }

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};
  
  kwk::iota(kwk::simd,d,3);

  TTS_ALL_EQUAL(data, vdata);
};

TTS_CASE("Check for kwk::iota(out, value) 4D")
{
  int data[2*3*4*5];
  int vdata[2*3*4*5];

  int iot = 4;
  for(int i = 0; i<2; i++)
    for(int j = 0; j<3; j++)
      for(int k = 0; k<4; k++)
        for(int l = 0; l<5; l++)
        {
          vdata[i*5*4*3+j*5*4+k*5+l] = iot;
          iot++;
        }

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};
  
  kwk::iota(kwk::simd,d,4);

  TTS_ALL_EQUAL(data, vdata);
};

TTS_CASE("Check for kwk::iota(out, value, step) 1D")
{
  int data[2];
  int vdata[2] = {0,10};

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};
  
  kwk::iota(kwk::simd,d, 0, 10);

  TTS_ALL_EQUAL(data, vdata);
};


TTS_CASE("Check for kwk::iota(out, value, step) 2D")
{
  int data[2*3];
  int vdata[2*3];

  int iot = 0;
  for(int i = 0; i<2; i++)
    for(int j = 0; j<3; j++)
    {
      vdata[i*3+j] = iot;
      iot+=20;
    }

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};
  
  kwk::iota(kwk::simd,d,0,20);

  TTS_ALL_EQUAL(data, vdata);
};

TTS_CASE("Check for kwk::iota(out, value, step) 3D")
{
  int data[2*3*4];
  int vdata[2*3*4];

  int iot = 0;
  for(int i = 0; i<2; i++)
    for(int j = 0; j<3; j++)
      for(int k = 0; k<4; k++)
      {
        vdata[i*4*3+j*4+k] = iot;
        iot+=30;
      }

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};
  
  kwk::iota(kwk::simd,d,0,30);

  TTS_ALL_EQUAL(data, vdata);
};

TTS_CASE("Check for kwk::iota(out, value, step) 4D")
{
  int data[2*3*4*5];
  int vdata[2*3*4*5];

  int iot = 0;
  for(int i = 0; i<2; i++)
    for(int j = 0; j<3; j++)
      for(int k = 0; k<4; k++)
        for(int l = 0; l<5; l++)
        {
          vdata[i*5*4*3+j*5*4+k*5+l] = iot;
          iot+=40;
        }

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};
  
  kwk::iota(kwk::simd,d,0,40);

  TTS_ALL_EQUAL(data, vdata);
};

TTS_CASE("Check for float kwk::iota(out, value, step) 4D")
{
  float data[2*3*4*5];
  float vdata[2*3*4*5];

  float iot = 0.f;
  for(int i = 0; i<2; i++)
    for(int j = 0; j<3; j++)
      for(int k = 0; k<4; k++)
        for(int l = 0; l<5; l++)
        {
          vdata[i*5*4*3+j*5*4+k*5+l] = iot;
          iot+=40.5f;
        }

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};
  
  kwk::iota(kwk::simd,d,0.f,40.5f);

  TTS_ALL_EQUAL(data, vdata);
};
