//==================================================================================================
/*
    KIWAKU - Containers Well Made
    Copyright : KIWAKU Contributors & Maintainers
    SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <iostream>
#include <kwk/kwk.hpp>

int main()
{
  // x will contains 160 x 2
  kwk::shape< kwk::_2D > x(kwk::of_size(4, 8, 5, 2));
  std::cout << x << "\n";

  // y will contains 1 x 1 x 7
  kwk::shape< kwk::_3D > y(kwk::of_size(7));
  std::cout << y << "\n";
}
