//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#pragma once
#include <kwk/container/options.hpp>
#include <type_traits>
#include <utility>

namespace kwk::detail
{
  //================================================================================================
  /*
    Data holder base class

    SCARY base class gathering all types and member functions pertaining to the upkeep, access and
    update to the data part of a kwk container.

    Contrary to most standard view and container,kwk::data_block keep the potential const qualifier
    of underlying type on purpose.

    @tparam Source  Source object carrying informations about the underlying pointer
  */
  //================================================================================================
  template<typename Source> struct data_block : Source
  {
    using base_t       = typename Source::base_type;

    static constexpr bool own_data = Source::own_data;

    // Underlying pointee value type
    using value_type      = std::remove_const_t<base_t>;

    // Associated reference type
    using reference       = std::add_lvalue_reference_t<base_t>;

    // Associated reference to const type
    using const_reference = std::add_lvalue_reference_t<base_t const>;

    // Associated pointer type
    using pointer         = std::add_pointer_t<base_t>;

    // Associated const pointer type
    using const_pointer   = std::add_pointer_t<base_t const>;

    // Associated iterator type
    using iterator        = pointer;

    // Associated  const iterator type
    using const_iterator  = const_pointer;

    // Constructs a kwk::data_block from any source
    constexpr data_block(auto const& tag, rbr::concepts::settings auto const& opts)
              : Source( options::source(tag,opts).as_span() )
    {}

    // Returns an iterator to the beginning
    constexpr iterator        begin()         { return Source::data(); }

    // Returns an iterator to the beginning
    constexpr const_iterator  begin()   const { return Source::data(); }

    // Returns a const iterator to the beginning
    constexpr const_iterator  cbegin()  const { return Source::data(); }

    /*
      Replaces the managed pointer.

      Given `current_ptr`, the pointer that was managed by `*this`, performs the following
      actions, in this order:

      - Saves a copy of the current pointer `old_ptr = current_ptr`
      - Overwrites the current pointer with the argument `current_ptr = ptr`
      - Returns the copy of the previous current pointer

      This function does not participate in overload resolution if the bse data
      Source is owning its data.
    */
    constexpr pointer reset(pointer ptr) noexcept requires(!own_data)
    {
      return std::exchange(Source::data, ptr);
    }

    /*
      Swap contents of two compatible kwk::data_block

      This function does not participate in overload resolution if
      `std::same_as<base_t, typename OtherSource::base_t>` or
      `own_data == data_block<OtherSource>::own_data` evaluates to `false`.
    */
    template<typename OtherSource>
    constexpr void swap( data_block<OtherSource>& other ) noexcept
    requires(   (std::same_as<base_t, typename OtherSource::base_t>)
            &&  (own_data == data_block<OtherSource>::own_data)
            )
    {
      std::swap(Source::data, other.data);
    }
  };
}
