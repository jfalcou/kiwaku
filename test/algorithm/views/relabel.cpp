//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <cstdlib>
#include <kwk/algorithm/views/relabel.hpp>
#include <kwk/container.hpp>
#include "test.hpp"

TTS_CASE("Check for kwk::relabel(in, name) 1D")
{
  int data[2];

  fill_data(data, kwk::of_size(2), false);

  auto d = kwk::view{kwk::source = data, kwk::label = "data"};
  auto e = kwk::relabel(d, "vdata");

  TTS_EQUAL(e.label(), "vdata");
};

TTS_CASE("Check for kwk::relabel(in, name) 2D")
{
  int data[2*3];

  fill_data(data, kwk::of_size(2,3), false);

  auto d = kwk::view{kwk::source = data, kwk::label = "data"};
  auto e = kwk::relabel(d, "vdata");

  TTS_EQUAL(e.label(), "vdata");
};

TTS_CASE("Check for kwk::relabel(in, name) 3D")
{
  int data[2*3*4];

  fill_data(data, kwk::of_size(2,3,4), false);

  auto d = kwk::view{kwk::source = data, kwk::label = "data"};
  auto e = kwk::relabel(d, "vdata");

  TTS_EQUAL(e.label(), "vdata");
};

TTS_CASE("Check for kwk::relabel(in, name) 4D")
{
  int data[2*3*4*5];

  fill_data(data, kwk::of_size(2,3,4,5), false);

  auto d = kwk::view{kwk::source = data, kwk::label = "data"};
  auto e = kwk::relabel(d, "vdata");

  TTS_EQUAL(e.label(), "vdata");
};