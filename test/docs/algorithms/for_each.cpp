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
  int data[] = {3, -4, 2, -8, 15, 267};
  auto v = kwk::view{ kwk::source = data };

  auto print = [](const int& n) { std::cout << n << ' '; };

  std::cout << "Before:\t";
  kwk::for_each(print, v);
  std::cout << '\n';

  // increment elements in-place
  kwk::for_each( [](int &n){ n++; }, v);

  std::cout << "After:\t";
  kwk::for_each(print, v);
  std::cout << '\n';

  struct Sum
  {
    void operator()(int n) { sum += n; }
    int sum{0};
  };

  // invoke Sum::operator() for each element
  Sum s = kwk::for_each(Sum{}, v);
  std::cout << "sum:\t" << s.sum << '\n';
}
