#pragma once
#include <cstddef>
#include <cstdlib>
#include <type_traits>

template<typename T>
struct aligned_allocator
{
    using value_type = T;

    template<typename U> struct rebind
    {
        using other = aligned_allocator<U>;
    };

    aligned_allocator() noexcept {}

    template<typename U>
    aligned_allocator(aligned_allocator<U> const &) noexcept {}

    value_type *allocate(std::size_t n)
    {
        return static_cast<value_type*>( allocate_aligned(n * sizeof(value_type)));
    }

    void * allocate_aligned(std::size_t n)
    {
      auto sz = (n + 1023) & ~1023;
      return std::aligned_alloc(1024, sz);
    }

    void deallocate(value_type *ptr, std::size_t) noexcept
    {
        if(!ptr) return;
        std::free(ptr);
    }
};

template<typename T, typename U>
bool operator==(aligned_allocator<T> const &, aligned_allocator<U> const &) noexcept
{
  return std::is_same_v<T, U>;
}

template<typename T, typename U>
bool operator!=(aligned_allocator<T> const &, aligned_allocator<U> const &) noexcept
{
return !std::is_same_v<T, U>;
}