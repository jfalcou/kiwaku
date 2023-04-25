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
#include <kwk/settings/type.hpp>

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
  //! @brief @ref glossary-extent value to descriptor converter
  //!
  //! Convert a valid @ref glossary-extent value into the associated consteval descriptor suitable to define a shape
  //! or stride type.
  //!
  //! The default behavior is to:
  //!   - convert any kwk::joker to kwk::joker
  //!   - convert any integral value to its associated kwk::as type descriptor
  //!   - convert any static constant to its value
  //!   - convert any @ref glossary-axis to an axis with the proper associated value while preserving pre-existing
  //!     type constraints
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
  //!   - `e` : @ref glossary-extent value to convert
  //!
  //! **Returns:** A consteval descriptor usable in the type description of a shape or stride.
  //!
  //====================================================================================================================
  inline constexpr tags::to_descriptor to_descriptor = {};
}

namespace kwk::__
{
  KWK_TRIVIAL constexpr auto to_descriptor_(KWK_DELAY(), joker )              { return _; }

  template<std::integral T>
  KWK_TRIVIAL constexpr auto to_descriptor_(KWK_DELAY(), T)                   { return as<T>; }

  template<typename T>
  KWK_TRIVIAL constexpr auto to_descriptor_(KWK_DELAY(), type_::info<T> x)    { return x; }

  template<concepts::axis Axis>
  KWK_TRIVIAL constexpr auto to_descriptor_(KWK_DELAY(), Axis a)
  {
    return axis_<Axis::identifier>{}[to_descriptor(a.value)];
  }

  template<concepts::static_constant Extent>
  KWK_TRIVIAL constexpr auto to_descriptor_(KWK_DELAY(), Extent)              { return Extent::value; }

  template<rbr::concepts::option Extent>
  KWK_TRIVIAL constexpr auto to_descriptor_(KWK_DELAY(), Extent const&)
  {
    using key_t     = typename Extent::keyword_type;
    using content_t = typename key_t::content_type;
    using value_t   = typename Extent::stored_value_type;

    if      constexpr(is_joker_v<value_t>)                return key_t{};
    else if constexpr(std::integral<value_t>)             return key_t{}[to_descriptor(content_t{})];
    else if constexpr(concepts::static_constant<value_t>)
    {
      using tgt_t = decltype(to_descriptor(content_t{}));
      if constexpr(is_joker_v<tgt_t>) return key_t{}[value_t::value];
      else                            return key_t{}[static_cast<typename tgt_t::type>(value_t::value)];
    }
  }
}
