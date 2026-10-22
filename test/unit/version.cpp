//======================================================================================================================
/*
  KIWAKU - Shape Up Your Data
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#include <kiwaku/kiwaku.hpp>

#include <iostream>

int main()
{
  std::cout << "kiwaku " << kiwaku::version << "\n";

  return kiwaku::version ? 0 : 1;
}
