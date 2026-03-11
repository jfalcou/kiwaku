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
  namespace _
  {
    template<typename T> struct indexing_option;

    struct indexing_id : kumi::identifier<indexing_id>
    {
      template<typename T> constexpr auto operator=(indexing_option<T> const& cnst) const noexcept { return cnst; }

      friend constexpr auto to_str(indexing_id) { return kumi::str{"Indexing"}; }
    };

    template<typename T> struct indexing_option
    {
      using element_type = T;
      using type = indexing_option<T>;
      using identifier_type = indexing_id;

      constexpr auto operator()(identifier_type) const { return *this; }

      static constexpr auto name() { return kumi::str{"Indexing"}; }

      template<typename CharT, typename Traits>
      friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                           indexing_option const&) noexcept
      {
        return os << "Indexing: " << kumi::_::typer<T>();
      }
    };
  }

  inline constexpr _::indexing_id indexing{};

  template<typename T> constexpr _::indexing_option<T> strategy(T = {})
  {
    return {};
  }

  struct canonical_
  {
  };

  struct safe_
  {
  };

  struct linear_
  {
  };

  struct nearest_
  {
  };

  struct affine_
  {
  };

  struct morton_
  {
  };

  struct hilbert_
  {
  };

  struct clamp_
  {
  };

  struct tile_
  {
  };

  struct mirror_
  {
  };

  // Basic strategies
  constexpr inline auto canonical = strategy<canonical_>();
  constexpr inline auto safe = strategy<safe_>();

  // Interpolation strategies
  constexpr inline auto linear = strategy<linear_>();
  constexpr inline auto nearest = strategy<nearest_>();
  constexpr inline auto affine = strategy<affine_>();

  // Space filling curves
  constexpr inline auto morton = strategy<morton_>();
  constexpr inline auto hilbert = strategy<hilbert_>();

  // Boundary condition
  constexpr inline auto clamp = strategy<clamp_>();
  constexpr inline auto tile = strategy<tile_>();
  constexpr inline auto mirror = strategy<mirror_>();

  /// Feels like these are fibers
  // Interpolation
  // Space filling curves
  // wrapping : clamp, periodic, ? mirrored ?

  // To be opposed to strides -> metric tensor
  // which describe the canonical space
}
