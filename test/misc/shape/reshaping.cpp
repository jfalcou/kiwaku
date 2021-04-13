//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include "test.hpp"
#include <kiwaku/shape.hpp>
#include <kiwaku/misc/indexer.hpp>

TTS_CASE( "Computation of sub-shapes")
{
  auto s = kwk::of_shape(10,20,30);
  using kwk::_;
  using kwk::along;

  std::cout << s(_) << "\n";
  std::cout << s(_,_) << "\n";
  std::cout << s(_,_,_) << "\n";
  std::cout << "\n";

  std::cout << s(_(0,5)) << "\n";
  std::cout << s(_(0,5),_(0,5)) << "\n";
  std::cout << s(_(0,5),_(10,19),_(0,29)) << "\n";
  std::cout << "\n";

  std::cout << s(_(0,5,599)) << "\n";
  std::cout << s(_(0,5,9),_(0,5,59)) << "\n";
  std::cout << s(_(0,3,9),_(0,3,19),_(0,3,29)) << "\n";
  std::cout << "\n";
}

