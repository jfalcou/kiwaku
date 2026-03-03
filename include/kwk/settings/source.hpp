//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

namespace kwk
{
  namespace _
  {
    template<typename T, auto S> struct array_option;
    template<typename T> struct pointer_option;
    template<typename T> struct range_option;

    struct source_id : kumi::identifier<source_id>
    {
      template<concepts::contiguous_static_range R> constexpr auto operator=(R && r) const noexcept
      {
        return array_option<kumi::container_type_t<R>, kumi::container_size_v<R>>{ &KWK_FWD(r[0]) };
      }

      template<concepts::contiguous_range R> constexpr auto operator=(R && r) const noexcept
      {
        return range_option{ std::data(KWK_FWD(r)), std::size(KWK_FWD(r)) };
      }

      template<concepts::pointer P> constexpr auto operator=(P && p) const noexcept
      {
        return pointer_option{ KWK_FWD(p) };
      }
    };

    template<typename T> struct source_option
    {
      using element_type    = T;
      using type            = source_option<T>;
      using identifier_type = source_id;
      
      constexpr auto operator()(identifier_type) const { return *this; }

      static constexpr auto name() { return kumi::str{"Source"}; }

      template<typename CharT, typename Traits>
      friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                           source_option const&) noexcept
      {
        return os << "Source: " << kumi::_::typer<T*>();
      }
    };

    template<typename P> struct pointer_option : source_option<P>
    {
      using value_type      = std::remove_const_t<P>;
      using reference       = std::add_lvalue_reference<P>;
      using const_reference = std::add_lvalue_reference<P const>;
      using point           = std::add_pointer_t<P>;
      using const_poinder   = std::add_pointer_t<P const>;
      
      P* data_;

      constexpr pointer_option(P* p = nullptr) : data_(p){}

      template<typename Other> constexpr pointer_option(Other* o = {}) : data_(std::data(o)){}
    };


    template<typename T, auto S> struct array_option : source_option<T>
    {  
      using value_type      = std::remove_const_t<T>;
      using reference       = std::add_lvalue_reference<T>;
      using const_reference = std::add_lvalue_reference<T const>;
      using point           = std::add_pointer_t<T>;
      using const_poinder   = std::add_pointer_t<T const>;
      
      T* data_;

      constexpr array_option(T* p = nullptr) : data_(p){}
    };


    template<typename T> struct range_option : source_option<T>
    {
      T* data_;
      std::size_t size_;
      
      constexpr range_option(T* p = nullptr, std::size_t s = 0) : data_(p), size_(s) {}
    };
  }

  //================================================================================================
  //! @ingroup  settings
  //! @brief    Source setting for kwk::table and kwk::view
  //================================================================================================
  inline constexpr _::source_id source{};
}
