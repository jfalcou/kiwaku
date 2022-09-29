//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier:   KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <kwk/concepts/allocator.hpp>
#include "test.hpp"
#include "tts.hpp"

#include <memory>

namespace ns
{
  struct fake_alloc {};
  void* allocate(fake_alloc&  , std::size_t ) {return nullptr; }
  void  deallocate(fake_alloc&, void*) {}
};

namespace std
{
  template<typename T>
  void* allocate(allocator<T>&, std::size_t ) { return nullptr; }

  template<typename T>
  void  deallocate(allocator<T>&, void*) { }
};

TTS_CASE( "Checks concepts::allocator adaptation" )
{
  TTS_CONSTEXPR_EXPECT( kwk::concepts::allocator<ns::fake_alloc>);
  TTS_CONSTEXPR_EXPECT( kwk::concepts::allocator<std::allocator<int>>);
};
