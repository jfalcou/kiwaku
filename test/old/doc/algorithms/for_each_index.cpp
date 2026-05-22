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
  int data[] = {3, 4, 2, 8, 15, 27, 56, 9, 36};
  auto v = kwk::view{ kwk::source = data, kwk::of_size(3,3) };

  auto print  = [](const int& n, auto... ps)
                {
                  std::cout << "( ";
                  ((std::cout << ps << ' '),...);
                  std::cout << ") = " << n << '\n';
                };

  std::cout << "Before:\n";
  kwk::for_each_index(print, v);
  std::cout << '\n';

  // use values and coordinates
  kwk::for_each_index( [](int &n, int i1, int i0) { n = n + (1000*i1+100*i0); }, v);

  std::cout << "After:\n";
  kwk::for_each_index(print, v);
  std::cout << '\n';
}
