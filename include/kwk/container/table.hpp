//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/container/container.hpp>
#include <kwk/detail/raberu.hpp>
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
  template<auto... Os>
  struct  table  : container<kwk::table_,Os...>
  {
    using parent = container<kwk::table_,Os...>;

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

    /// Construct a table from a list of options
    constexpr table(rbr::concepts::option auto const&... opts) : table{rbr::settings{opts...}} {}

    /// Construct a table from a settings descriptor
    constexpr table(rbr::concepts::settings auto const& opts)
            : parent{ []<typename S>(S const& p)
                      { return rbr::merge(rbr::settings{kwk::table_}, p); }(opts)
                    }
    {}

    //==============================================================================================
    //! @}
    //==============================================================================================
    constexpr auto settings() const noexcept
    {
      // Retrieve all basic options + correct shape value
      auto const base   = rbr::settings(kwk::table_, Os...);
      auto const opts   = rbr::merge( rbr::settings(size = parent::shape())
                                    , base
                                    );

      if constexpr(parent::has_label)
      {
        return rbr::merge ( rbr::settings (source = parent::get(), label = parent::label())
                          , opts
                          );
      }
      else
      {
        return rbr::merge ( rbr::settings(source = parent::get()), opts);
      }
    }
  };

  //================================================================================================
  //! @name Deduction guides
  //! @{
  //================================================================================================

  /// This deduction guide is provided for kwk::table to allow deduction from a list of options
  template<rbr::concepts::option... O>
  table(O const&...) -> table<O{}...>;

  /// This deduction guide is provided for kwk::table to allow deduction from another table's settings
  template<rbr::concepts::option... O>
  table(rbr::settings<O...> const&) -> table<O{}...>;

  //================================================================================================
  //! @}
  //================================================================================================
}
