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

  //================================================================================================
  //! @ingroup  settings
  //! @brief    Indexing setting for kwk::table
  //================================================================================================
  inline constexpr __::indexing_id indexing{};

  template<typename T> constexpr __::indexing_option<T> strategy(T = {})
  {
    return {};
  }

  // These objects are to be implemented properly. They are used as placeholders atm
  // The idea seems to be that each of them expose some operator[] taking either pos or
  // pos + shape and return the index of the element to index in the storage or smth along
  // those lines, essentially serving as the Pi function over a fiber
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
  inline constexpr auto canonical = strategy<canonical_>();
  inline constexpr auto safe = strategy<safe_>();

  // Interpolation strategies
  inline constexpr auto linear = strategy<linear_>();
  inline constexpr auto nearest = strategy<nearest_>();
  inline constexpr auto affine = strategy<affine_>();

  // Space filling curves
  inline constexpr auto morton = strategy<morton_>();
  inline constexpr auto hilbert = strategy<hilbert_>();

  // Boundary condition
  inline constexpr auto clamp = strategy<clamp_>();
  inline constexpr auto tile = strategy<tile_>();
  inline constexpr auto mirror = strategy<mirror_>();
}
