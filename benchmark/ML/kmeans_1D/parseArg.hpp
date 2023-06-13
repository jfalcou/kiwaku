#pragma once
#include <ios>
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <cstring>
#include <cstdint>
#include <unistd.h>

struct ParamArg
{
  unsigned int array_size = 100;
  unsigned int Freq_CPU = 1600;
  unsigned int num_times = 2;
  unsigned int seed = 1618;
  bool benchmark = false;
};

int parseUInt(const char *str, unsigned int *output);
int parseInt(const char *str, int *output);
void parseArguments(int argc, char *argv[], ParamArg* p);