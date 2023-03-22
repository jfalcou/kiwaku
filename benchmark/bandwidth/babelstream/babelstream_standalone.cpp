#include "babelstream_standalone.hpp"

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

uint32_t cpufreq()
{
    uint32_t cpuFreq = 0;

    // CPU frequency is stored in /proc/cpuinfo in lines beginning with "cpu MHz"
    std::string pattern = "^cpu MHz\\s*:\\s*(\\d+)";
    const char* pcreErrorStr = NULL;
    int pcreErrorOffset = 0;

    pcre* reCompiled = pcre_compile(pattern.c_str(), PCRE_ANCHORED, &pcreErrorStr,
                                    &pcreErrorOffset, NULL);
    if(reCompiled == NULL)
    {
	return 0;
    }

    std::ifstream ifs("/proc/cpuinfo");
    if(ifs.is_open())
    {	
	std::string line;
	int results[10];

	while(ifs.good())
	{
	    getline(ifs, line);
	    int rc = pcre_exec(reCompiled, 0, line.c_str(), line.length(), 
                               0, 0, results, 10);
	    if(rc < 0)
		continue;

	    // Match found - extract frequency
	    const char* matchStr = NULL;
	    pcre_get_substring(line.c_str(), results, rc, 1, &(matchStr));
	    cpuFreq = atol(matchStr);
	    pcre_free_substring(matchStr);
	    break;
	}
    }
    
    ifs.close();
    pcre_free(reCompiled);

    return cpuFreq;
}


int main(int argc, char *argv[])
{
  parseArguments(argc, argv);
  run<float>();
  run<double>();
  std::cout << "Frequence CPU : " << cpufreq() << " Hz" << std::endl;
}