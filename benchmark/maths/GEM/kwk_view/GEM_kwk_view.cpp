#include "GEM_kwk_view.hpp"

int main(int argc, char *argv[])
{
  parseArguments(argc, argv);

  if(UNITTEST)
  {
    UnitTest<float>();
    UnitTest<double>();
  }
  else if(BENCHMARK)
  {
    Benchmarking<float>();
    Benchmarking<double>();
  } 
  else 
  {
    SIZE_X = ARRAY_SIZE;
    SIZE_Y = ARRAY_SIZE;
    run<float>();
    run<double>();
  }
}