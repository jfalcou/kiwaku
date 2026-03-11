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
    template<typename T> struct generator_option;

    struct generator_id : kumi::identifier<generator_id>
    {
      template<typename C> constexpr auto operator=(C const& c) const noexcept { return generator_option{c}; }

      friend constexpr auto to_str(generator_id) { return kumi::str{"Generator"}; }
    };

    template<typename C> struct generator_option
    {
      using element_type = C;
      using type = generator_option<C>;
      using identifier_type = generator_id;

      C content;

      constexpr generator_option() = default;

      constexpr generator_option(C c) : content(c) {}

      constexpr auto operator()(identifier_type) const { return *this; }

      static constexpr auto name() { return kumi::str{"Generator"}; }

      template<typename CharT, typename Traits>
      friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                           generator_option const&) noexcept
      {
        return os << "Generator: " << kumi::_::typer<C>();
      }
    };
  }

  //================================================================================================
  //! @ingroup  settings
  //! @brief    Generator setting for kwk::table and kwk::view
  //================================================================================================
  inline constexpr _::generator_id generator{};
}
