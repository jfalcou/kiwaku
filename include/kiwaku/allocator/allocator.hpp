//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kiwaku/allocator/block.hpp>
#include <kiwaku/allocator/size.hpp>
#include <kiwaku/concept/allocator.hpp>
#include <cstdlib>

namespace kwk
{
  //================================================================================================
  // allocator is a type-erased object for any allocator following the basic of the block-based
  // allocators interface.
  //
  // It is meant to be used as a non-dependent type in containers or other functions one may
  // want to be non-template
  //================================================================================================
  struct allocator
  {
    private:

    struct api_t
    {
      virtual ~api_t() {}
      virtual block                   allocate(bytes)                 = 0;
      virtual bool                    reallocate(block&, bytes)       = 0;
      virtual void                    deallocate(block&)              = 0;
      virtual bool                    expand(block&, std::ptrdiff_t)  = 0;
      virtual std::unique_ptr<api_t>  clone() const                   = 0;
    };

    template<concepts::allocator T> struct model_t : api_t
    {
      using base_type = T;

      model_t()                     : object()              {}
      model_t(const base_type&  t)  : object(t)             {}
      model_t(base_type&&       t)  : object(std::move(t))  {}

      block allocate(bytes n)                   override { return object.allocate(n);      }
      bool  reallocate(block& b, bytes n)       override { return object.reallocate(b, n); }
      void  deallocate(block& b)                override { object.deallocate(b);           }
      bool  expand(block& b, std::ptrdiff_t n ) override { return object.expand(b, n);     }

      std::unique_ptr<api_t> clone() const override
      {
        return std::make_unique<model_t>(object);
      }

      private:
      T object;
    };

    public:

    allocator() = default;
    allocator(allocator const& a)     : object( a.object->clone() ) {}
    allocator(allocator&& a) noexcept : object{} { swap( a ); }

    template<typename T>
    allocator(T&& o) : object(std::make_unique<model_t<std::decay_t<T>>>(std::forward<T>(o))) {}

    [[nodiscard]] block allocate(bytes n)                   { return object->allocate(n);      }
                  bool  reallocate(block& b, bytes n)       { return object->reallocate(b, n); }
                  void  deallocate(block& b)                { object->deallocate(b);           }
                  bool  expand(block& b, std::ptrdiff_t n)  { return object->expand(b, n);     }

    void swap(allocator& other) noexcept { object.swap(other.object); }

    template<concepts::allocator T> allocator& operator=(T&& other)
    {
      allocator that(std::forward<T>(other));
      swap(that);
      return *this;
    }

    allocator& operator=(allocator const& other)
    {
      allocator that(other);
      swap(that);
      return *this;
    }

    allocator& operator=(allocator&& other)
    {
      allocator that(std::move(other));
      swap(that);
      return *this;
    }

    private:
    std::unique_ptr<api_t> object;
  };
}
