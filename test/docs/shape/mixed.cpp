//==================================================================================================
/*
    KIWAKU - Containers Well Made
    Copyright : KIWAKU Contributors & Maintainers
    SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <kwk/kwk.hpp>
#include <iostream>

int main()
{
  using namespace kwk::literals;

  // _ is the dimension specifier
  using kwk::_;

  // Constructs shape of 3 x 5 x 9
  kwk::shape x(3, 5, 9);
  std::cout << x << " : " << get<0>(x) << " x " << get<1>(x) << " x " << get<2>(x) << "\n";

  // Constructs shape with width = 7 and height = 9 in this order
  kwk::shape y(kwk::width = 7, kwk::height = 9);
  std::cout << y << " : " << y[kwk::width] << " x " << y[kwk::height] << "\n";

  // Constructs shape with 4 x color = 10 with 4 being known at compile time
  kwk::shape z(4_c, kwk::axis<"color"> = 9);
  std::cout << z << " : " << get<0>(z) << " x " << z[kwk::axis<"color">] << "\n";
}
