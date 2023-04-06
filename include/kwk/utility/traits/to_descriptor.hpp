//======================================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
#pragma once

#include <kwk/detail/abi.hpp>
#include <kwk/utility/invoke.hpp>

namespace kwk::tags
{
  struct to_descriptor
  {
    KWK_DEFERS_CALLABLE(to_descriptor_);

    template<typename Extent>
    KWK_TRIVIAL constexpr auto operator()(Extent const& e) const noexcept -> decltype(kwk::tag_invoke(*this, e))
    {
      return kwk::tag_invoke(*this, e);
    }

    //==================================================================================================================
    // Calling this overload means you passed a non-supported type to to_descriptor
    //==================================================================================================================
    template<typename T>
    unsupported_call<to_descriptor(T)> operator()(T const& e) const
    requires(!requires { kwk::tag_invoke(*this, e); }) = delete;
  };
}

namespace kwk
{
  //====================================================================================================================
  //! @ingroup extension
  //! @var to_descriptor
  //! @brief Extent value to descriptor converter
  //!
  //! Convert a valid extent value into the associated consteval descriptor suitable to define a shape or stride type.
  //! The default behavior is to:
  //!   - convert any kwk::joker or integral value to kwk::joker
  //!   - convert any static constant to its value
  //!   - convert any axis to kwk::joker.
  //!   - convert any axis with an associated value to its underlying value.
  //!
  //! @note This @callable can be customized using the [generalized tag_invoke protocol](@ref invoke).
  //!
  //! @groupheader{Callable Signature}
  //! @code
  //! namespace kwk
  //! {
  //!   template<typename Extent>
  //!   constexpr to_descriptor(Extent const& e) noexcept;
  //! }
  //! @endcode
  //!
  //! **Parameters:**
  //!   - `e` : Extent value to convert
  //!
  //! **Returns:** A consteval descriptor usable in the type description of a shape or stride.
  //!
  //====================================================================================================================
  inline constexpr tags::to_descriptor to_descriptor = {};
}

namespace kwk::__
{
  KWK_TRIVIAL constexpr auto to_descriptor_(KWK_DELAY(), joker )              { return _; }
  KWK_TRIVIAL constexpr auto to_descriptor_(KWK_DELAY(), std::integral  auto) { return _; }
  KWK_TRIVIAL constexpr auto to_descriptor_(KWK_DELAY(), concepts::axis auto) { return _; }

  template<concepts::static_constant Extent>
  KWK_TRIVIAL constexpr auto to_descriptor_(KWK_DELAY(), Extent)              { return Extent::value; }

  template<rbr::concepts::option Extent>
  KWK_TRIVIAL constexpr auto to_descriptor_(KWK_DELAY(), Extent const&)
  {
    using key_t   = typename Extent::keyword_type;
    using value_t = typename Extent::stored_value_type;

    if      constexpr(std::integral<value_t> || is_joker_v<value_t>)  return key_t{};
    else if constexpr(concepts::static_constant<value_t>)             return key_t{}[value_t::value];
  }
}
