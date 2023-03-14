//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KWK Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/detail/raberu.hpp>
#include <kwk/utility/invoke/tag_invoke.hpp>

namespace kwk
{
//==================================================================================================
//! @addtogroup invoke
//! @{
//==================================================================================================

//==================================================================================================
//! @struct unsupported_call
//! @brief Overloading error reporting helper
//!
//! kwk::unsupported_call is used as a return type when an **KWK** @callable is called with some
//! incorrect parameter types or quantity. Its template parameters embed the tag of the @callable
//! along with the parameter types that caused the error.
//!
//! @tparam Signature Function type describing the erroneous call
//==================================================================================================
template<typename Signature>
struct unsupported_call
{
  constexpr operator bool() const noexcept { return false; }
};

//==================================================================================================
//! @concept callable
//! @brief **KWK** callable
//!
//! A type `T` satisfies kwk::callable if and only if it is tagged as such either
//! manually or by inheriting from a T properties.
//!
//! @tparam T  T type for the @callable to check
//==================================================================================================
template<typename T>
concept callable = requires(T) { typename T::callable_tag_type; };

//==================================================================================================
//! @concept deferred_callable
//! @brief **KWK** deferred callable
//!
//! A type `T` satisfies kwk::deferred_callable_tag if and only if it is a kwk::callable and
//! provides the required static function `deferred_call`.
//!
//! @tparam T  T type for the @callable to check
//==================================================================================================
template<typename T>
concept deferred_callable = requires(T) { typename T::deferred_callable_tag; };

//==================================================================================================
// All callable can have their type streamed to an output stream
// Other project can use tags that inherit from an KWK tag family type or add
// `using kwk::operator<<;` to their own tags namespace to benefit from this.
//==================================================================================================
template<callable Tag>
std::ostream& operator<<(std::ostream& os, Tag const&)
{
  return os << rbr::detail::type<Tag>.name();
}
}

//==================================================================================================
// Basic hook for tag_invoke that just forward to the proper deferred call if possible.
// This binds all existing implementation of <func>_ back to tag_invoke.
// This specialization lives in kwk::tags to be found by ADL as tag themselves will be defines
// in kwk::tags.
//==================================================================================================
namespace kwk::tags
{
  template<deferred_callable Tag>
  KWK_FORCEINLINE constexpr auto tag_invoke(Tag, auto arch, auto&&... x)
  noexcept(noexcept(Tag::deferred_call(arch, KWK_FWD(x)...)))
  -> decltype(Tag::deferred_call(arch, KWK_FWD(x)...))
  {
    return Tag::deferred_call(arch, KWK_FWD(x)...);
  }
}

//==================================================================================================
//! @}
//==================================================================================================

//==================================================================================================
//  Helpers macros
//  NOTE: Those macros are here for convenience but are no way mandatory to use in either KWK, any
//        KWK-dependent library or any user-facing code. The required interface for KWK callable is
//        short enough to be written manually if the need arise.
//==================================================================================================

// Register kwk::detail as the deferred namespace
namespace kwk::detail
{
  struct adl_delay_t {};
  inline constexpr auto adl_delay = adl_delay_t {}
}

//==================================================================================================
//  Defines the static deferred call interface. This static function in tag_invoke callable let
//  external types specify their implementation by asking the callable to forward the call to an
//  externally defined function with a specific name. This reduces the amount of overloads to look
//  at by not looking at all the non NAME related calls.
//==================================================================================================
//  General macro taking the deferred namespace NS and the function NAME
//==================================================================================================
#define KWK_DEFERS_CALLABLE(NAME)                                                                 \
static KWK_FORCEINLINE auto deferred_call(auto arch, auto&&...args) noexcept                      \
    -> decltype(NAME(kwk::detail::adl_delay, arch, KWK_FWD(args)...))                             \
{                                                                                                 \
  return NAME(kwk::detail::adl_delay, arch, KWK_FWD(args)...);                                    \
}                                                                                                 \
using deferred_callable_tag = void                                                                \
/**/

//==================================================================================================
//  Defines the extended tag_invoke with deferred call support and named_callable interface
//==================================================================================================
//  General macro taking the deferred namespace NS and the function NAME
//  NOTE: Don't hesitate to wrap this macro if you need it in your KWK-dependent library.
//==================================================================================================
#define KWK_IMPLEMENTS_CALLABLE(TYPE, NAME)                                                     \
KWK_DEFERS_CALLABLE(NS,NAME);                                                                   \
using callable_tag_type = TYPE                                                                  \
/**/
