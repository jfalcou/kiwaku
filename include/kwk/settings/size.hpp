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
    template<kumi::concepts::indexer T> struct size_option;

    struct size_id : kumi::identifier<size_id>
    {
      template<kumi::concepts::indexer I> constexpr auto operator=(I i) const noexcept 
      { 
        return size_option<I>{ i }; 
      }

      friend constexpr auto to_str(size_id) { return kumi::str{"Shape"}; }
    };

    template<kumi::concepts::indexer T> struct size_option
    {
      using element_type = T;
      using type = size_option<T>;
      using identifier_type = size_id;
      
      T value;

      constexpr auto operator()(identifier_type) const { return *this; }

      static constexpr auto name() { return kumi::str{"Shape"}; }

      template<typename CharT, typename Traits>
      friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                           size_option const& s) noexcept
      {
        return os << "Shape : " << s.value; 
      }
    };
  }

  inline constexpr _::size_id size{}; 
}
