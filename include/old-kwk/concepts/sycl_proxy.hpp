//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <concepts>
#include <raberu/raberu.hpp>
#include <kwk/detail/stdfix.hpp>
#include <kwk/settings/size.hpp>
#include <kwk/settings/type.hpp>
#include <kwk/container/settings/category.hpp>
#include <type_traits>
#include <sycl/sycl.hpp>

namespace kwk::concepts
{
  namespace sycl
  {
    // Proxy is our way of going from a Kiwaku data structure to a SYCL accessor.
    // A proxy must have a .size() method to return the number of elements
    // and a method that returns an accessor to its data, given a SYCL handler.
    template<typename T>
    concept proxy = requires(T& t, ::sycl::handler& h)
    {
      { t.size()    };
      { t.access(h) };
    };

    template<typename T>
    concept proxy_accessor = requires(T& t, ::sycl::handler& h)
    {
      { t.access(h) };
    };
  }

  // A Kiwaku context must have a "is_context" property.
  // It serves as a remainder that the object is a context.
  template<typename T>
  concept context = requires(T const&)
  {
    typename T::is_context;
  };
}
