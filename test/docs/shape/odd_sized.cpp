//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#include <iostream>
#include <kwk/kwk.hpp>

int main()
{
  // x will contains 4 x 32
  kwk::shape< kwk::_2D > x(kwk::of_size(4, 8, 2, 2));
  std::cout << x << "\n";

  // y will contains 7 x 1 x 1
  kwk::shape< kwk::_3D > y = kwk::of_size(7);
  std::cout << y << "\n";
}
