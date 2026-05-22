//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/kwk.hpp>

TTS_CASE("kwk::shallow_blob behavior")
{
  float ref[7] = {1, 2, 3, 4, 5, 6, 7};

  kwk::shallow_blob sb(&ref[0]);
  kwk::shallow_blob sb2{};

  TTS_EQUAL(sizeof(sb), sizeof(void*));
  TTS_EQUAL(alignof(decltype(sb)), alignof(float*));
  TTS_EQUAL((reinterpret_cast<std::uintptr_t>(sb.get())), (reinterpret_cast<std::uintptr_t>(&ref[0])));

  std::swap(sb, sb2);
  TTS_EQUAL((reinterpret_cast<std::uintptr_t>(sb2.get())), (reinterpret_cast<std::uintptr_t>(&ref[0])));
};

TTS_CASE("kwk::heap_blob behavior")
{
  auto alloc = kwk::heap_allocator{};
  std::size_t size = 10 * sizeof(float);

  kwk::heap_blob hb(alloc, size);
  kwk::heap_blob hb2(alloc, size);

  auto orig_hb_addr = reinterpret_cast<std::uintptr_t>(hb.data);

  std::swap(hb, hb2);

  TTS_EQUAL(reinterpret_cast<std::uintptr_t>(hb2.data), orig_hb_addr);
  TTS_EQUAL(sizeof(hb), (sizeof(void*) * 2));
};

TTS_CASE("kwk::stack_blob behavior")
{
  static constexpr auto size = (10 * sizeof(float));
  using type = kwk::stack_blob<alignof(float), size>;
  type sb{};
  type sb2{};
  auto addr = reinterpret_cast<std::uintptr_t>(sb.get());

  TTS_EQUAL(sizeof(sb), (10 * sizeof(float)));
  TTS_EQUAL(alignof(decltype(sb)), alignof(float));

  std::swap(sb, sb2);
  TTS_NOT_EQUAL((reinterpret_cast<std::uintptr_t>(sb2.get())), addr);
};

TTS_CASE("kwk::blob behavior")
{
  // An empty class has a size of 1. With EBO, inheriting it should remain size 1.
  // TTS_EQUAL(sizeof(kwk::blob<float, >), sizeof());

  // Test with your stack_blob to ensure it doesn't inflate the footprint
  constexpr auto size = 10;
  constexpr auto bytes_needed = size * sizeof(float);
  using stack_storage_t = kwk::stack_blob<alignof(float), bytes_needed>;
  using stack_blob_t = kwk::__::blob<float, stack_storage_t>;

  // blob must take exactly the size of its storage, adding zero overhead
  TTS_EQUAL(sizeof(stack_blob_t), sizeof(stack_storage_t));
  TTS_EQUAL(sizeof(stack_blob_t), bytes_needed);

  stack_blob_t b{};

  // Initialize elements (triggers start_lifetime_as or placement new)
  b.initialize(size);

  // Prepare a localized source array to verify copying
  float source_data[size] = {1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f};

  // Copy over the elements (triggers trivially copyable memcpy or loop)
  b.copy(source_data, size);

  // Validate that indexing operations and std::launder extract values properly
  for (auto i = 0; i < size; ++i) TTS_EQUAL(b[i], source_data[i]);

  // Clean up lifetimes safely
  b.destroy(size);
};
