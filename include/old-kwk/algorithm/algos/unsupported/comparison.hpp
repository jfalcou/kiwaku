//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <kwk/concepts/container.hpp>
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/detail/abi.hpp>
#include <cstddef>
#include <utility>

// ALGO:
/*
equal
mismatch
lexicographical_compare
*/

namespace kwk
{
  // KWK_ASSERT shp in1 == in2
//   template<concepts::container In>
//   constexpr bool equal(In const& in1, In const& in2)
//   {
//     if (in1.shape() != in2.shape()) return false;

//     bool isEqual = true;
//     for_each([&](auto i1, auto i2) { isEqual = (isEqual && i1 == i2); }, in1, in2 );

//     return isEqual;
//   }

//   template<concepts::container In>
//   constexpr bool mismatch(In const& in1, In const& in2)
//   {
//     if (in1.shape() != in2.shape()) return false;

//     bool isEqual = true;
//     for_each([&](auto i1, auto i2) 
//     { 
//       if(i1 != i2)
//         return std::make_pair(i1,i2); 
//     }, in1, in2 );

//     return isEqual;
//   }
}
