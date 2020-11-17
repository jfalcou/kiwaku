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

#include <type_traits>
#include <utility>

template<int N> using int_ = std::integral_constant<int,N>;
template<int N> struct nD : std::integral_constant<int,N> {};
template<typename N> using up_to = std::make_index_sequence<N::value>;

#define TTS_CUSTOM_DRIVER_FUNCTION kwk_entry_point
#include <tts/tts.hpp>

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

#endif
