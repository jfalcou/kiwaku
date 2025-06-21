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
    // Le proxy est notre manière de passer d'une structure de données Kiwaku
    // à un accesseur.
    // Un proxy doit avoir une méthode .size() pour en donner le nombre d'éléments
    // et une méthode pour retourner créer un accesseur vers ses données.
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

  // Un contexte kiwaku doit avoir une propriété "is_context"
  // (ça nous sert de détrompeur, simplement)
  template<typename T>
  concept context = requires(T const&)
  {
    typename T::is_context;
  };
}
