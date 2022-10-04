//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/detail/raberu.hpp>
#include <kwk/utility/container/shape.hpp>

TTS_CASE("WIP")
{
  using namespace kwk;

  // auto s = kwk::of_size(7_c,fixed<9>,11);
  shape s(7,9,11);

  std::cout << rbr::detail::type_name<decltype(s)>()        << " : " << s << "\n";
  std::cout << rbr::detail::type_name<decltype(s(_,1,_))>() << " : " << s(_,1,_) << "\n";
  std::cout << rbr::detail::type_name<decltype(s(_,_)  )>() << " : " << s(_,_)   << "\n";
  std::cout << rbr::detail::type_name<decltype(s(_)    )>() << " : " << s(_)     << "\n";
};
