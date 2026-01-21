//======================================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
#pragma once

#define TTS_MAIN
#define TTS_CUSTOM_DRIVER_FUNCTION kwk_entry_point
#include <tts/tts.hpp>

int main(int argc, char const** argv)
{
  printf("[Kiwaku] - Assertions: ");
#ifdef NDEBUG
  printf("Disabled\n");
#else
  printf("Enabled\n");
#endif

  kwk_entry_point(argc, argv);
  return tts::report(0, 0);
}
