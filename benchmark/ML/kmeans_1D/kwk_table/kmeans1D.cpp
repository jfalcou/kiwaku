#include "kmeans1D.hpp"

int main(int argc, char* argv[]) {
  // Input points
  ParamArg param;

  parseArguments(argc, argv, &param);

  if(param.benchmark){
    std::cout << "Bench Float \n";
    runBench<float>(param);
    std::cout << "Bench Double \n";
    runBench<double>(param);
  } else {
    std::cout << "Test Float \n";
    runTest<float>(param);
    std::cout << "Test Double \n";
    runTest<double>(param);
  }
  return 0;
}