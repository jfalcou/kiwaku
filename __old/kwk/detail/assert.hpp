//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#if defined(KIWAKU_ASSERT)

// user/predefined

#elif defined(KIWAKU_DISABLE_ASSERTS) || defined(NDEBUG)

#define KIWAKU_ASSERT(cond, ...) ((void)0)
#define KIWAKU_VERIFY(cond, ...) ((void)(cond))

#else

#include <kwk/detail/abi.hpp>

#include <cstdlib>
#include <type_traits>
#include <utility>
#if defined(BOOST_ENABLE_ASSERT_HANDLER)
#   include <boost/assert.hpp>
#   include <sstream>
#else
#   include <iostream>
#endif

namespace kwk
{
    namespace __
    {
#if defined(BOOST_ENABLE_ASSERT_HANDLER)
#   ifdef __GNUC__
        [[gnu::cold, gnu::noinline]]
#   endif
        inline
        auto print_assertion_header(char const * /*condition*/, char const * /*file*/, unsigned /*line*/) noexcept
        {
            return std::ostringstream{ "[KIWAKU] Assertion failed: " };
        }

#   ifdef __GNUC__
        [[gnu::cold, gnu::noinline]]
#   endif
        [[noreturn]] inline
        void assert_termination(std::ostringstream & stream, char const * const condition, char const * const function, char const * const file, unsigned const line) noexcept
        {
            boost::assertion_failed_msg(condition, std::move(stream).str().c_str(), function, file, line);
            KWK_UNREACHABLE();
        }

#   define KWK_ASSERT_TERMINATION(condition) \
        kwk::__::assert_termination(stream, condition, BOOST_CURRENT_FUNCTION, __FILE__, __LINE__)

#else // Kiwaku assert implementation
#   ifdef __GNUC__
        [[gnu::cold, gnu::noinline]]
#   endif
        inline
        auto & print_assertion_header(char const * const condition, char const * const file, unsigned const line) noexcept
        {
            return std::cerr << "[KIWAKU] Assertion '" << condition << "' failed in " << file << ':' << line << " - ";
        }

#   define KWK_ASSERT_TERMINATION(condition) std::abort()

#endif // assert implementation
    } // namespace __

#ifdef __GNUC__
    [[gnu::cold, gnu::flatten]]
#endif
    [[noreturn]]
    // this simpler approach requires omitting/skipping << (stream operators) in KIWAKU_ASSERT invocations
    // assertion_handler(#cond, __FILE__, __LINE__, __VA_ARGS__);
    void assertion_handler(char const * const condition, [[maybe_unused]]char const * const function, char const * const file, unsigned const line, auto const ... args) noexcept
    {
        decltype(auto) stream{ __::print_assertion_header(condition, file, line) };
        (stream << ... << args);
        KWK_ASSERT_TERMINATION(condition);
    }
} // namespace kwk


#if !defined( KIWAKU_ASSERT_USE_DBG_BREAK )
#    define KIWAKU_ASSERT_DBG_BREAK()
#elif defined( _MSC_VER )
#    define KIWAKU_ASSERT_DBG_BREAK() __debugbreak()
#elif defined( __GNUC__ )
#    define KIWAKU_ASSERT_DBG_BREAK() __builtin_trap()
#endif

#define KIWAKU_ASSERT(cond, ...)                                                        \
  do                                                                                    \
  {                                                                                     \
    if(!(cond)) [[unlikely]]                                                            \
    {                                                                                   \
      KIWAKU_ASSERT_DBG_BREAK();                                                        \
      auto && stream{ kwk::__::print_assertion_header(#cond, __FILE__, __LINE__) };     \
      stream << __VA_ARGS__;                                                            \
      KWK_ASSERT_TERMINATION(#cond);                                                    \
    }                                                                                   \
  } while(0) /**/

#define KIWAKU_VERIFY(cond, ...) KIWAKU_ASSERT(cond, __VA_ARGS__)

#endif
