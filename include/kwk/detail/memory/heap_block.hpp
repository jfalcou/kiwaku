//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once
#include <kwk/utility/memory/any_allocator.hpp>
#include <algorithm>
#include <memory>

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

      deleter() : size_(0) {}
      template<typename A> deleter(A a, auto s) : allocator_(a), size_(s) {}

      void operator()(void* p) { deallocate(allocator_,(value_type*)(p)); }
    };

    std::unique_ptr<value_type, deleter> data_;

    template<typename Allocator>
    constexpr heap_block( Allocator a, auto size)
                        : data_ ( (value_type*)(allocate(a,size*sizeof(T)))
                                , deleter(a,size)
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

    constexpr auto get()       noexcept { return data_.get(); }
    constexpr auto get() const noexcept { return data_.get(); }

    constexpr void assign(auto const& src)
    {
      std::copy(data(src), data(src) + data_.get_deleter().size_, get());
    }
  };
}
