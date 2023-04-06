//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/detail/abi.hpp>

#include <ostream>
#include <utility>
#include <type_traits>

//====================================================================================================
//  This part is a indirect implementation of P1895R0
//  See https://www.open-std.org/jtc1/sc22/WG21/docs/papers/2019/p1895r0.pdf for details and
//  rationale.
//====================================================================================================
namespace kwk::func_ns
{
struct invoker
{
  template<typename Tag, typename... Args>
  requires requires(Tag tag, Args&&...args) { tag_invoke(tag, KWK_FWD(args)...); }
  KWK_FORCEINLINE constexpr auto operator()(Tag tag, Args&&...args) const
  noexcept(noexcept(tag_invoke(tag, KWK_FWD(args)...)))
      -> decltype(tag_invoke(tag, KWK_FWD(args)...))
  {
    return tag_invoke(tag, KWK_FWD(args)...);
  }
};
}

namespace kwk
{
//==================================================================================================
//! @addtogroup traits
//! @{
//! @defgroup invoke Generalized Tag Invoke Protocol
//! @brief This module defines all the **KIWAKU** generalized tag_invoke protocol infrastructure.
//! @}
//==================================================================================================

inline namespace callable_ns
{
  inline constexpr func_ns::invoker tag_invoke = {};
}

//==================================================================================================
//! @addtogroup invoke
//! @{
//==================================================================================================

//==================================================================================================
//! @concept tag_invocable
//! @brief Type supporting the tag_invoke protocol
//!
//! A type `Tag` satisfies kwk::tag_invocable<Args...> if and only if it can be used in a call to
//! kwk::tag_invoke.
//==================================================================================================
template<typename Tag, typename... Args>
concept tag_invocable =
    requires(Tag&& tag, Args&&...args) { kwk::tag_invoke(KWK_FWD(tag), KWK_FWD(args)...); };

//! @brief Compute the return type of a kwk::tag_invoke call.
template<typename Tag, typename... Args>
using tag_invoke_result = std::invoke_result_t<decltype(kwk::tag_invoke), Tag, Args...>;

//! @brief Compute the type of an instance of an **KIWAKU** @callable.
template<auto& Func> using tag_of = std::decay_t<decltype(Func)>;

//==================================================================================================
//! @}
//==================================================================================================

}
