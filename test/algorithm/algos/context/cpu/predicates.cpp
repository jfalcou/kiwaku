//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/algorithm/algos/predicates.hpp>
#include <kwk/container.hpp>
#include "test.hpp"
#include "../generic/predicates.hpp"

// TODO: update these tests

TTS_CASE("Check for kwk::all_of(in, func) 1D")
{
  kwk::test::all_of_in_func_1D(kwk::cpu);
};

TTS_CASE("Check for kwk::all_of(in, func) 2D")
{
  kwk::test::all_of_in_func_2D(kwk::cpu);
};

TTS_CASE("Check for kwk::all_of(in, func) 3D")
{
  kwk::test::all_of_in_func_3D(kwk::cpu);
};

TTS_CASE("Check for kwk::all_of(in, func) 4D")
{
  kwk::test::all_of_in_func_4D(kwk::cpu);
};

TTS_CASE("Check for kwk::all_of(in, func) False")
{
  kwk::test::all_of_in_func_false(kwk::cpu);
};


TTS_CASE("Check for kwk::any_of(in, func) 1D")
{
  kwk::test::any_of_in_func_1D(kwk::cpu);
};

TTS_CASE("Check for kwk::any_of(in, func) 2D")
{
  kwk::test::any_of_in_func_2D(kwk::cpu);
};

TTS_CASE("Check for kwk::any_of(in, func) 3D")
{
  kwk::test::any_of_in_func_3D(kwk::cpu);
};

TTS_CASE("Check for kwk::any_of(in, func) 4D")
{
  kwk::test::any_of_in_func_4D(kwk::cpu);
};

TTS_CASE("Check for kwk::any_of(in, func) False")
{
  kwk::test::any_of_in_func_false(kwk::cpu);
};

TTS_CASE("Check for kwk::none_of(in, func) 1D")
{
  kwk::test::none_of_in_func_1D(kwk::cpu);
};

TTS_CASE("Check for kwk::none_of(in, func) 2D")
{
  kwk::test::none_of_in_func_2D(kwk::cpu);
};

TTS_CASE("Check for kwk::none_of(in, func) 3D")
{
  kwk::test::none_of_in_func_3D(kwk::cpu);
};

TTS_CASE("Check for kwk::none_of(in, func) 4D")
{
  kwk::test::none_of_in_func_4D(kwk::cpu);
};

TTS_CASE("Check for kwk::none_of(in, func) False")
{
  kwk::test::none_of_in_func_false(kwk::cpu);
};

TTS_CASE("Check for kwk::count(in, value) 1D")
{
  kwk::test::count_in_value_1D(kwk::cpu);
};

TTS_CASE("Check for kwk::count(in, value) 2D")
{
  kwk::test::count_in_value_2D(kwk::cpu);
};

TTS_CASE("Check for kwk::count(in, value) 3D")
{
  kwk::test::count_in_value_3D(kwk::cpu);
};

TTS_CASE("Check for kwk::count(in, value) 4D")
{
  kwk::test::count_in_value_4D(kwk::cpu);
};

TTS_CASE("Check for kwk::count_if(in, func) 1D")
{
  kwk::test::count_if_in_func_1D(kwk::cpu);
};

TTS_CASE("Check for kwk::count_if(in, func) 2D")
{
  kwk::test::count_if_in_func_2D(kwk::cpu);
};

TTS_CASE("Check for kwk::count_if(in, func) 3D")
{
  kwk::test::count_if_in_func_3D(kwk::cpu);
};

TTS_CASE("Check for kwk::count_if(in, func) 4D")
{
  kwk::test::count_if_in_func_4D(kwk::cpu);
};