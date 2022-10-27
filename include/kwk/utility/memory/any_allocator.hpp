//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/concepts/allocator.hpp>
#include <kwk/detail/abi.hpp>
#include <cstdlib>
#include <cstddef>
#include <memory>

namespace kwk
{
  //================================================================================================
  //! @ingroup memory
  //! @brief Opaque allocator type
  //!
  //! Model of the kwk::concepts::allocator concept. kwk::any_allocator is a type-erased object able
  //! to contains any allocator type modeling kwk::concepts::allocator. It is meant to be used as a
  //! non-dependent type in containers or other functions one may want to be non-template
  //================================================================================================
  struct any_allocator
  {
    private:

    struct api_t
    {
      virtual ~api_t() {}
      virtual void*                   do_allocate(std::size_t) = 0;
      virtual void                    do_deallocate(void* )    = 0;
      virtual std::unique_ptr<api_t>  clone()         const = 0;
    };

    template<concepts::allocator T> struct model_t final : api_t
    {
      using base_type = T;

      model_t()             : object()              {}
      model_t(base_type t)  : object(std::move(t))  {}

      void* do_allocate(std::size_t n)        override { return allocate(object,n);             }
      void  do_deallocate(void* b)            override { deallocate(object,b);                  }
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

    /// Constructor from an arbitrary allocator type
    template<typename T>
    any_allocator ( T&& other )
                  : object(std::make_unique<model_t<std::decay_t<T>>>(KWK_FWD(other)))
    {}

    /// Assignment from an arbitrary allocator
    template<typename T> any_allocator& operator=(T&& other)
    {
      any_allocator that(KWK_FWD(other));
      swap(that);
      return *this;
    }

    /// Swap the contents of two instance of kwk::heap_allocator
    void swap(any_allocator& other) noexcept { object.swap(other.object); }
    friend void swap(any_allocator& a, any_allocator& b) noexcept { a.swap(b); }

    /// Access to internal allocator pointer
    api_t* get() const { return object.get(); }

    private:
    std::unique_ptr<api_t> object;
  };

  //================================================================================================
  //! @ingroup memory
  //! @brief  Allocates data from an opaque allocator instance
  //! @param  n Number of bytes to allocate
  //! @return A pointer to the newly allocated memory.
  //================================================================================================
  [[nodiscard]] inline auto allocate(any_allocator& a, std::size_t n)
  {
    return a.get()->do_allocate(n);
  }

  //================================================================================================
  //! @ingroup memory
  //! @brief  Deallocates data from an opaque allocator instance
  //! @param  ptr Pointer to the data to deallocates
  //================================================================================================
  inline void deallocate(any_allocator& a, void* ptr)
  {
    return a.get()->do_deallocate(ptr);
  }
}
