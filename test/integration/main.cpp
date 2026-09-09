//======================================================================================================================
/*
  KIWAKU - Shape Up Your Data
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#include <iostream>

#include <kiwaku/kiwaku.hpp>

int main()
{
  std::cout << "kiwaku " << kiwaku::version << "\n";

  return kiwaku::version.empty() ? 1 : 0;
}
