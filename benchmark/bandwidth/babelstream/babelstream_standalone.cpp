#include "babelstream_standalone.hpp"
#include <math.h>

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

void parseArguments(int argc, char *argv[])
{
  for (int i = 1; i < argc; i++)
  {
    if (!std::string("--arraysize").compare(argv[i]) ||
        !std::string("-s").compare(argv[i]))
    {
      if (++i >= argc || !parseInt(argv[i], &ARRAY_SIZE) || ARRAY_SIZE <= 0)
      {
        std::cerr << "Invalid array size." << std::endl;
        exit(EXIT_FAILURE);
      }
    }
    else if (!std::string("--freq").compare(argv[i]) ||
             !std::string("-f").compare(argv[i]))
    {
      if (++i >= argc || !parseInt(argv[i], &Freq_CPU) || ARRAY_SIZE <= 0)
      {
        std::cerr << "Invalid frequency." << std::endl;
        exit(EXIT_FAILURE);
      }
    }
    else if (!std::string("--numtimes").compare(argv[i]) ||
             !std::string("-n").compare(argv[i]))
    {
      if (++i >= argc || !parseUInt(argv[i], &num_times))
      {
        std::cerr << "Invalid number of times." << std::endl;
        exit(EXIT_FAILURE);
      }
      if (num_times < 2)
      {
        std::cerr << "Number of times must be 2 or more" << std::endl;
        exit(EXIT_FAILURE);
      }
    }
    else if (!std::string("--help").compare(argv[i]) ||
             !std::string("-h").compare(argv[i]))
    {
      std::cout << std::endl;
      std::cout << "Usage: " << argv[0] << " [OPTIONS]" << std::endl
                << std::endl;
      std::cout << "Options:" << std::endl;
      std::cout << "  -h  --help               Print the message" << std::endl;
      std::cout << "  -s  --arraysize  SIZE    Use SIZE elements in the array" << std::endl;
      std::cout << "  -n  --numtimes   NUM     Run the test NUM times (NUM >= 2)" << std::endl;
      std::cout << "  -f  --freq   freq     CPU freq in MHz" << std::endl;
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

int main(int argc, char *argv[])
{
  parseArguments(argc, argv);

  myresults.open("Benchmark_float.csv");
  myresults << "Function;Size(Bytes);Mean Babel(GBytes/sec);Mean Nano(GBytes/sec);Median Nano(GBytes/sec);Min Nano(GBytes/sec);Max Nano(GBytes/sec);Err Nano(GBytes/sec);\n";

  for(long long s = 128;  s<pow(2, 25); s=round(s*1.5)){
    ARRAY_SIZE = s;
    run<float>();
  }
  myresults.close();

  myresults.open("Benchmark_double.csv");
  myresults << "Function;Size(Bytes);Mean Babel(GBytes/sec);Mean Nano(GBytes/sec);Median Nano(GBytes/sec);Min Nano(GBytes/sec);Max Nano(GBytes/sec);Err Nano(GBytes/sec);\n";

  for(long long s = 128;  s<pow(2, 25); s=round(s*1.5)){
    ARRAY_SIZE = s;
    run<double>();
  }
  myresults.close();
}