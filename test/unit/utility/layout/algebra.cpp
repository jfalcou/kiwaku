//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
/*
  Layout algebra : hand-verified tensor cases for the mutually-inverse operator pairs in
  `kwk::layout` (see `include/kiwaku/utility/layout.hpp`, "Layout algebra" section).

  Four arithmetic operators form two inverse pairs :
    - `*` (tensor / direct product) and `/` (tiled divide) : `(A * B) / B == A`.
    - `+` (block-diagonal sum) and `-` (inverse)           : `(A + B) - B == A`.
  Two logical operators build combined layouts without scaling :
    - `&&` = merge = nested two-block build (same as `+`).
    - `||` = concatenation (disjoint union of modes).
  `%` is the shape-remainder / complement, `coalesce()` (== unary `*`) flattens, and `make_layout`
  derives a canonical stride from a shape and a compile-time storage order.

  Coverground (every produced ``layout`` is verified against the cosize invariant, i.e.
  `cosize(L) == L.stride().cosize(L.shape())`) :

  - 2x3 x 4x5 tensor product (contiguous : cosize == size == 120).
  - unequal-rank tensor products : (2x3) x 4 and 4 x (2x3) (contiguous).
  - tiled divide round trips : `(A*B)/B == A` and `((A*B)/B)*B == A*B`.
  - block-diagonal sum `+` and its inverse `-` (strip trailing block).
  - `&&` blocked build equals `+`; `||` concatenation.
  - `%` shape remainder / complement.
  - `coalesce()` / unary `*` flattening; `make_layout` canonical strides.
*/
//==================================================================================================
#include "test.hpp"
#include <kiwaku/utility.hpp>

namespace
{
  // Guard : a layout must satisfy cosize(L) == L.stride().cosize(L.shape()).
  template<typename L> void check_invariant(L const& l)
  {
    TTS_EQUAL(l.cosize(), l.stride().cosize(l.shape()));
  }
}

//--------------------------------------------------------------------------------------------------
// `*` : tensor (direct) product, equal ranks, contiguous.
//--------------------------------------------------------------------------------------------------
TTS_CASE("layout algebra - tensor product : 2x3 * 4x5 == { {2,3}, {4,5} } / { {60,20}, {5,1} }")
{
  using namespace kwk::literals;

  auto A = kwk::layout{kwk::shape{2_c, 3_c}, kwk::stride{3_c, 1_c}};  // 2x3 row-major
  auto B = kwk::layout{kwk::shape{4_c, 5_c}, kwk::stride{5_c, 1_c}};  // 4x5 row-major

  auto P = A * B;

  // shape     : { {2,3}, {4,5} }   (both factors kept as nested blocks, B is the inner block)
  // stride    : { {3*20, 1*20}, {5,1} } = { {60,20}, {5,1} }
  // size      : 2*3*4*5  = 120
  // cosize    : 1 + (2-1)*60 + (3-1)*20 + (4-1)*5 + (5-1)*1 = 120  (contiguous)
  TTS_EQUAL(P.shape(), (kwk::shape{kwk::shape{2_c, 3_c}, kwk::shape{4_c, 5_c}}));
  TTS_EQUAL(P.stride(), (kwk::stride{kwk::stride{60_c, 20_c}, kwk::stride{5_c, 1_c}}));
  TTS_EQUAL(P.size(), 120);
  TTS_EQUAL(P.cosize(), 120);
  check_invariant(P);

  // Static (compile-time) preservation : the scaled stride leaf is a compile-time `constant` of value 60.
  TTS_EQUAL(get<0>(get<0>(P.stride())).value, 60);
};

//--------------------------------------------------------------------------------------------------
// `*` : tensor product, arbitrary (unequal) ranks.
//--------------------------------------------------------------------------------------------------
TTS_CASE("layout algebra - tensor product : arbitrary ranks (2x3 * 4 and 4 * 2x3)")
{
  using namespace kwk::literals;

  auto A = kwk::layout{kwk::shape{2_c, 3_c}, kwk::stride{3_c, 1_c}};  // size 6
  auto C = kwk::layout{kwk::shape{4_c},      kwk::stride{1_c}};       // size 4

  // A * C : size(A) == 6 -> C is the inner block ; stride(A) scaled by 4.
  auto P = A * C;
  TTS_EQUAL(P.shape(), (kwk::shape{kwk::shape{2_c, 3_c}, kwk::shape{4_c}}));
  TTS_EQUAL(P.stride(), (kwk::stride{kwk::stride{12_c, 4_c}, kwk::stride{1_c}}));
  TTS_EQUAL(P.size(), 24);
  TTS_EQUAL(P.cosize(), 1 + 1*12 + 2*4 + 3*1);   // 24
  check_invariant(P);

  // C * A : size(C) == 4 -> A is the inner block ; stride(C) scaled by 6.
  auto Q = C * A;
  TTS_EQUAL(Q.shape(), (kwk::shape{kwk::shape{4_c}, kwk::shape{2_c, 3_c}}));
  TTS_EQUAL(Q.stride(), (kwk::stride{kwk::stride{6_c}, kwk::stride{3_c, 1_c}}));
  TTS_EQUAL(Q.size(), 24);
  TTS_EQUAL(Q.cosize(), 1 + 3*6 + 1*3 + 2*1);    // 24
  check_invariant(Q);
};

//--------------------------------------------------------------------------------------------------
// `/` : tiled divide (algebraic inverse of `*`).
//--------------------------------------------------------------------------------------------------
TTS_CASE("layout algebra - tiled divide : (A*B)/B == A and ((A*B)/B)*B == A*B")
{
  using namespace kwk::literals;

  auto A = kwk::layout{kwk::shape{2_c, 3_c}, kwk::stride{3_c, 1_c}};
  auto B = kwk::layout{kwk::shape{4_c, 5_c}, kwk::stride{5_c, 1_c}};
  auto C = kwk::layout{kwk::shape{4_c},      kwk::stride{1_c}};

  auto P = A * B;

  // (A*B) / B == A : strip the inner {4,5} block and divide the stride back by 20.
  auto Q = P / B;
  TTS_EQUAL(Q.shape(), (kwk::shape{2_c, 3_c}));
  TTS_EQUAL(Q.stride(), (kwk::stride{3_c, 1_c}));
  check_invariant(Q);

  // Recombining reproduces the product exactly.
  auto RR = Q * B;
  TTS_EQUAL(RR.shape(), P.shape());
  TTS_EQUAL(RR.stride(), P.stride());

  // Unequal-rank divide : (A*C) / C == A.
  auto Q2 = (A * C) / C;
  TTS_EQUAL(Q2.shape(), (kwk::shape{2_c, 3_c}));
  TTS_EQUAL(Q2.stride(), (kwk::stride{3_c, 1_c}));
  check_invariant(Q2);
};

//--------------------------------------------------------------------------------------------------
// `+` : block-diagonal direct sum, and `-` its inverse.
//--------------------------------------------------------------------------------------------------
TTS_CASE("layout algebra - block-diagonal sum and its inverse")
{
  using namespace kwk::literals;

  auto A = kwk::layout{kwk::shape{2_c, 3_c}, kwk::stride{3_c, 1_c}};
  auto B = kwk::layout{kwk::shape{4_c, 5_c}, kwk::stride{5_c, 1_c}};

  auto S = A + B;

  // shape  : { {2,3}, {4,5} }      each factor keeps its own (unscaled) stride.
  // stride : { {3,1}, {5,1} }
  // cosize : 1 + (2-1)*3 + (3-1)*1 + (4-1)*5 + (5-1)*1 = 25
  TTS_EQUAL(S.shape(), (kwk::shape{kwk::shape{2_c, 3_c}, kwk::shape{4_c, 5_c}}));
  TTS_EQUAL(S.stride(), (kwk::stride{kwk::stride{3_c, 1_c}, kwk::stride{5_c, 1_c}}));
  TTS_EQUAL(S.size(), 120);
  TTS_EQUAL(S.cosize(), 25);
  check_invariant(S);

  // (A + B) - B == A  (the inverse strips the trailing block)
  auto back_a = S - B;
  TTS_EQUAL(back_a.shape(), A.shape());
  TTS_EQUAL(back_a.stride(), A.stride());
  check_invariant(back_a);
};

//--------------------------------------------------------------------------------------------------
// `&&` (merge) and `||` (concatenation).
//--------------------------------------------------------------------------------------------------
TTS_CASE("layout algebra - logical build (&&) and concatenation (||)")
{
  using namespace kwk::literals;

  auto A = kwk::layout{kwk::shape{2_c, 3_c}, kwk::stride{3_c, 1_c}};
  auto B = kwk::layout{kwk::shape{4_c, 5_c}, kwk::stride{5_c, 1_c}};

  // `&&` is the nested two-block build : identical to the block-diagonal sum.
  auto And = A && B;
  TTS_EQUAL(And.shape(), (A + B).shape());
  TTS_EQUAL(And.stride(), (A + B).stride());
  check_invariant(And);

  // `||` concatenates the modes : shape (2,3,4,5), stride (3,1,5,1) ; cosize 25.
  auto Or = A || B;
  TTS_EQUAL(Or.shape(), (kwk::shape{2_c, 3_c, 4_c, 5_c}));
  TTS_EQUAL(Or.stride(), (kwk::stride{3_c, 1_c, 5_c, 1_c}));
  TTS_EQUAL(Or.cosize(), 1 + 1*3 + 2*1 + 3*5 + 4*1);   // 25
  check_invariant(Or);
};

//--------------------------------------------------------------------------------------------------
// `%` : shape remainder / complement.
//--------------------------------------------------------------------------------------------------
TTS_CASE("layout algebra - remainder/complement (%)")
{
  using namespace kwk::literals;

  // 1D : 10 elements tiled by blocks of 3 -> quotient 3, leftover 1 element.
  auto Big = kwk::layout{kwk::shape{10_c}, kwk::stride{1_c}};
  auto Tile = kwk::layout{kwk::shape{3_c}, kwk::stride{1_c}};

  auto rem = Big % Tile;
  TTS_EQUAL(rem.shape(), (kwk::shape{10_c % 3_c}));   // shape {1}
  TTS_EQUAL(rem.stride(), (kwk::stride{1_c}));
  TTS_EQUAL(rem.size(), 1);
  check_invariant(rem);

  // Broadcast : a single-mode divisor applies to every leaf.
  auto Big2 = kwk::layout{kwk::shape{7_c, 8_c}, kwk::stride{8_c, 1_c}};
  auto rem2 = Big2 % Tile;
  TTS_EQUAL(rem2.shape(), (kwk::shape{7_c % 3_c, 8_c % 3_c}));   // shape {1, 2}
  TTS_EQUAL(rem2.size(), 2);
  check_invariant(rem2);
};

//--------------------------------------------------------------------------------------------------
// coalesce / unary `*`, and make_layout.
//--------------------------------------------------------------------------------------------------
TTS_CASE("layout algebra - coalesce (flatten) and make_layout")
{
  using namespace kwk::literals;

  auto A = kwk::layout{kwk::shape{2_c, 3_c}, kwk::stride{3_c, 1_c}};
  auto B = kwk::layout{kwk::shape{4_c, 5_c}, kwk::stride{5_c, 1_c}};

  // coalesce() (equivalently unary `*`) flattens a nested layout to its flat leaves.
  auto merged = A && B;
  auto flat = merged.coalesce();
  TTS_EQUAL(flat.shape(), (kwk::shape{2_c, 3_c, 4_c, 5_c}));
  TTS_EQUAL(flat.stride(), (kwk::stride{3_c, 1_c, 5_c, 1_c}));
  TTS_EQUAL(flat.shape(), (A || B).shape());
  TTS_EQUAL(flat.stride(), (A || B).stride());
  check_invariant(flat);

  // Unary `*` is the same flattening.
  auto flat2 = *A;
  TTS_EQUAL(flat2.shape(), A.shape());
  TTS_EQUAL(flat2.stride(), A.stride());
  check_invariant(flat2);

  // make_layout(shape, order) derives the canonical stride.
  auto L = kwk::make_layout(kwk::shape{2_c, 3_c}, kwk::row_major_order);
  TTS_EQUAL(L.stride(), (kwk::stride{3_c, 1_c}));
  TTS_EQUAL(L.cosize(), 6);
  check_invariant(L);

  auto Lc = kwk::make_layout(kwk::shape{2_c, 3_c}, kwk::column_major_order);
  TTS_EQUAL(Lc.stride(), (kwk::stride{1_c, 2_c}));
  TTS_EQUAL(Lc.cosize(), 6);
  check_invariant(Lc);

  // make_layout(shape, stride) is the direct factory.
  auto Ld = kwk::make_layout(kwk::shape{2_c, 3_c}, kwk::stride{3_c, 1_c});
  TTS_EQUAL(Ld.shape(), A.shape());
  TTS_EQUAL(Ld.stride(), A.stride());
  check_invariant(Ld);
};

//--------------------------------------------------------------------------------------------------
// is_compact : external trait (cosize == size).
//--------------------------------------------------------------------------------------------------
TTS_CASE("layout algebra - is_compact (external trait)")
{
  using namespace kwk::literals;

  // Compact : contiguous row-major / column-major layouts occupy exactly size() slots.
  auto row = kwk::layout{kwk::shape{2_c, 3_c}, kwk::stride{3_c, 1_c}};   // cosize 6 == size 6
  auto col = kwk::layout{kwk::shape{2_c, 3_c}, kwk::stride{1_c, 2_c}};   // cosize 6 == size 6
  TTS_EQUAL(kwk::is_compact(row), true);
  TTS_EQUAL(kwk::is_compact(col), true);

  // The tensor product of two compact layouts is compact (contiguous) : cosize == size == 120.
  auto A = kwk::layout{kwk::shape{2_c, 3_c}, kwk::stride{3_c, 1_c}};
  auto B = kwk::layout{kwk::shape{4_c, 5_c}, kwk::stride{5_c, 1_c}};
  TTS_EQUAL(kwk::is_compact(A * B), true);

  // Gapped : unit strides overlapping -> cosize 4 < size 6 -> not compact.
  auto gap = kwk::layout{kwk::shape{2_c, 3_c}, kwk::stride{1_c, 1_c}};
  TTS_EQUAL(kwk::is_compact(gap), false);

  // Block-diagonal sum is not compact when blocks are juxtaposed with their own offsets (cosize 25 < size 120).
  TTS_EQUAL(kwk::is_compact(A + B), false);

  // make_layout(shape, order) derives compact canonical strides.
  TTS_EQUAL(kwk::is_compact(kwk::make_layout(kwk::shape{2_c, 3_c}, kwk::row_major_order)), true);

  // For a fully-static layout the check is a compile-time constant expression.
  static_assert(kwk::is_compact(row));
  static_assert(!kwk::is_compact(gap));
};
