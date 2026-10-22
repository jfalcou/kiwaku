//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

namespace kwk::__
{
  template<typename T>
  requires(requires(T const& t) { t.clone(); })
  struct clone_ptr
  {
    T* ptr = nullptr;

    clone_ptr() = default;

    explicit clone_ptr(T* p) : ptr(p) {}

    ~clone_ptr() { delete ptr; }

    clone_ptr(clone_ptr const& other) : ptr(other.ptr ? other.ptr->clone() : nullptr) {}

    clone_ptr& operator=(clone_ptr const& other)
    {
      if (this != &other)
      {
        clone_ptr tmp(other);
        std::swap(ptr, tmp.ptr);
      }
      return *this;
    }

    clone_ptr(clone_ptr&& other) noexcept : ptr(std::exchange(other.ptr, nullptr)) {}

    clone_ptr& operator=(clone_ptr&& other) noexcept
    {
      delete ptr;
      ptr = std::exchange(other.ptr, nullptr);
      return *this;
    }

    T* get() const { return ptr; }

    T& operator*() const { return *ptr; }

    T* operator->() const { return ptr; }

    explicit operator bool() const { return ptr != nullptr; }
  };
}
