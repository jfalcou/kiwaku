//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

namespace kwk
{
  namespace __
  {
    struct iterator
    {
      template<typename Dimension, typename Func, typename... Is>
      constexpr auto operator()(Func f, Dimension d, Is... is) const noexcept
      {
        for(std::ptrdiff_t i = 0; i < d; ++i) f(is..., i);
      }
    } inline constexpr iteration;

    struct for_each_t
    {
      template<typename Func, typename Dimensions>
      constexpr auto operator()(Func f, Dimensions&& d) const noexcept
      {
        // Reduce a (possibly tiled/nested) space to its flat leaf extents, then iterate row-major
        // over every leaf so the callback receives one coordinate per flat dimension (matching the
        // collection's position operator `c[idxs...]`). Folding over the unflattened structure
        // would try to iterate `i < shape` (a compile error) or silently drop trailing grid
        // dimensions.
        auto leaf_extents = kumi::flatten_all(KWK_FWD(d));
        return kumi::fold_right([](auto m, auto acc)
                                { return kumi::bind_front(iteration, acc, m); },
                                leaf_extents,
                                KWK_FWD(f))();
      }
    } inline constexpr for_each;
  }
}
