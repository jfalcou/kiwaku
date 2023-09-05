//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <kwk/algorithm/algos/generator.hpp>
#include <kwk/container.hpp>
#include "test.hpp"


TTS_CASE("Check for kwk::fill(out, value) 1D")
{
  int data[2];

  int vdata[2]  =  { 1, 1 };

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};
  
  kwk::fill(d, 1);

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
  
  kwk::fill(d, 2);

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
  
  kwk::fill(d, 3);

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
  
  kwk::fill(d, 4);

  TTS_ALL_EQUAL(data, vdata);
};

TTS_CASE("Check for kwk::generate(out, func) 1D")
{
  int data[2];
  int vdata[2];

  fill_data(vdata, kwk::of_size(2), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};
  
  int count = 0;
  kwk::generate([&](auto p1)
  { 
    count++;
    return p1; 
  }
  , d);

  TTS_ALL_EQUAL(data, vdata);
  TTS_EQUAL(count, d.numel());
};

TTS_CASE("Check for kwk::generate(out, func) 2D")
{
  int data[2*3];
  int vdata[2*3];

  fill_data(vdata, kwk::of_size(2,3), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};
  
  int count = 0;
  kwk::generate([&](auto p1, auto p2)
  { 
    count++;
    return 10*p1+p2; 
  }
  , d);

  TTS_ALL_EQUAL(data, vdata);
  TTS_EQUAL(count, d.numel());
};

TTS_CASE("Check for kwk::generate(out, func) 3D")
{
  int data[2*3*4];
  int vdata[2*3*4];

  fill_data(vdata, kwk::of_size(2,3,4), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};
  
  int count = 0;
  kwk::generate([&](auto p1, auto p2, auto p3)
  { 
    count++;
    return 100*p1+10*p2+p3; 
  }
  , d);

  TTS_ALL_EQUAL(data, vdata);
  TTS_EQUAL(count, d.numel());
};

TTS_CASE("Check for kwk::generate(out, func) 4D")
{
  int data[2*3*4*5];
  int vdata[2*3*4*5];

  fill_data(vdata, kwk::of_size(2,3,4,5), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};
  
  int count = 0;
  kwk::generate([&](auto p1, auto p2, auto p3, auto p4)
  { 
    count++;
    return 1000*p1+100*p2+10*p3+p4; 
  }
  , d);

  TTS_ALL_EQUAL(data, vdata);
  TTS_EQUAL(count, d.numel());
};

TTS_CASE("Check for kwk::iota(out) 1D")
{
  int data[2];
  int vdata[2]  =  { 0, 1};

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};
  
  kwk::iota(d);

  TTS_ALL_EQUAL(data, vdata);
};

TTS_CASE("Check for kwk::iota(out) 2D")
{
  int data[2*3];
  int vdata[2*3];

  int iot = 0;
  for(int i = 0; i<2; i++)
    for(int j = 0; j<3; j++)
    {
      vdata[i*3+j] = iot;
      iot++;
    }

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};
  
  kwk::iota(d);

  TTS_ALL_EQUAL(data, vdata);
};

TTS_CASE("Check for kwk::iota(out) 3D")
{
  int data[2*3*4];
  int vdata[2*3*4];

  int iot = 0;
  for(int i = 0; i<2; i++)
    for(int j = 0; j<3; j++)
      for(int k = 0; k<4; k++)
      {
        vdata[i*4*3+j*4+k] = iot;
        iot++;
      }

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};
  
  kwk::iota(d);

  TTS_ALL_EQUAL(data, vdata);
};

TTS_CASE("Check for kwk::iota(out) 4D")
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
          iot++;
        }

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};
  
  kwk::iota(d);

  TTS_ALL_EQUAL(data, vdata);
};

TTS_CASE("Check for kwk::iota(out, value) 1D")
{
  int data[2];
  int vdata[2] = {1,2};

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};
  
  kwk::iota(d, 1);

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
  
  kwk::iota(d,2);

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
  
  kwk::iota(d,3);

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
  
  kwk::iota(d,4);

  TTS_ALL_EQUAL(data, vdata);
};

TTS_CASE("Check for kwk::iota(out, value, step) 1D")
{
  int data[2];
  int vdata[2] = {0,10};

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};
  
  kwk::iota(d, 0, 10);

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
  
  kwk::iota(d,0,20);

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
  
  kwk::iota(d,0,30);

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
  
  kwk::iota(d,0,40);

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
  
  kwk::iota(d,0.f,40.5f);

  TTS_ALL_EQUAL(data, vdata);
};