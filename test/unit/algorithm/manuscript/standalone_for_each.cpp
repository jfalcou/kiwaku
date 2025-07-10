#include <vector>
#include <algorithm>

int main()
{
  using data_type = float;
  const std::size_t input_size = 20;
  std::vector<data_type> std_data;
  std_data.resize(input_size);

  for (std::size_t i = 0; i < input_size; ++i)
  {
    std_data[i] = i;
  }

  // std::for_each(std_data.data(), std_data.end(), [](float& e) { e = e * 2; } );

  std::for_each(std::begin(std_data.data()), std::end(std_data.data()), [](float& e) { e = e * 2; } );

  return 0;
};