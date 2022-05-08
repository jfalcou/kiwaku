//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once
#include <kwk/allocator/any_allocator.hpp>
#include <memory>
#include <iostream>

namespace kwk::detail
{
  template<typename T> struct heap_block
  {
    using base_type       = T;
    using value_type      = std::remove_const_t<base_type>;
    using reference       = std::add_lvalue_reference_t<base_type>;
    using const_reference = std::add_lvalue_reference_t<base_type const>;
    using pointer         = std::add_pointer_t<base_type>;
    using const_pointer   = std::add_pointer_t<base_type const>;

    struct deleter
    {
      any_allocator   allocator_;
      std::ptrdiff_t  size_;
      std::ptrdiff_t  offset_;

      deleter() : size_(0), offset_(0) {}
      template<typename A> deleter(A a, auto s, auto o) : allocator_(a), size_(s), offset_(o) {}

      void operator()(void* p) { allocator_.deallocate((T*)(p) + offset_); }
    };

    std::unique_ptr<T, deleter> data_;

    template<typename Allocator>
    constexpr heap_block( Allocator a, auto size, auto offset)
                        : data_ ( (T*)(a.allocate(size*sizeof(T))) - offset
                                , deleter(a,size,offset)
                                )
    {}

    constexpr heap_block(heap_block&&)            = default;
    constexpr heap_block& operator=(heap_block&&) = default;

    constexpr heap_block(heap_block const& other)
    {
      auto del = other.data_.get_deleter();
      auto local = heap_block( del.allocator_, del.size_ , del.offset_);
      this->swap(local);
    }

    constexpr heap_block& operator=(heap_block const& other)
    {
      auto that(other);
      this->swap(that);
      return *this;
    }

    constexpr         void swap(heap_block& other)            { data_.swap(other.data_); }
    constexpr friend  void swap(heap_block& a,heap_block& b)  { a.swap(b); }

    constexpr auto data() noexcept { return data_.get(); }
  };
}
