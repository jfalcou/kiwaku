//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/algorithm/for_each.hpp>
#include <kwk/container/options.hpp>
#include <kwk/container/components/container.hpp>
#include <kwk/detail/raberu.hpp>
#include <kwk/options.hpp>
#include <type_traits>


namespace kwk
{
  //================================================================================================
  //! @ingroup containers
  //! @brief Owning, contiguous multi-dimensional container
  //!
  //!   @tparam Type  Type of the underlying data
  //!   @tparam Os    Variadic list of settings describing current's table behavior
  //================================================================================================
  template<typename Type, auto... Os>
  struct  table  : container<tag::table_,Type,Os...>
  {
    using parent = container<tag::table_,Type,Os...>;

    static constexpr auto tag = parent::tag;

    /// Underlying value type
    using value_type        = typename parent::value_type;

    /// Associated reference type
    using reference         = typename parent::reference;

    /// Associated reference to const type
    using const_reference   = typename parent::const_reference;

    /// Associated pointer type
    using pointer           = typename parent::pointer;

    /// Associated const pointer type
    using const_pointer     = typename parent::const_pointer;

    /// Compile-time @ref glossary-order
    static constexpr auto static_order = parent::static_order;

    //==============================================================================================
    //! @name Constructors
    //! @{
    //==============================================================================================

    constexpr table() : parent{rbr::settings(Os...)} {}

    /// Construct a table from a list of options
    constexpr table(rbr::concepts::option auto const&... opts) : parent{rbr::settings(opts...)} {}

    /// Construct a table from a settings descriptor
    constexpr table(rbr::concepts::settings auto const& params) : parent{ params } {}

    //==============================================================================================
    //! @}
    //==============================================================================================

    constexpr auto settings() const noexcept
    {
      auto const opts   = rbr::settings(Os...);

      if constexpr(parent::has_label)
      {
        return rbr::merge ( rbr::settings ( size  = parent::shape()
                                          , label = parent::label()
                                          )
                          , opts
                          );
      }
      else
      {
        return rbr::merge ( rbr::settings(size = parent::shape())
                          , opts
                          );
      }
    }
  };

  //================================================================================================
  //! @name Deduction guides
  //! @{
  //================================================================================================

  /// This deduction guide is provided for kwk::table to allow deduction from a list of options
  template<rbr::concepts::option... O>
  table(O const&...) -> table<typename options::element<tag::table_,rbr::settings<O...>>::type,O{}...>;

  /// This deduction guide is provided for kwk::table to allow deduction from another table's settings
  template<rbr::concepts::option... O>
  table(rbr::settings<O...> const&)
      -> table<typename options::element<tag::table_,rbr::settings<O...>>::type, O{}...>;

  //================================================================================================
  //! @}
  //================================================================================================
}
