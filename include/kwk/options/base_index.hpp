//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kwk/detail/raberu.hpp>
#include <kwk/detail/kumi.hpp>
#include <kwk/detail/ct_helpers.hpp>
#include <kwk/options/fixed.hpp>

namespace kwk
{
  template<std::ptrdiff_t Base    > struct index_;
  template<std::ptrdiff_t... Bases> struct indexes_;

  namespace detail
  {
    template< typename Index, std::size_t Dims
            , typename Indexes = std::make_index_sequence<Dims>
            >
    struct indexes;

    template<std::ptrdiff_t... Bs, std::size_t Dims, std::size_t... I>
    requires( sizeof...(Bs) == sizeof...(I) )
    struct indexes<indexes_<Bs...>, Dims, std::index_sequence<I...>>
    {
      static constexpr auto value = kumi::tuple{Bs...};
    };

    template<std::ptrdiff_t B, std::size_t Dims, std::size_t... I>
    struct indexes<index_<B>, Dims, std::index_sequence<I...>>
    {
      static constexpr auto value = kumi::tuple{(I?B:B)...};
    };
  }

#if !defined(KWK_USE_DOXYGEN)
  struct base_index_;

  template<std::ptrdiff_t Base> struct index_
  {
    using stored_value_type = index_<Base>;
    using keyword_type      = base_index_;

    constexpr auto operator()(keyword_type const&) const noexcept { return *this; }

    template<std::size_t Order> constexpr auto as_position() const noexcept
    {
      return detail::indexes<index_, Order>::value;
    }
  };

  template<std::ptrdiff_t... Bases> struct indexes_
  {
    using stored_value_type = indexes_<Bases...>;
    using keyword_type      = base_index_;

    constexpr auto operator()(keyword_type const&) const noexcept { return *this; }

    template<std::size_t Order> constexpr auto as_position() const noexcept
    {
      return detail::indexes<indexes_, Order>::value;
    }
  };

  struct base_index_ : rbr::as_keyword<base_index_>
  {
    template<std::ptrdiff_t Base>
    constexpr auto operator=(index_<Base> const& s) const noexcept { return s; }

    template<std::ptrdiff_t... Bases>
    constexpr auto operator=(indexes_<Bases...> const& s) const noexcept { return s; }

    // Display
    template<std::ptrdiff_t N>
    std::ostream& show(std::ostream& os, index_<N> const&) const
    {
      return os << "Base Index: " << N;
    }

    template<std::ptrdiff_t... N>
    std::ostream& show(std::ostream& os, indexes_<N...> const& ) const
    {
      os << "Base Indexes: {";
      ((os << " " << std::showpos << N),...);
      return os << " }" << std::noshowpos;
    }
  };
#endif

  /**
    @ingroup  options
    @brief    Keyword for accessing base index options
  **/
  inline constexpr auto base_index = base_index_{};

  /**
    @ingroup  options
    @brief    Settings describing the base index for all dimensions of a container
  **/
  template<std::ptrdiff_t N>    inline constexpr auto index   = index_<N>{};

  /**
    @ingroup  options
    @brief    Settings describing different base indexes for each dimensions of a container
  **/
  template<std::ptrdiff_t... N> inline constexpr auto indexes = indexes_<N...>{};
}
