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
#include <tts/tests/relation.hpp>
#include <tts/tests/basic.hpp>

TTS_CASE( "Experimental Reverdy's shape" )
{
  constexpr auto  s = kwk::extent[4][7][6];
  kwk::shape<s> sh;
  auto sh2 = kwk::of_shape(4,5,6,1,1,1,1,1,7,1);
  kwk::shape sh3 = {4,4,4};
  kwk::shape<kwk::_4D> sh4 = sh;
  kwk::shape<kwk::_1D> sh5{sh3};

  std::cout << "sh3 == sh : "  << std::boolalpha << (sh3 == sh) << std::endl;
  std::cout << "sh3 != sh   : "  << std::boolalpha << (sh3 != sh) << std::endl;
  std::cout << "sh3 contains sh   : "  << std::boolalpha << sh3.contains(sh) << std::endl;

  std::cout << sizeof(sh ) << " " << sh  << " " << sh.numel()  << " " << sh.nbdims()  << "\n";
  std::cout << sizeof(sh2) << " " << sh2 << " " << sh2.numel() << " " << sh2.nbdims() << "\n";
  std::cout << sizeof(sh3) << " " << sh3 << " " << sh3.numel() << " " << sh3.nbdims() << "\n";
  std::cout << sizeof(sh4) << " " << sh4 << " " << sh4.numel() << " " << sh4.nbdims() << "\n";
  std::cout << sizeof(sh5) << " " << sh5 << " " << sh5.numel() << " " << sh5.nbdims() << "\n";

  auto[s0,s1,s2] = sh;
  std::cout << s0 << " " << s1 << " " << s2 << "\n";
}
