//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kwk/concepts/range.hpp>

namespace kwk
{
  namespace __
  {
    template<typename T, std::size_t S> struct array_option;
    template<typename T> struct pointer_option;
    template<typename T> struct range_option;

    struct source_id : kumi::identifier<source_id>
    {
      template<kwk::concepts::contiguous_static_range R> constexpr auto operator=(R&& r) const noexcept
      {
        return array_option<kumi::container_type_t<R>, kumi::container_size_v<R>>{&KWK_FWD(r[0])};
      }

      template<kwk::concepts::contiguous_range R> constexpr auto operator=(R&& r) const noexcept
      {
        return range_option{std::data(KWK_FWD(r)), std::size(KWK_FWD(r))};
      }

      template<concepts::pointer P> constexpr auto operator=(P&& p) const noexcept
      {
        return pointer_option{KWK_FWD(p)};
      }

      friend constexpr auto to_str(source_id) { return kumi::str{"Source"}; };
    };

    template<typename T> struct source_option
    {
      using element_type = T;
      using type = source_option<T>;
      using identifier_type = source_id;
      using label_type = kumi::str;

      using value_type = std::remove_const_t<T>;
      using reference = std::add_lvalue_reference<T>;
      using const_reference = std::add_lvalue_reference<T const>;
      using pointer = std::add_pointer_t<T>;
      using const_pointer = std::add_pointer_t<T const>;

      static constexpr label_type label() { return kumi::str{"Source"}; }

      template<typename CharT, typename Traits>
      friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                           source_option const&) noexcept
      {
        return os << "Source: " << kumi::_::typer<T*>();
      }
    };

    template<typename P> struct pointer_option : source_option<P>
    {
      using base = source_option<P>;
      P* data_;

      constexpr pointer_option(P* p = nullptr) : data_(p) {}

      template<typename Other> constexpr pointer_option(Other* o = {}) : data_(std::data(o)) {}

      constexpr auto operator()(base::identifier_type) const { return *this; }
    };

    template<typename P> constexpr auto storage(pointer_option<P> const& source)
    {
      return source.data_;
    }

    template<typename T, std::size_t S> struct array_option : source_option<T>
    {
      using base = source_option<T>;
      T* data_;

      constexpr array_option(T* p = nullptr) : data_(p) {}

      constexpr auto operator()(base::identifier_type) const { return *this; }
    };

    template<typename T, std::size_t S> constexpr auto storage(array_option<T, S> const& source)
    {
      return source.data_;
    }

    template<typename T> struct range_option : source_option<T>
    {
      using base = source_option<T>;
      T* data_;
      std::size_t size_;

      constexpr range_option(T* p = nullptr, std::size_t s = 0) : data_(p), size_(s) {}

      constexpr auto operator()(base::identifier_type) const { return *this; }
    };

    template<typename T> constexpr auto storage(range_option<T> const& source)
    {
      return source.data_;
    }
  }

  //====================================================================================================================
  /**
    @ingroup  settings-source
    @brief Identifier for the source setting for containers

    Identifies the source setting for KIWAKU containers, which specifies the source of the data for the container.
  **/
  //====================================================================================================================
  inline constexpr __::source_id source{};
}
