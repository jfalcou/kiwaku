#include <kwk/kwk.hpp>
#include <fstream>
#include <iostream>
#include <cstdint>

int main(int argc, char *argv[])
{
int size = 2;

using namespace kwk;
  if(argc > 1) 
  {
    size = std::atoi(argv[1]);
    std::ostringstream oss;
    // std::string fname = "./Benchmark_ccl_kwk_table_" + std::to_string(size2) + ".csv";
    std::string fname;
    fname = "./Benchmark_ccl_kwk_table_" + std::to_string(size) + ".csv";
    // res_nano.open(fname);
    // res_nano << "Size(N*N);Density(1/1000);Mean Nano(Cycles);Median Nano(Cycles);Min Nano(Cycles);Max Nano(Cycles);Err Nano(Cycles)\n";
  }
  else
  {
    std::cout << "Size input needed";
    exit(EXIT_FAILURE);
  }

  auto cells        = table{of_size(size*size), as<int> };
  auto equivalences = table{ of_size(size*size/2), as<int> };

  for (int i = 0; i<(size*size); i++)cells(i)=0;

  std::cout << cells;
}