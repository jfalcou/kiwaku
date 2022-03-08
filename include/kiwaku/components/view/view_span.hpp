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
  template<typename Source> struct view_span : Source
  {
    using base_type       = typename Source::base_type;
    using value_type      = std::remove_const_t<base_type>;
    using reference       = std::add_lvalue_reference_t<base_type>;
    using const_reference = std::add_lvalue_reference_t<base_type const>;
    using pointer         = std::add_pointer_t<base_type>;
    using const_pointer   = std::add_pointer_t<base_type const>;
    using iterator        = pointer;
    using const_iterator  = const_pointer;

    /// Constructs a kwk::view_span from any source
    view_span(Source src) : Source(src) {}

    iterator        begin()         { return Source::data(); }
    const_iterator  begin()   const { return Source::data(); }
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
      `std::same_as<base_type, typename OtherSource::base_type>` evaluates to `false`.
    **/
    template<typename OtherSource>
    void swap( view_span<OtherSource>& other ) noexcept
#if !defined(KWK_USE_DOXYGEN)
    requires( std::same_as<base_type, typename OtherSource::base_type> )
#endif
    {
      std::swap(Source::data, other.data);
    }
  };
}
