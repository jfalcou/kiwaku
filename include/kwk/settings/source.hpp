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
  namespace config
  {
    template<typename T> struct preprocess_source
    {
    };
  }

  //====================================================================================================================
  /**
    @ingroup settings
    @defgroup settings-source  Container Source
    @brief Components related to the source of the container
  **/
  //====================================================================================================================
  namespace __
  {
    struct source_id : kumi::identifier<source_id>
    {
      template<typename T>
      constexpr auto operator=(T&& t) const noexcept
      requires(requires { config::preprocess_source<T>::from(KWK_FWD(t)); })
      {
        return config::preprocess_source<T>::from(KWK_FWD(t));
      }

      template<typename T> constexpr auto operator=(T&& t) const = delete;

      friend constexpr auto to_str(source_id) { return kumi::str{"Source"}; };
    };

    template<typename T> struct source_option
    {
      using element_type = T;
      using type = source_option<T>;
      using identifier_type = source_id;
      using label_type = kumi::str;

      using reference = std::add_lvalue_reference<T>;
      using const_reference = std::add_lvalue_reference<std::add_const_t<T>>;
      using pointer = std::add_pointer_t<T>;
      using const_pointer = std::add_pointer_t<T const>;

      static constexpr label_type label() { return kumi::str{"Source"}; }

      constexpr auto data() const { return data_; }

      constexpr auto data() { return data_; }

      pointer data_;
    };
  }

  //====================================================================================================================
  /**
    @ingroup  settings-source
    @brief Identifier for the source setting for containers

    Identifies the source setting for KIWAKU containers, which specifies the source of the data for the container.
  **/
  //====================================================================================================================
  inline constexpr __::source_id source{};

  template<typename Source> struct source_traits;
}

#include <kwk/settings/source/pointer_source.hpp>
#include <kwk/settings/source/array_source.hpp>
#include <kwk/settings/source/range_source.hpp>
