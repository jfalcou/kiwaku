//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <concepts>
#include <type_traits>

namespace kwk::concepts
{
  template<typename T>
  concept proxy = requires(T& t)
  {
    { t.is_proxy() };
  };

  template<typename T>
  concept payload = requires(T& t)
  {
    { t.is_payload() };
  };

  template<typename T>
  concept payload = payload<T> && requires(T& t)
  {
    { t.is_payload() };
  };

  template<typename T>
  concept payload = requires(T& t)
  {
    { t.is_payload() };
  };

  template<typename T>
  concept payload = requires(T& t)
  {
    { t.is_payload() };
  };
}
