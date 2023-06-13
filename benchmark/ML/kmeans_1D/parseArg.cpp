#include "parseArg.hpp"

int parseUInt(const char *str, unsigned int *output)
{
  char *next;
  *output = strtoul(str, &next, 10);
  return !strlen(next);
}

int parseInt(const char *str, int *output)
{
  char *next;
  *output = strtol(str, &next, 10);
  return !strlen(next);
}

void parseArguments(int argc, char *argv[], ParamArg *p)
{
  for (int i = 1; i < argc; i++)
  {
    if (!std::string("--arraysize").compare(argv[i]) ||
        !std::string("-s").compare(argv[i]))
    {
      if (++i >= argc || !parseUInt(argv[i], &p->array_size) || p->array_size <= 0)
      {
        std::cerr << "Invalid array size." << std::endl;
        exit(EXIT_FAILURE);
      }
    }
    else if (!std::string("--freq").compare(argv[i]) ||
             !std::string("-f").compare(argv[i]))
    {
      if (++i >= argc || !parseUInt(argv[i], &p->Freq_CPU) || p->Freq_CPU <= 0)
      {
        std::cerr << "Invalid frequency." << std::endl;
        exit(EXIT_FAILURE);
      }
    }
    else if (!std::string("--numtimes").compare(argv[i]) ||
             !std::string("-n").compare(argv[i]))
    {
      if (++i >= argc || !parseUInt(argv[i], &p->num_times) || p->num_times < 1)
      {
        std::cerr << "Invalid number of times." << std::endl;
        exit(EXIT_FAILURE);
      }
    }
    else if (!std::string("--seed").compare(argv[i]) ||
        !std::string("-S").compare(argv[i]))
    {
      if (++i >= argc || !parseUInt(argv[i], &p->seed) || p->seed <= 0)
      {
        std::cerr << "Invalid seed." << std::endl;
        exit(EXIT_FAILURE);
      }
    }
    else if (!std::string("--bench").compare(argv[i]) ||
        !std::string("-B").compare(argv[i]))
    {
      p->benchmark = true;
      std::cout << "BENCHMARK MODE" << std::endl;
      sleep(1);
    }
    else if (!std::string("--help").compare(argv[i]) ||
             !std::string("-h").compare(argv[i]))
    {
      std::cout << std::endl;
      std::cout << "Usage: " << argv[0] << " [OPTIONS]" << std::endl
                << std::endl;
      std::cout << "Options:" << std::endl;
      std::cout << "  -h  --help                Print the message" << std::endl;
      std::cout << "  -s  --arraysize   SIZE    Use SIZE elements in the array (0 for bench mode)" << std::endl;
      std::cout << "  -n  --numtimes    NUM     Run the test NUM times (NUM >= 1)" << std::endl;
      std::cout << "  -f  --freq        freq    CPU freq in MHz" << std::endl;
      std::cout << "  -S  --seed        seed    Random seed" << std::endl;
      std::cout << "  -B  --bench       bench   Benchmark mode" << std::endl;
      std::cout << std::endl;
      exit(EXIT_SUCCESS);
    }
    else
    {
      std::cerr << "Unrecognized argument '" << argv[i] << "' (try '--help')"
                << std::endl;
      exit(EXIT_FAILURE);
    }
  }
}