#include "babelstream_std.hpp"

int main(int argc, char *argv[])
{
  parseArguments(argc, argv);

  if(BENCHMARK)
  {
    Benchmarking<int>();
    Benchmarking<float>();
    Benchmarking<double>();
  } 
  else 
  {
    run<int>();
    run<float>();
    run<double>();
  }
}