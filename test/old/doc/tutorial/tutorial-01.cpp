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
  // 1/ defines some data in memory
  std::vector<double> data = {1.2,3.4,5.6,7.8,9.10,11.12};

  // 2/ take a view over data as a 2 x 3 array
  auto v = kwk::view{ kwk::source = data        // select the data source
                    , kwk::of_size(2,3)         // specify the extents of the view
                    };

  std::cout << v << "\n";
}
