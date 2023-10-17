#pragma once

#include <iostream>
#include <chrono>
#include <ctime>
 
namespace bench
{
  struct metrics_t
  {
    double AccessCount, AccessRate, LoadBytes, LoadBandwidth, ArithmeticIntensity, Time;
    void print()
    {
      std::cout
          << "Time="                << Time << " "
          << "AccessCount="         << AccessCount << " "
          << "AccessRate="          << AccessRate << " "
          << "ArithmeticIntensity=" << ArithmeticIntensity << " "
          << "LoadBandwidth="       << LoadBandwidth << " "
          << "LoadBytes="           << LoadBytes << "\n";

      // 72M AccessRate=20.8765M/s ArithmeticIntensity=1.75 LoadBandwidth=250.519M/s LoadBytes=201.327M
    }
  };

  struct chrono_t
  {
    void Init()
    {
      elapsed_ = 0;
      ResumeTiming();
    }

    void PauseTiming()
    {
      auto now = std::chrono::system_clock::now();
      std::chrono::duration<double> e = now - last_point_;
      elapsed_ += e.count();
    }

    void ResumeTiming()
    {
      last_point_ = std::chrono::system_clock::now();
    }
    
    double ElapsedTime()
    {
      PauseTiming();
      ResumeTiming();
      return elapsed_;
    }

    std::size_t ElapsedTimeMS()
    {
      PauseTiming();
      ResumeTiming();
      return elapsed_ * 1000;
    }

    std::string Str()
    {
      // long t = static_cast<long>(elapsed_);
      // std::string res =
      //     std::to_string(t)
      //   + "." + std::to_string(elapsed_ - t);
      std::string res = std::to_string(elapsed_) + "s";
      return res;
    }

  private:
    std::chrono::time_point<std::chrono::system_clock> last_point_;
    double elapsed_;
  };
}
