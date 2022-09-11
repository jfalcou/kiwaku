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
    using pointer         = typename Source::pointer;

    // Constructs a kwk::data_block from any source
    constexpr data_block(auto const& tag, rbr::concepts::settings auto const& opts)
              : Source( options::block(tag,opts) )
    {}

    constexpr Source&       as_source()       noexcept { return static_cast<Source&>(*this);       }
    constexpr Source const& as_source() const noexcept { return static_cast<Source const&>(*this); }

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
    constexpr pointer reset(pointer ptr) noexcept
    requires requires(Source& s) { s.reset(ptr); }
    {
      return as_source().reset(ptr);
    }

    /*
      Swap contents of two kwk::data_block

      This function does not participate in overload resolution if the underlying source block
      can't be properly swapped.
    */
    template<typename OtherSource>
    constexpr void swap( data_block<OtherSource>& other ) noexcept
    requires requires(Source& a, OtherSource& b) { a.swap(b);}
    {
      as_source().swap(other.as_source());
    }
  };
}
