//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once
#include <type_traits>
#include <utility>

namespace kwk
{
  //================================================================================================
  /**
    @ingroup components
    @brief Span-like kwk::view base class

    SCARY base class gathering all types and member functions pertaining to the upkeep, access and
    update to the span-like part of a kwk::view.

    Contrary to most standard view and container,kwk::view_span keep the potential const qualifier
    of underlying type on purpose.

    @tparam Source  Source object carrying informations about the underlying pointer
  **/
  //================================================================================================
  template<typename Source> struct view_span : Source
  {
    using base_t       = typename Source::base_type;

    /// Underlying pointee value type
    using value_type      = std::remove_const_t<base_t>;

    /// Associated reference type
    using reference       = std::add_lvalue_reference_t<base_t>;

    /// Associated reference to const type
    using const_reference = std::add_lvalue_reference_t<base_t const>;

    /// Associated pointer type
    using pointer         = std::add_pointer_t<base_t>;

    /// Associated const pointer type
    using const_pointer   = std::add_pointer_t<base_t const>;

    /// Associated iterator type
    using iterator        = pointer;

    /// Associated  const iteratortype
    using const_iterator  = const_pointer;

    /// Constructs a kwk::view_span from any source
    view_span(Source src) : Source(src) {}

    /// Returns an iterator to the beginning
    iterator        begin()         { return Source::data(); }

    /// Returns an iterator to the beginning
    const_iterator  begin()   const { return Source::data(); }

    /// Returns a const iterator to the beginning
    const_iterator  cbegin()  const { return Source::data(); }

    /**
      @brief Replaces the managed pointer.

      Given `current_ptr`, the pointer that was managed by `*this`, performs the following
      actions, in this order:

      - Saves a copy of the current pointer `old_ptr = current_ptr`
      - Overwrites the current pointer with the argument `current_ptr = ptr`
      - Returns the copy of the previous current pointer

      @param  ptr pointer to a new pointer to manage
      @return The previous pointer value
    **/
    pointer reset(pointer ptr) noexcept { return std::exchange(Source::data, ptr); }

    /**
      @brief Swap contents of two compatible kwk::view_span

      This function does not participate in overload resolution if
      `std::same_as<base_t, typename OtherSource::base_t>` evaluates to `false`.
    **/
    template<typename OtherSource>
    void swap( view_span<OtherSource>& other ) noexcept
#if !defined(KWK_USE_DOXYGEN)
    requires( std::same_as<base_t, typename OtherSource::base_t> )
#endif
    {
      std::swap(Source::data, other.data);
    }
  };
}
