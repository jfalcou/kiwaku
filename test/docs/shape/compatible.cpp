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
  // Defines a 3D shape type with dimension 2 fixed to 3
  using shape_t = kwk::shape< kwk::extent()[3]() >;

  // 4 x fixed<3> x 8 is compatible
  shape_t x = kwk::of_size(4, kwk::fixed<3>, 8);
  std::cout << x << "\n";

  // 4 x fixed<3> x fixed<5> is compatible
  shape_t y = kwk::of_size(4, kwk::fixed<3>, kwk::fixed<5>);
  std::cout << y << "\n";

  // 4 x 3 x fixed<5> is incompatible
  // shape_t z = kwk::of_size(4, 3, kwk::fixed<5>);
}
