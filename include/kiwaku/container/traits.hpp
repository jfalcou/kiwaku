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
  // The traits reach *through* a collection's `properties` object (via `operator->`), so they give
  // the proper types regardless of which storage/scheme/accessor the collection is built upon. Only
  // the layout/kind/storage/accessor accessors are used; the collection shell itself declares none
  // of these aliases.

  // Layout
  template<typename T>
    requires(std::declval<T>()->shape())
  using shape_type = std::remove_cvref_t<decltype(std::declval<T>()->shape())>;

  template<typename T>
    requires(std::declval<T>()->stride())
  using stride_type = std::remove_cvref_t<decltype(std::declval<T>()->stride())>;

  template<typename T>
    requires(std::declval<T>()->layout())
  using layout_type = std::remove_cvref_t<decltype(std::declval<T>()->layout())>;

  // Memory
  template<typename T>
    requires(std::declval<T>()->storage())
  using storage_t = std::remove_cvref_t<decltype(std::declval<T>()->storage())>;

  // Accessor
  template<typename T>
    requires(std::declval<T>()->accessor())
  using accessor_type = std::remove_cvref_t<decltype(std::declval<T>()->accessor())>;

  // Meta
  template<typename T>
    requires(std::declval<T>()->kind())
  using kind_t = std::remove_cvref_t<decltype(std::declval<T>()->kind())>;

  // Derived from the kind
  template<typename T>
    requires(std::declval<T>()->kind())
  using value_type = typename kind_t<T>::element_type;
}
