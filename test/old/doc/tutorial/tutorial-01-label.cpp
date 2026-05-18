/**
    KIWAKU - Containers Well Made
    Copyright : KIWAKU Contributors & Maintainers
    SPDX-License-Identifier: BSL-1.0
**/
#include <kwk/kwk.hpp>
#include <iostream>
#include <vector>

int main()
{
  std::vector<double> data = {1.2,3.4,5.6,7.8,9.10,11.12};

  auto v = kwk::view{ kwk::source = data
                    , kwk::label = "The Original Data"
                    , kwk::of_size(2,3)
                    };

  std::cout << v << "\n";
}
