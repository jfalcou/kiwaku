//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#pragma once

#include <kiwaku/detail/ct_helpers.hpp>
#include <kiwaku/concept/allocator.hpp>
#include <kiwaku/allocator/block.hpp>
#include <cstdlib>
#include <cstddef>
#include <memory>

namespace kwk
{
  //================================================================================================
  /**
    @ingroup memory
    @brief Opaque allocator type

    Model of the kwk::concepts::allocator concept. kwk::any_allocator is a type-erased object able
    to contains any allocator type modeling kwk::concepts::allocator. It is meant to be used as a
    non-dependent type in containers or other functions one may want to be non-template
  **/
  //================================================================================================
  struct any_allocator
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

    /// Default constructor
    any_allocator()                                 = default;

    /// Move constructor
    any_allocator(any_allocator&& a)                = default;

    /// Move assignment operator
    any_allocator& operator=(any_allocator&& other) = default;

    /// Copy constructor
    any_allocator(any_allocator const& a) : object( a.object->clone() ) {}

    /// Copy assignment operator
    any_allocator& operator=(any_allocator const& other)
    {
      any_allocator that(other);
      swap(that);
      return *this;
    }

    /**
      @brief Constructor from an arbitrary allocator type

      Initializes an kwk::any_allocator with an instance of an kwk::concepts::allocator.

      @param other  Instance of allocator to store
    **/
    template<typename T>
    any_allocator ( T&& other )
                  : object(std::make_unique<model_t<std::decay_t<T>>>(KWK_FWD(other)))
    {}

    /**
      @brief Assign an arbitrary allocator type

      Assign an instance of an kwk::concepts::allocator to the current instance of kwk::any_allocator.

      @param other  Instance of allocator to store
    **/
    template<typename T> any_allocator& operator=(T&& other)
    {
      any_allocator that(KWK_FWD(other));
      swap(that);
      return *this;
    }

    /// Swap the contents of two instance of kwk::heap_allocator
    void swap(any_allocator& other) noexcept { object.swap(other.object); }

    /**
      @brief  Allocates data
      @param  n Number of bytes to allocate
      @return A kwk::block wrapping the newly allocated memory and its size. If zero byte was
              requested, the returned kwk::block is empty.
    **/
    [[nodiscard]] block allocate(std::size_t n) { return object->allocate(n); }

    /**
      @brief  Deallocates data

      Deallocate the memory from a kwk::block. If the deallocated memory was not allocated by the
      same instance of kwk::any_allocator, the behavior is undefined.

      @param  b kwk::block containing the memory to deallocate
    **/
    void  deallocate(block& b)    { object->deallocate(b); }

    private:
    std::unique_ptr<api_t> object;
  };
}
