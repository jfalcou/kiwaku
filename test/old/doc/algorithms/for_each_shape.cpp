//!=================================================================================================
/**
    KIWAKU - Containers Well Made
    Copyright : KIWAKU Contributors & Maintainers
    SPDX-License-Identifier: BSL-1.0
**/
//!=================================================================================================
#include <iostream>
#include <kwk/kwk.hpp>

int main()
{
  auto print  = [](auto p2, auto p1, auto p0)
                {
                  std::cout << p2 << ' ' << p1 << ' ' << p0 << '\n';
                  if(p1==2 && p0==1) std::cout << '\n';
                };

  kwk::for_each(print, kwk::of_size(4,3,2));
}
