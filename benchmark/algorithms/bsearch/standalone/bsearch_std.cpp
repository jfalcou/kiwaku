#include "bsearch_std.hpp"

int main(int argc, char *argv[])
{
  parseArguments(argc, argv);

  if(BENCHMARK)
  {
    Benchmarking<float>();
    Benchmarking<double>();
  } 
  else 
  {
    run<float>();
    run<double>();
  }
}