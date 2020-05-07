//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_DETAIL_CONTAINER_STACK_STORAGE_HPP_INCLUDED
#define KIWAKU_DETAIL_CONTAINER_STACK_STORAGE_HPP_INCLUDED

#include <cstddef>
#include <array>

namespace kwk::detail
{
  template<typename Type, auto Shape, auto Stride>
  struct stack_storage : private std::array<Type,Shape.numel()>
  {
    using parent          = std::array<Type,Shape.numel()>;
    using value_type      = typename parent::value_type;
    using reference       = typename parent::reference;
    using const_reference = typename parent::const_reference;
    using shape_type      = decltype(Shape);
    using stride_type     = decltype(Stride);

    static constexpr bool is_dynamic  = false;

    //==============================================================================================
    // Access value at a given position in the stack storage
    //==============================================================================================
    template<typename... Int>
    const_reference operator()(Int... is) const noexcept requires(std::is_integral_v<Int> && ...)
    {
      return this->data()[ Stride.index(is...) ];
    }

    template<typename... Int>
    reference operator()(Int... is) noexcept requires(std::is_integral_v<Int> && ...)
    {
      return this->data()[ Stride.index(is...) ];
    }

    constexpr std::size_t     size()          const noexcept  { return Shape.numel(); }
    constexpr std::ptrdiff_t  count()         const noexcept  { return Shape.numel(); }
    constexpr std::size_t     size(int dim)   const noexcept  { return Shape[dim];    }
    constexpr std::ptrdiff_t  count(int dim)  const noexcept  { return Shape[dim];    }
    constexpr auto shape()                    const noexcept  { return Shape;         }
    constexpr auto stride()                   const noexcept  { return Stride;        }

    using parent::swap;
    using parent::data;
    using parent::begin;
    using parent::end;
  };
}

#endif
