#include "kmeans1D.hpp"

int main(int argc, char* argv[]) {
  // Input points
  ParamArg param;

  parseArguments(argc, argv, &param);

  run<float>(param);
  run<double>(param);

  return 0;
}