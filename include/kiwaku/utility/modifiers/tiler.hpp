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
  template<typename T, typename Shape, typename Stride> struct tiler : T
  {
    using solver_t = T;

    template<typename Self> [[nodiscard]] KWK_ABI constexpr decltype(auto) solver(this Self&& self)
    {
      using type = decltype(std::forward_like<Self>(std::declval<solver_t>()));
      return static_cast<type>(self);
    }
  };

  template<typename T, typename S, typename D>
  tiler(T, S&&, D&&) -> tiler<T, std::unwrap_ref_decay_t<S>, std::unwrap_ref_decay_t<D>>;

  // Helpers containing tiling formula

  // Paving tiles
  struct paving_t
  {
    template<kwk::concepts::layout Lhs, kwk::concepts::layout Rhs>
    constexpr auto operator()(Lhs && lhs, Rhs && rhs) const noexcept
    {
      return (lhs / rhs) & (lhs % rhs);
    }
  } inline constexpr pave;

  // Overlapping tiles
  struct overlap_t
  {
    template<kwk::concepts::layout Lhs, kwk::concepts::layout Rhs>
    constexpr auto operator()(Lhs && lhs, Rhs && rhs) const noexcept
    {
      return (lhs - rhs) & (lhs % rhs);
    }
  } inline constexpr overlap;

  //
  template<typename T> struct tiles_t : T
  {
    using solver_t = T;

    template<typename Self> [[nodiscard]] KWK_ABI constexpr decltype(auto) solver(this Self&& self)
    {
      using type = decltype(std::forward_like<Self>(std::declval<solver_t>()));
      return static_cast<type>(self);
    }

    template<kwk::concepts::shape Shape> KWK_ABI constexpr auto operator()(Shape && shp) const noexcept
    {
      return (*this)(shp, to_stride(shp));
    }

    template<kwk::concepts::shape Shape, kwk::concepts::stride Stride>
    KWK_ABI constexpr auto operator()(Shape && shp, Stride && srd) const noexcept
    {
      return kwk::tiler{solver(), shp, srd};
    }

    template<typename O> KWK_ABI constexpr auto operator[](O o) const noexcept { return kwk::tiles_t{o}; }
  };

  template<typename T> tiles_t(T) -> tiles_t<T>;

  inline constexpr tiles_t<paving_t> tiles{};
}
