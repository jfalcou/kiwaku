//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <cstdlib>
#include <kwk/algorithm/algos/binary_search.hpp>
#include <kwk/container.hpp>
#include "test.hpp"

TTS_CASE("Check for kwk::lower_bound(In, value) 1D")
{
  int data[2];
  int vdata[] = {1};

  fill_data(data, kwk::of_size(2), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

  auto res = kwk::lower_bound(d, 1);

  if(res)
    TTS_ALL_EQUAL(*res, vdata);
  else
    TTS_EXPECT(res == std::nullopt);
};


TTS_CASE("Check for kwk::lower_bound(In, value) 2D")
{
  int data[2*3];
  int vdata[] = {1,1};

  fill_data(data, kwk::of_size(2,3), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

  auto res = kwk::lower_bound(d, 21);

  if(res)
    TTS_ALL_EQUAL(*res, vdata);
  else
    TTS_EXPECT(res == std::nullopt);
};


TTS_CASE("Check for kwk::lower_bound(In, value) 2D - with CPU context")
{
  int data[2*3];
  int vdata[] = {1,1};

  fill_data(data, kwk::of_size(2,3), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

  auto res = kwk::lower_bound(kwk::cpu, d, 21);

  if(res)
    TTS_ALL_EQUAL(*res, vdata);
  else
    TTS_EXPECT(res == std::nullopt);
};

TTS_CASE("Check for kwk::lower_bound(In, value) 3D")
{
  int data[2*3*4];
  int vdata[] = {1,0,0};

  fill_data(data, kwk::of_size(2,3,4), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

  auto res = kwk::lower_bound(d, 100);

  if(res)
    TTS_ALL_EQUAL(*res, vdata);
  else
    TTS_EXPECT(res == std::nullopt);
};

TTS_CASE("Check for kwk::lower_bound(In, value) 4D")
{
  int data[2*3*4*5];
  int vdata[] = {1,0,0,0};

  fill_data(data, kwk::of_size(2,3,4,5), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

  auto res = kwk::lower_bound(d, 1000);

  if(res)
    TTS_ALL_EQUAL(*res, vdata);
  else
    TTS_EXPECT(res == std::nullopt);
};

TTS_CASE("Check for kwk::lower_bound(In, value, func) 1D")
{
  int data[2];
  int vdata[] = {0};

  fill_data(data, kwk::of_size(2), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

  auto res = kwk::lower_bound(d, 1, [](auto e, auto s)
  {
    return e < (s-1);
  });

  if(res)
    TTS_ALL_EQUAL(*res, vdata);
  else
    TTS_EXPECT(res == std::nullopt);
};


TTS_CASE("Check for kwk::lower_bound(In, value, func) 2D")
{
  int data[2*3];
  int vdata[] = {1,0};

  fill_data(data, kwk::of_size(2,3), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

  auto res = kwk::lower_bound(d, 21, [](auto e, auto s)
  {
    return e < (s-1);
  });

  if(res)
    TTS_ALL_EQUAL(*res, vdata);
  else
    TTS_EXPECT(res == std::nullopt);
};

TTS_CASE("Check for kwk::lower_bound(In, value, func) 3D")
{
  int data[2*3*4];
  int vdata[] = {1,0,0};

  fill_data(data, kwk::of_size(2,3,4), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

  auto res = kwk::lower_bound(d, 101, [](auto e, auto s)
  {
    return e < (s-1);
  });

  if(res)
    TTS_ALL_EQUAL(*res, vdata);
  else
    TTS_EXPECT(res == std::nullopt);
};

TTS_CASE("Check for kwk::lower_bound(In, value, func) 3D - with CPU context")
{
  int data[2*3*4];
  int vdata[] = {1,0,0};

  fill_data(data, kwk::of_size(2,3,4), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

  auto res = kwk::lower_bound(kwk::cpu, d, 101, [](auto e, auto s)
  {
    return e < (s-1);
  });

  if(res)
    TTS_ALL_EQUAL(*res, vdata);
  else
    TTS_EXPECT(res == std::nullopt);
};

TTS_CASE("Check for kwk::lower_bound(In, value, func) 4D")
{
  int data[2*3*4*5];
  int vdata[] = {1,0,0,0};

  fill_data(data, kwk::of_size(2,3,4,5), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

  auto res = kwk::lower_bound(d, 1001, [](auto e, auto s)
  {
    return e < (s-1);
  });

  if(res)
    TTS_ALL_EQUAL(*res, vdata);
  else
    TTS_EXPECT(res == std::nullopt);
};


TTS_CASE("Check for kwk::upper_bound(In, value) 1D")
{
  int data[2];
  int vdata[] = {1};

  fill_data(data, kwk::of_size(2), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

  auto res = kwk::upper_bound(d, 0);

  if(res)
    TTS_ALL_EQUAL(*res, vdata);
  else
    TTS_EXPECT(res == std::nullopt);
};


TTS_CASE("Check for kwk::upper_bound(In, value) 2D")
{
  int data[2*3];
  int vdata[] = {1,1};

  const std::vector<int> rdata {0, 1, 2, 10, 11, 12};

  fill_data(data, kwk::of_size(2,3), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

  auto res = kwk::upper_bound(d, 10);

  if(res)
    TTS_ALL_EQUAL(*res, vdata);
  else
    TTS_EXPECT(res == std::nullopt);
};

TTS_CASE("Check for kwk::upper_bound(In, value) 3D")
{
  int data[2*3*4];
  int vdata[] = {1,0,0};

  fill_data(data, kwk::of_size(2,3,4), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

  auto res = kwk::upper_bound(d, 23);

  if(res)
    TTS_ALL_EQUAL(*res, vdata);
  else
    TTS_EXPECT(res == std::nullopt);
};

TTS_CASE("Check for kwk::upper_bound(In, value) 4D")
{
  int data[2*3*4*5];
  int vdata[] = {1,0,0,0};

  fill_data(data, kwk::of_size(2,3,4,5), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

  auto res = kwk::upper_bound(d, 234);

  if(res)
    TTS_ALL_EQUAL(*res, vdata);
  else
    TTS_EXPECT(res == std::nullopt);
};

TTS_CASE("Check for kwk::upper_bound(In, value) 4D - with CPU context")
{
  int data[2*3*4*5];
  int vdata[] = {1,0,0,0};

  fill_data(data, kwk::of_size(2,3,4,5), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

  auto res = kwk::upper_bound(kwk::cpu, d, 234);

  if(res)
    TTS_ALL_EQUAL(*res, vdata);
  else
    TTS_EXPECT(res == std::nullopt);
};

TTS_CASE("Check for kwk::upper_bound(In, value, func) 1D")
{
  int data[2];
  int vdata[] = {1};

  fill_data(data, kwk::of_size(2), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

  auto res = kwk::upper_bound(d, 0, [](auto e, auto s)
  {
    return e > (s+1);
  });

  if(res)
    TTS_ALL_EQUAL(*res, vdata);
  else
    TTS_EXPECT(res == std::nullopt);
};


TTS_CASE("Check for kwk::upper_bound(In, value, func) 2D")
{
  int data[2*3];
  int vdata[] = {1,1};

  const std::vector<int> rdata {0, 1, 2, 10, 11, 12};

  fill_data(data, kwk::of_size(2,3), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

  auto res = kwk::upper_bound(d, 9, [](auto e, auto s)
  {
    return e > (s+1);
  });

  if(res)
    TTS_ALL_EQUAL(*res, vdata);
  else
    TTS_EXPECT(res == std::nullopt);
};


TTS_CASE("Check for kwk::upper_bound(In, value, func) 2D - with CPU context")
{
  int data[2*3];
  int vdata[] = {1,1};

  const std::vector<int> rdata {0, 1, 2, 10, 11, 12};

  fill_data(data, kwk::of_size(2,3), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

  auto res = kwk::upper_bound(kwk::cpu, d, 9, [](auto e, auto s)
  {
    return e > (s+1);
  });

  if(res)
    TTS_ALL_EQUAL(*res, vdata);
  else
    TTS_EXPECT(res == std::nullopt);
};

TTS_CASE("Check for kwk::upper_bound(In, value, func) 3D")
{
  int data[2*3*4];
  int vdata[] = {1,0,0};

  fill_data(data, kwk::of_size(2,3,4), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

  auto res = kwk::upper_bound(d, 22, [](auto e, auto s)
  {
    return e > (s+1);
  });

  if(res)
    TTS_ALL_EQUAL(*res, vdata);
  else
    TTS_EXPECT(res == std::nullopt);
};

TTS_CASE("Check for kwk::upper_bound(In, value, func) 4D")
{
  int data[2*3*4*5];
  int vdata[] = {1,0,0,0};

  fill_data(data, kwk::of_size(2,3,4,5), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

  auto res = kwk::upper_bound(d, 233, [](auto e, auto s)
  {
    return e > (s+1);
  });

  if(res)
    TTS_ALL_EQUAL(*res, vdata);
  else
    TTS_EXPECT(res == std::nullopt);
};


TTS_CASE("Check for kwk::binary_search(In, value) 1D")
{
  int data[2];
  int needles[] = {0,1,2};

  std::array<bool,3> res;
  std::array<bool,3> vres {true, true, false};

  fill_data(data, kwk::of_size(2), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

  size_t i = 0;
  for (auto needle : needles)
    if(kwk::binary_search(d, needle))
      res[i++] = true;
    else
      res[i++] = false;

  TTS_ALL_EQUAL(res, vres);
};


TTS_CASE("Check for kwk::binary_search(In, value) 1D - with CPU context")
{
  int data[2];
  int needles[] = {0,1,2};

  std::array<bool,3> res;
  std::array<bool,3> vres {true, true, false};

  fill_data(data, kwk::of_size(2), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

  size_t i = 0;
  for (auto needle : needles)
    if(kwk::binary_search(kwk::cpu, d, needle))
      res[i++] = true;
    else
      res[i++] = false;

  TTS_ALL_EQUAL(res, vres);
};


TTS_CASE("Check for kwk::binary_search(In, value) 2D")
{
  int data[2*3];
  int needles[] = {0,11,21};

  std::array<bool,3> res;
  std::array<bool,3> vres {true, true, false};

  fill_data(data, kwk::of_size(2,3), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

  size_t i = 0;
  for (auto needle : needles)
    if(kwk::binary_search(d, needle))
      res[i++] = true;
    else
      res[i++] = false;

  TTS_ALL_EQUAL(res, vres);
};

TTS_CASE("Check for kwk::binary_search(In, value) 3D")
{
  int data[2*3*4];
  int needles[] = {0,111,211};

  std::array<bool,3> res;
  std::array<bool,3> vres {true, true, false};

  fill_data(data, kwk::of_size(2,3,4), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

  size_t i = 0;
  for (auto needle : needles)
    if(kwk::binary_search(d, needle))
      res[i++] = true;
    else
      res[i++] = false;

  TTS_ALL_EQUAL(res, vres);
};

TTS_CASE("Check for kwk::binary_search(In, value) 4D")
{
  int data[2*3*4*5];
  int needles[] = {0,1111,2111};

  std::array<bool,3> res;
  std::array<bool,3> vres {true, true, false};

  fill_data(data, kwk::of_size(2,3,4,5), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

  size_t i = 0;
  for (auto needle : needles)
    if(kwk::binary_search(d, needle))
      res[i++] = true;
    else
      res[i++] = false;

  TTS_ALL_EQUAL(res, vres);
};

TTS_CASE("Check for kwk::binary_search(In, value, func) 1D")
{
  int data[2];
  int needles[] = {0,1,2};

  std::array<bool,3> res;
  std::array<bool,3> vres {true, true, false};

  fill_data(data, kwk::of_size(2), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

  size_t i = 0;
  for (auto needle : needles)
    if(kwk::binary_search(d, needle, [](auto e, auto s){return e < s;}))
      res[i++] = true;
    else
      res[i++] = false;

  TTS_ALL_EQUAL(res, vres);
};


TTS_CASE("Check for kwk::binary_search(In, value, func) 2D")
{
  int data[2*3];
  int needles[] = {0,11,21};

  std::array<bool,3> res;
  std::array<bool,3> vres {true, true, false};

  fill_data(data, kwk::of_size(2,3), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

  size_t i = 0;
  for (auto needle : needles)
    if(kwk::binary_search(d, needle, [](auto e, auto s){return e < s;}))
      res[i++] = true;
    else
      res[i++] = false;

  TTS_ALL_EQUAL(res, vres);
};


TTS_CASE("Check for kwk::binary_search(In, value, func) 2D - with CPU context")
{
  int data[2*3];
  int needles[] = {0,11,21};

  std::array<bool,3> res;
  std::array<bool,3> vres {true, true, false};

  fill_data(data, kwk::of_size(2,3), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

  size_t i = 0;
  for (auto needle : needles)
    if(kwk::binary_search(kwk::cpu, d, needle, [](auto e, auto s){return e < s;}))
      res[i++] = true;
    else
      res[i++] = false;

  TTS_ALL_EQUAL(res, vres);
};

TTS_CASE("Check for kwk::binary_search(In, value, func) 3D")
{
  int data[2*3*4];
  int needles[] = {0,111,211};

  std::array<bool,3> res;
  std::array<bool,3> vres {true, true, false};

  fill_data(data, kwk::of_size(2,3,4), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

  size_t i = 0;
  for (auto needle : needles)
    if(kwk::binary_search(d, needle, [](auto e, auto s){return e < s;}))
      res[i++] = true;
    else
      res[i++] = false;

  TTS_ALL_EQUAL(res, vres);
};

TTS_CASE("Check for kwk::binary_search(In, value, func) 4D")
{
  int data[2*3*4*5];
  int needles[] = {0,1111,2111};

  std::array<bool,3> res;
  std::array<bool,3> vres {true, true, false};

  fill_data(data, kwk::of_size(2,3,4,5), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

  size_t i = 0;
  for (auto needle : needles)
    if(kwk::binary_search(d, needle, [](auto e, auto s){return e < s;}))
      res[i++] = true;
    else
      res[i++] = false;

  TTS_ALL_EQUAL(res, vres);
};