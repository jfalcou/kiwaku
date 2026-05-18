//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

/*
 * This avoid to include iterators just to get these which cost 300ms of compile time
 * */
namespace kwk
{
  template<typename I> constexpr auto data(I& i) -> decltype(i.data())
  {
    return i.data();
  }

  template<typename I> constexpr auto data(I const& i) -> decltype(i.data())
  {
    return i.data();
  }

  template<typename T, auto N> constexpr auto data(T (&array)[N])
  {
    return array[0];
  }

  template<typename I> constexpr auto size(I& i) -> decltype(i.size())
  {
    return i.size();
  }

  template<typename I> constexpr auto size(I const& i) -> decltype(i.size())
  {
    return i.size();
  }

  template<typename T, auto N> constexpr auto size(T (&)[N])
  {
    return N;
  }

}
