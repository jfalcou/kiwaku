//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#include "test.hpp"
#include <algorithm>
#include <kiwaku/memory/blob.hpp>

// A minimal, allocator-concept-modeling storage intent.
struct custom_alloc
{
  friend void allocate(custom_alloc&, std::size_t) {}
  friend void deallocate(custom_alloc&, void*) {}
};

TTS_CASE("element - names a kind, byte offset and its layout index")
{
  using kwk::__::element;

  static_assert( std::same_as<element<double, 1>::kind_type, double> );
  static_assert( std::same_as<element<double, 1>::value_type, double> );
  static_assert( std::same_as<element<double, 1>::Layout_idx, kumi::index_t<1>> );

  element<double, 1> e{ 32u };

  // Default element sits at offset zero.
  element<int, 2> d;
  TTS_EQUAL(d.offset, 0u);

  // Offset is a raw byte offset in the blob.
  TTS_EQUAL(e.offset, 32u);

  // Elements with equal bytes are equal.
  TTS_EQUAL((element<int, 1>{4u}), (element<int, 1>{4u}));
  TTS_NOT_EQUAL((element<int, 1>{4u}), (element<int, 1>{5u}));
};

TTS_CASE("resolve - maps (base, element) to a typed pointer")
{
  using kwk::__::resolve;

  alignas(std::max_align_t) std::byte storage[ 16 * sizeof(double) ];
  std::byte* base = storage;

  kwk::__::element<double, 1> e{ 8u };

  double* p = resolve(e, base);
  TTS_EQUAL(p, reinterpret_cast<double*>(base + 8u));

  // A const blob yields a const pointer.
  double const* cp = resolve(e, static_cast<std::byte const*>(base));
  TTS_EQUAL(cp, reinterpret_cast<double const*>(base + 8u));
};

TTS_CASE("scheme - uniform mode: a flat strictly-positive array")
{
  using kwk::__::scheme;

  // Scalar kind: one homogeneous fiber, no per-field offsets.
  scheme<double> uniform;

  static_assert( !scheme<double>::is_product_space );
  static_assert( scheme<double>::field_count == 1u );

  alignas(std::max_align_t) std::byte storage[ 4 * sizeof(double) ];
  std::byte* base = storage;

  auto p = uniform.data(base);
  static_assert( std::same_as<decltype(p), std::byte*> );
  TTS_EQUAL(p, base);

  // operator[] returns a reference into the array
  double* a = reinterpret_cast<double*>(base);
  a[0] = 1.5; a[2] = 3.5;
  TTS_EQUAL(uniform.operator[](base, 0u), 1.5);
  TTS_EQUAL(uniform.operator[](base, 2u), 3.5);

  // It is a true reference: writing through it updates storage.
  uniform.operator[](base, 1u) = 4.25;
  TTS_EQUAL(a[1], 4.25);
};

TTS_CASE("scheme - product mode: one SoA fiber per field")
{
  using record = kumi::tuple<int, double>;
  using kwk::__::scheme;

  // Product kind: two fields, each with its own byte offset (0 and 64 say).
  scheme<record, std::size_t, std::size_t> product{ 0u, 64u };

  static_assert( scheme<record, std::size_t, std::size_t>::is_product_space );
  static_assert( scheme<record, std::size_t, std::size_t>::field_count == 2u );

  alignas(std::max_align_t) std::byte storage[ 128 ];
  std::byte* base = storage;

  // data(base) => tuple of two byte pointers: { base, base + 64 }
  auto fibers = product.data(base);
  static_assert( kumi::size_v<decltype(fibers)> == 2u );
  TTS_EQUAL(static_cast<void*>(kumi::get<0>(fibers)), static_cast<void*>(base));
  TTS_EQUAL(static_cast<void*>(kumi::get<1>(fibers)), static_cast<void*>(base + 64));

  // Write two records' worth so that operator[] sees both fibers.
  int   is[2] = { 7, 9 };
  double ds[2] = { 1.25, 2.5 };
  std::copy_n(reinterpret_cast<std::byte*>(is), 2*sizeof(int),    base);
  std::copy_n(reinterpret_cast<std::byte*>(ds), 2*sizeof(double), static_cast<std::byte*>(kumi::get<1>(fibers)));

  auto r0 = product.operator[](base, 0u);
  auto r1 = product.operator[](base, 1u);
  TTS_EQUAL(kumi::get<0>(r0), 7);
  TTS_EQUAL(kumi::get<1>(r0), 1.25);
  TTS_EQUAL(kumi::get<0>(r1), 9);
  TTS_EQUAL(kumi::get<1>(r1), 2.5);
  static_assert( std::same_as<std::remove_reference_t<decltype(kumi::get<0>(r0))>, int> );
  static_assert( std::same_as<std::remove_reference_t<decltype(kumi::get<1>(r0))>, double> );
};

TTS_CASE("as_storage - selects the concrete backend from a storage intent")
{
  using kwk::__::as_storage_t;

  static_assert( std::same_as<as_storage_t<kwk::heap_allocator>, kwk::heap_blob> );
  static_assert( std::same_as<as_storage_t<kwk::stack_allocator<128, 16>>, kwk::stack_blob<128, 16>> );
  static_assert( std::same_as<as_storage_t<kwk::shallow_blob>, kwk::shallow_blob> );

  // A custom allocator still selects the owning dynamic heap backend.
  static_assert( kwk::concepts::allocator<custom_alloc> );
  static_assert( std::same_as<as_storage_t<custom_alloc>, kwk::heap_blob> );

  // Runtime smoke check: constructing a heap blob allocates a non-null region.
  kwk::blob<kwk::heap_allocator> b{ kwk::heap_allocator{}, 16u };
  alignas(std::max_align_t) std::byte storage[16];
  kwk::blob<kwk::shallow_blob> c{ storage };
  TTS_NOT_EQUAL(b.data(), nullptr);
  TTS_EQUAL(c.data(), storage);
};
