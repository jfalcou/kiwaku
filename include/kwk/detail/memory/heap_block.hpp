//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once
#include <kwk/utility/memory/any_allocator.hpp>
#include <kwk/utility/memory/heap_allocator.hpp>
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

      deleter()  {}
      template<typename A> deleter(A a) : allocator_(a) {}
      void operator()(void* p) { deallocate(allocator_,(value_type*)(p)); }
    };

    using ptr_t = std::unique_ptr<value_type, deleter>;
    ptr_t data_;

    constexpr heap_block() : data_(nullptr, deleter{heap_allocator{}}) {}

    template<rbr::concepts::settings Settings>
    constexpr heap_block(Settings const& p)
            : data_ ( [&]()
                      {
                        auto a = pick(kwk::allocator,p);
                        auto s = pick(kwk::size,p);
                        return ptr_t{(value_type*)(allocate(a,s.numel()*sizeof(T))), deleter(a)};
                      }()
                    )
    {
      if constexpr( Settings::contains(kwk::source) )
        assign( storage(pick(kwk::source, p)), pick(kwk::size,p).numel() );
    }

    constexpr         void swap(heap_block& other)            noexcept { data_.swap(other.data_); }
    constexpr friend  void swap(heap_block& a,heap_block& b)  noexcept { a.swap(b); }

    constexpr auto get_data()       noexcept { return data_.get(); }
    constexpr auto get_data() const noexcept { return data_.get(); }

    constexpr void assign(auto const& src, auto sz)
    {
      std::copy(src, src + sz, get_data());
    }
  };

  template<typename T>
  constexpr auto data(heap_block<T> const& src) noexcept { return src.get_data(); }

  template<typename T>
  constexpr auto data(heap_block<T>& src) noexcept { return src.get_data(); }
}
