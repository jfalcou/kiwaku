/**
    KIWAKU - Containers Well Made
    Copyright : KIWAKU Contributors & Maintainers
    SPDX-License-Identifier: BSL-1.0
**/
#include <kwk/kwk.hpp>
#include <iostream>

int main()
{
  using kwk::_;

  //====================================================================================================================
  // Named extents
  //====================================================================================================================
  kwk::shape<kwk::width, kwk::height>   s1(kwk::height = 8, kwk::width = 10);
  kwk::shape<kwk::width, kwk::height>   s2(10,8);
  kwk::shape<_, kwk::axis<"phase">, _>  s3(11, kwk::axis<"phase"> = 10, 15);

  // Uncomment for a compiler error
  // kwk::shape<_, kwk::axis<"phase">, _>  s4(11, 15, kwk::axis<"phase"> = 10);

  std::cout << s1 << "\n" << s2 << "\n" << s3 << "\n\n";

  //====================================================================================================================
  // Named extents with static size
  //====================================================================================================================
  kwk::shape<kwk::depth[66], kwk::width[23]>  s4;
  kwk::shape<kwk::width, kwk::height[23]>     s5(9,_);
  kwk::shape<kwk::depth[66], kwk::width[23]>  s6(66,23);

  // Uncomment for dynamic assert
  // kwk::shape<kwk::depth[66], kwk::width[23]>  s7(6,32);

  std::cout << "Sizeof of s4: " << sizeof(s4) << "\n";
  std::cout << s4 << "\n" << s5 << "\n" << s6 << "\n\n";

  //====================================================================================================================
  // Named extents with type specifier
  //====================================================================================================================
  kwk::shape<kwk::axis<"count">[kwk::int8]> s7(kwk::axis<"count"> = 100);

  std::cout << "Size of s7: " << sizeof(s7) << "\n";
  std::cout << s7 << "\n";
}
