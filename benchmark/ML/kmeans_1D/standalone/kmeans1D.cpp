#include "kmeans1D.hpp"

int main(int argc, char* argv[]) {
  // Input points
  ParamArg param;

  parseArguments(argc, argv, &param);

  if(param.benchmark){
    runBench<float>(param);
    runBench<double>(param);
  } else {
    runTest<float>(param);
    runTest<double>(param);
  }
  return 0;
}