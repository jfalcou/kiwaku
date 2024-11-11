//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#if defined(KWK_ASSERT)

// user/predefined

#elif defined(KWK_DISABLE_ASSERTS) || defined(NDEBUG)

#define KWK_ASSERT(cond, ...) ((void)0)
#define KWK_VERIFY(cond, ...) ((void)(cond))

#else

#include <kwk/detail/abi.hpp>
#include <source_location>
#include <cstdlib>

//======================================================================================================================
// Interoperability with Boost.Assert
//======================================================================================================================
#if defined(BOOST_ENABLE_ASSERT_HANDLER)
#include <boost/assert.hpp>
#include <sstream>

#define KWK_ASSERT_TERMINATION(condition, loc) kwk::_::assert_termination(stream, condition, loc)

namespace kwk
{
  namespace _
  {
    KWK_COLD inline auto&& print_assertion_header(char const* cond, std::source_location)
    {
      return std::ostringstream{ "[KIWAKU] Assertion failed: " };
    }

    KWK_COLD [[noreturn]] inline
    void assert_termination(std::ostringstream & stream, char const* cond, std::source_location loc)
    {
        boost::assertion_failed_msg ( cond, std::move(stream).str().c_str()
                                    , loc.function_name(), loc.file_name(), loc.line()
                                    );
        KWK_UNREACHABLE();
    }
  }
}

#else
//======================================================================================================================
// Basic implementation
//======================================================================================================================
#include <iostream>

#define KWK_ASSERT_TERMINATION(condition, loc) std::abort()

namespace kwk::_
{
  KWK_COLD inline auto& print_assertion_header(char const* cond, std::source_location loc)
  {
    return std::cerr  << "[KIWAKU] Assertion Failed : '" << cond << "'\n"
                      << "At " << loc.file_name() << ':' << loc.line() << ':' << loc.column() << '\n'
                      << "In function: " << loc.function_name() << '\n'
                      << "Diagnostic: ";
  }
}
#endif

//======================================================================================================================
// Assertion handling function
//======================================================================================================================
namespace kwk::_
{
  KWK_COLD [[noreturn]] inline void assertion_handler(char const* cond, std::source_location loc, auto&&... args)
  {
      auto&& stream{ _::print_assertion_header(cond, loc) };
      (stream << ... << args);
      KWK_ASSERT_TERMINATION(cond, loc);
  }
}

#define KWK_ASSERT(cond, ...)                                                         \
do                                                                                    \
{                                                                                     \
  if(!(cond)) [[unlikely]]                                                            \
  {                                                                                   \
    KWK_ASSERT_DBG_BREAK();                                                           \
    ::kwk::_::assertion_handler(#cond,std::source_location::current(), __VA_ARGS__);  \
  }                                                                                   \
} while(0)                                                                            \
/**/

#define KWK_VERIFY(cond, ...) KWK_ASSERT(cond, __VA_ARGS__)

#endif
