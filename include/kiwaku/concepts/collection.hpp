//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kiwaku/detail/detail.hpp>

namespace kwk::concepts
{
  template<typename T>
  concept collection_like = requires(T const& t) {
    typename T::value_type;
    typename T::source_type;
    { t.ndim };
    { t->kind() };
    { t->shape() };
  };

  template<typename T, typename... Opts>
  concept collection = collection_like<T> && (std::same_as<Opts, Opts> && ...);

  template<typename T>
  concept positional_collection = collection<T>;

  template<typename T>
  concept associative_collection = collection<T>;

  template<typename T>
  concept relational_collection = collection<T>;
}
