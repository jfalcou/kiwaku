//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef TEST_TEST_HPP
#define TEST_TEST_HPP

#define TTS_USE_CUSTOM_DRIVER
#include <tts/tts.hpp>

int main(int argc, char **argv)
{
  std::cout << "[KIWAKU] - Assertions: ";
#ifdef NDEBUG
  std::cout << "Disabled\n";
#else
  std::cout << "Enabled\n";
#endif

  ::tts::env runtime(argc, argv, std::cout);
  return ::tts::run(runtime, ::tts::detail::suite, 0, 0);
}

#endif
