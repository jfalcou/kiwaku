#include <kwk/container.hpp>
#include <iostream>

int main()
{
  float ref[] = {1,2,3,4,5,6,7,8,9,10,11,12};

  auto v = kwk::view{ kwk::source = ref
                    , kwk::of_size(4,3)
                    , kwk::label = "Test matrix"
                    };

  std::cout << v << "\n";
}
