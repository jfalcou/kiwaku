//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once
#include <type_traits>
#include <utility>

template<int N> using int_ = std::integral_constant<int,N>;
template<int N> struct nD : std::integral_constant<int,N> {};
template<typename N> using up_to = std::make_index_sequence<N::value>;

#define TTS_MAIN
#define TTS_CUSTOM_DRIVER_FUNCTION kwk_entry_point
#include "tts.hpp"

int main(int argc, char const **argv)
{
  std::cout << "[Kiwaku] - Assertions: ";
#ifdef NDEBUG
  std::cout << "Disabled\n";
#else
  std::cout << "Enabled\n";
#endif

  kwk_entry_point(argc, argv);
  return tts::report(0,0);
}
