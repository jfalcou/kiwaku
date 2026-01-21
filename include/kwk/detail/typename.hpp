//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <array>
#include <string_view>
#include <utility>

namespace kwk::__
{
  // Type -> String converter
  template<typename T> constexpr auto typer() noexcept
  {
#if defined(__clang__)
    constexpr auto pfx = std::string_view("auto kwk::__::typer() [T = ").size();
    constexpr auto sfx = std::string_view("]").size();
    constexpr auto raw = std::string_view(__PRETTY_FUNCTION__);
#elif defined(__GNUC__)
    constexpr auto pfx = std::string_view("constexpr auto kwk::__::typer() [with T = ").size();
    constexpr auto sfx = std::string_view("]").size();
    constexpr auto raw = std::string_view(__PRETTY_FUNCTION__);
#elif defined(_MSC_VER)
    constexpr auto pfx = std::string_view("auto __cdecl kwk::__::typer<").size();
    constexpr auto sfx = std::string_view(">(void)").size();
    constexpr auto raw = std::string_view(__FUNCSIG__);
#endif
    auto value = raw;
    value.remove_prefix(pfx);
    value.remove_suffix(sfx);

    constexpr auto size = raw.size() - (pfx + sfx);
    auto fn = [&]<std::size_t... Is>(std::index_sequence<Is...>) {
      return std::array<char const, sizeof...(Is) + 1>{value[Is]...};
    };

    return fn(std::make_index_sequence<size>{});
  }

  template<typename T> inline constexpr auto type_array = typer<T>();

  template<typename T, auto ID = type_array<T>> struct typename_t
  {
    static constexpr auto name() { return std::string_view(ID.data(), ID.size()); }
  };

  template<typename T> inline constexpr auto name_of = typename_t<T>::name();
}

::__
