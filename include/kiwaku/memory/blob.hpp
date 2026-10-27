//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kiwaku/detail/detail.hpp>
#include <kiwaku/memory/utils.hpp>
#include <kiwaku/memory/allocators.hpp>
#include <kiwaku/memory/elements.hpp>
#include <kiwaku/memory/scheme.hpp>
#include <kiwaku/memory/blob/heap_blob.hpp>
#include <kiwaku/memory/blob/stack_blob.hpp>
#include <kiwaku/memory/blob/shallow_blob.hpp>
#include <kiwaku/memory/blob/constant_blob.hpp>
#include <kiwaku/detail/compressed_tuple.hpp>

namespace kwk
{
  namespace __
  {
    struct blob_id : kumi::identifier<blob_id>
    {
      friend constexpr auto to_str(blob_id) { return kumi::str {"Blob"}; }
    } inline constexpr blob_type {};

    //==================================================================================================================
    // as_storage<Storage> : maps a *storage intent* to its concrete backend.
    //
    // `Storage` is whatever the builder's make_storage detected (the allocator, a stack
    // size/alignment pair, or nothing for a borrowed region). The mapping:
    //   - kwk::stack_allocator<Sz, A>        -> kwk::stack_blob<Sz, A>     (owning, static)
    //   - any kwk allocator / std allocator  -> kwk::heap_blob             (owning, dynamic)
    //   - anything else (borrowed intent)    -> kwk::shallow_blob          (borrowing an external
    //   region)
    // The function-source (constant) backend is selected at the builder level, where the generator
    // is known, and is documented there via kwk::constant_blob.
    //
    // The mapping is a plain class template naming the concrete blob types directly: the primary
    // template is the borrowed fallback and each storage intent partially specializes the trait.
    //==================================================================================================================
    template<typename Storage> struct as_storage
    {
      using type = kwk::shallow_blob;
    };

    template<std::size_t Sz, std::size_t Al> struct as_storage<kwk::stack_allocator<Sz, Al>>
    {
      using type = kwk::stack_blob<Sz, Al>;
    };

    template<typename Storage>
      requires kwk::concepts::allocator<Storage>
    struct as_storage<Storage>
    {
      using type = kwk::heap_blob;
    };

    //! The concrete owning/borrowing backend selected for a storage intent.
    template<typename Storage> using as_storage_t = typename kwk::__::as_storage<Storage>::type;
  }

  //====================================================================================================================
  // blob<Storage> : the raw byte region backing a collection.
  //
  // `Storage` is a *storage intent* type (e.g. kwk::heap_allocator, kwk::stack_allocator<S,A>, an
  // allocator, or a borrowed tag). The concrete backend is selected by
  // `kwk::__::as_storage<Storage>`:
  //   - owning  + dynamic : heap_blob
  //   - owning  + static  : stack_blob<Size, Alignment>
  //   - borrowed          : shallow_blob
  //====================================================================================================================
  template<typename Storage> struct blob_t : private kwk::__::as_storage_t<Storage>
  {
    using total_space     = kwk::__::as_storage_t<Storage>;

    // kumi::field requirements to consider blob an option
    using element_type    = Storage;
    using type            = blob_t;
    using identifier_type = __::blob_id;
    using label_type      = kumi::str;

    //==================================================================================================================
    // Constructors
    //==================================================================================================================
    constexpr blob_t()      = default;

    //! Borrow an externally-owned region of memory.
    explicit constexpr blob_t(std::byte* addr)
        : total_space {addr}
    {
    }

    //! Allocate a (owning) region of `size` bytes through an allocator.
    template<kwk::concepts::allocator Allocator>
    explicit constexpr blob_t(Allocator a, std::size_t size)
        : total_space {std::move(a), size}
    {
    }

    //==================================================================================================================
    // Raw access
    //==================================================================================================================
    //! Pointer to the first byte of the region.
    constexpr std::byte*                     data() noexcept { return total_space::get(); }
    //! Pointer to the first byte of the region (const).
    constexpr std::byte const*               data() const noexcept { return total_space::get(); }

    //==================================================================================================================
    // Kumi identifier interface
    //==================================================================================================================
    template<typename Self> constexpr auto&& operator()(this Self&& self, identifier_type const&)
    {
      using self_t =
          std::conditional_t<std::is_const_v<std::remove_reference_t<Self>>, blob_t const, blob_t>;
      return std::forward_like<Self>(static_cast<self_t&>(self));
    }

    //==================================================================================================================
    // A blob is a property over itself : it exposes its own value through a `blob()` accessor.
    //==================================================================================================================
    template<typename Self> constexpr decltype(auto) blob(this Self&& self) noexcept
    {
      using type = decltype(std::forward_like<Self>(std::declval<blob_t>()));
      return static_cast<type>(self);
    }

    static constexpr label_type label() { return kumi::str {"Blob"}; }

    constexpr void              swap(blob_t& rhs) noexcept
    {
      if(this != &rhs)
      {
        total_space::swap(rhs);
      }
    }

    constexpr friend void swap(blob_t& self, blob_t& rhs) noexcept { return self.swap(rhs); }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                         blob_t const&) noexcept
    {
      return os << "Blob type";
    }
  };

  //! Alias for the blob type (natural name of the property).
  template<typename Storage> using blob = blob_t<Storage>;

  //! Deduction guide: a `blob` built from an allocator + size keeps the allocator as its storage
  //! intent.
  template<typename Allocator>
  explicit blob_t(Allocator&&, std::size_t) -> blob_t<std::decay_t<Allocator>>;
}
