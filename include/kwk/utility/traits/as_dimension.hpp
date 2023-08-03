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
  struct as_dimension
  {
    KWK_DEFERS_CALLABLE(as_dimension_);

    KWK_TRIVIAL constexpr   auto operator()(auto const& e, std::integral auto def) const noexcept
                        ->  decltype(kwk::tag_invoke(*this, e, def))
    {
      return kwk::tag_invoke(*this, e, def);
    }

    //==================================================================================================================
    // Calling this overload means you passed a non-supported type to as_dimension
    //==================================================================================================================
    template<typename Extent, typename Default>
    unsupported_call<as_dimension(Extent,Default)> operator()(Extent const& e, Default d ) const
    requires(!requires { kwk::tag_invoke(*this, e, d); }) = delete;
  };
}

namespace kwk
{
  //====================================================================================================================
  //! @ingroup extension
  //! @var as_dimension
  //! @brief Normalize an @ref glossary-extent value
  //!
  //! Convert a valid @ref glossary-extent value into the simplest value required for its storage.
  //! The default behavior is to:
  //!   - convert any integral value to itself
  //!   - convert any kwk::joker or @ref glossary-axis to the default value
  //!   - convert any @ref glossary-axis with an associated value to the dimension of its underlying value.
  //!
  //! @note This @callable can be customized using the [generalized tag_invoke protocol](@ref invoke).
  //!
  //! @groupheader{Callable Signature}
  //! @code
  //! namespace kwk
  //! {
  //!   template<typename Extent>
  //!   constexpr as_dimension(Extent const& e, std::integral auto d) noexcept;
  //! }
  //! @endcode
  //!
  //! **Parameters:**
  //!   - `e` : @ref glossary-extent value to convert
  //!   - `d` : Default value to use if needed
  //!
  //! **Returns:** The actual value stored in the shape or stride entity.
  //!
  //====================================================================================================================
  inline constexpr tags::as_dimension as_dimension = {};
}

namespace kwk::__
{
  KWK_TRIVIAL constexpr auto as_dimension_(KWK_DELAY(), joker                , std::integral auto d) { return d; }
  KWK_TRIVIAL constexpr auto as_dimension_(KWK_DELAY(), concepts::axis auto v, std::integral auto  ) { return v.value; }
  KWK_TRIVIAL constexpr auto as_dimension_(KWK_DELAY(), std::integral  auto v, std::integral auto  ) { return v; }

  KWK_TRIVIAL constexpr auto as_dimension_(KWK_DELAY(), concepts::static_constant auto a, std::integral auto)
  {
    return a;
  }

  template<typename T>
  KWK_TRIVIAL constexpr auto as_dimension_(KWK_DELAY(), type_::info<T>, std::integral auto d)
  {
    return static_cast<T>(d);
  }

  template<rbr::concepts::option Extent>
  KWK_TRIVIAL constexpr   auto as_dimension_(KWK_DELAY(), Extent const& e, std::integral auto d)
                      ->  decltype(as_dimension(e.contents, d))
  {
    return as_dimension(e.contents, d);
  }
}
