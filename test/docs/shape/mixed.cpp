//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#include <kiwaku/kwk.hpp>
#include <iostream>

int main()
{
  // _ is the dimension specifier
  using kwk::_;

  // Defines a 3D shape type with dimension 2 fixed to 5
  using shape_t = kwk::shape< kwk::extent()[5]() >;

  // Constructs such a shape with dim0 = 7 and dim2 = 9
  shape_t x( _[0] = 7, _[2] = 9);
  std::cout << x << "\n";

  // Dimensions specifiers are position independent
  shape_t y( _[2] = 9, _[0] = 7 );
  std::cout << y << "\n";

  // Missing dimensions are filled to be coherent with default constructor
  // w is a 11 x 5 x 1 shape
  shape_t w( _[0] = 11 );
  std::cout << w << "\n";

  // z is a 0 x 5 x 9 shape
  shape_t z( _[2] = 9 );
  std::cout << z << "\n";
}
