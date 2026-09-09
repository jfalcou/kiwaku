//======================================================================================================================
/*
  KIWAKU - C++20 collection of high performance data containers and related tools
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#include <kiwaku/kiwaku.hpp>

#include <iostream>

int main()
{
  std::cout << "kiwaku " << kiwaku::version << "\n";

  return kiwaku::version.empty() ? 1 : 0;
}
