//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kwk/concepts.hpp>

namespace kwk
{
  //====================================================================================================================
  /**
    @ingroup memory
    @brief Opaque allocator type

    kwk::any_allocator is a type-erased object that can store and use any allocator satisfying the
    kwk::concepts::allocator concept. It provides a uniform interface to allocate and deallocate memory
    without exposing the underlying allocator type.
  */
  //====================================================================================================================
  struct any_allocator
  {
  private:
    struct api_t
    {
      virtual ~api_t() {}

      virtual void* do_allocate(std::size_t) = 0;
      virtual void do_deallocate(void*) = 0;
      virtual api_t* clone() const = 0;
    };

    template<concepts::allocator T> struct model_t final : api_t
    {
      using base_type = T;

      model_t() : object() {}

      model_t(base_type t) : object(std::move(t)) {}

      void* do_allocate(std::size_t n) override { return allocate(object, n); }

      void do_deallocate(void* b) override { deallocate(object, b); }

      api_t* clone() const override { return new model_t(object); }

    private:
      T object;
    };

  public:
    any_allocator() = default;

    template<typename T> any_allocator(T&& other) : object(new model_t<std::decay_t<T>>(KWK_FWD(other))) {}

    template<typename T> any_allocator& operator=(T&& other)
    {
      object = clone_ptr<api_t>(new model_t<std::decay_t<T>>(KWK_FWD(other)));
      return *this;
    }

    any_allocator(any_allocator&&) = default;
    any_allocator& operator=(any_allocator&&) = default;
    any_allocator(any_allocator const&) = default;
    any_allocator& operator=(any_allocator const&) = default;

    void swap(any_allocator& other) noexcept { std::swap(object, other.object); }

    friend void swap(any_allocator& a, any_allocator& b) noexcept { a.swap(b); }

    api_t* get() const { return object.get(); }

  private:
    __::clone_ptr<api_t> object;
  };

  //====================================================================================================================
  /**
    @ingroup memory
    @brief  Allocates data from an opaque allocator instance

    @param  a Allocator to use for the allocation
    @param  n Number of bytes to allocate
    @return A pointer to the newly allocated memory.
  */
  //====================================================================================================================
  [[nodiscard]] inline auto allocate(any_allocator& a, std::size_t n)
  {
    return a.get()->do_allocate(n);
  }

  //====================================================================================================================
  /**
    @ingroup memory
    @brief  Deallocates data from an opaque allocator instance

    @param  a Allocator to use for the allocation
    @param  ptr Pointer to the data to deallocates
  */
  //====================================================================================================================
  inline void deallocate(any_allocator& a, void* ptr)
  {
    return a.get()->do_deallocate(ptr);
  }
}
