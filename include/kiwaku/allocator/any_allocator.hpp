//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kiwaku/concept/allocator.hpp>
#include <kiwaku/allocator/block.hpp>
#include <cstdlib>
#include <cstddef>
#include <memory>

namespace kwk
{
  //================================================================================================
  // any_allocator is a type-erased object for any allocator following the basic of the block-based
  // allocators interface.
  //
  // It is meant to be used as a non-dependent type in containers or other functions one may
  // want to be non-template
  //================================================================================================
  struct any_allocator final
  {
    private:

    struct api_t
    {
      virtual ~api_t() {}
      virtual block                   allocate(std::size_t) = 0;
      virtual void                    deallocate(block&)    = 0;
      virtual std::unique_ptr<api_t>  clone()         const = 0;
    };

    template<concepts::allocator T> struct model_t final : api_t
    {
      using base_type = T;

      model_t()                     : object()              {}
      model_t(const base_type&  t)  : object(t)             {}
      model_t(base_type&&       t)  : object(std::move(t))  {}

      block allocate(std::size_t n)        override { return object.allocate(n);                }
      void  deallocate(block& b)           override { object.deallocate(b);                     }
      std::unique_ptr<api_t> clone() const override { return std::make_unique<model_t>(object); }

      private:
      T object;
    };

    public:

    any_allocator()                                 = default;
    any_allocator(any_allocator&& a)                = default;
    any_allocator& operator=(any_allocator&& other) = default;

    any_allocator(any_allocator const& a) : object( a.object->clone() ) {}

    any_allocator& operator=(any_allocator const& other)
    {
      any_allocator that(other);
      swap(that);
      return *this;
    }

    template<typename T>
    any_allocator(T&& o) : object(std::make_unique<model_t<std::decay_t<T>>>(std::forward<T>(o))) {}

    template<typename T> any_allocator& operator=(T&& other)
    {
      any_allocator that(std::forward<T>(other));
      swap(that);
      return *this;
    }

    void swap(any_allocator& other) noexcept { object.swap(other.object); }

    [[nodiscard]] block allocate(std::size_t n) { return object->allocate(n); }
                  void  deallocate(block& b)    { object->deallocate(b);      }

    private:
    std::unique_ptr<api_t> object;
  };
}
