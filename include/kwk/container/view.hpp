//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/container/options.hpp>
#include <kwk/container/components/container.hpp>
#include <kwk/detail/raberu.hpp>
#include <kwk/options.hpp>
#include <type_traits>

namespace kwk
{
  //================================================================================================
  //! @ingroup containers
  //! @brief Non-owning, contiguous multi-dimensional container
  //!
  //!   @tparam Type  Type of the underlying data
  //!   @tparam Os    Variadic list of settings describing current's view behavior
  //================================================================================================
  template<typename Type, auto... Os>
  struct  view  : container<tag::view_,Type,Os...>
  {
    using parent = container<tag::view_,Type,Os...>;

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

    /// Construct a view from a list of options
    constexpr view(rbr::concepts::option auto const&... opts) : parent{rbr::settings(opts...)} {}

    /// Construct a view from a settings descriptor
    constexpr view(rbr::concepts::settings auto const& params) : parent{ params } {}

    //==============================================================================================
    //! @}
    //==============================================================================================

    constexpr auto settings() const noexcept
    {
      // Retrieve all basic options + correct shape value
      auto const base   = rbr::settings(Os...);
      auto const opts   = rbr::merge( rbr::settings(size = parent::shape())
                                    , base
                                    );

      // Retrieve potential offset to rebuild proper view target
      auto const offset = options::offset(tag, opts);

      if constexpr(parent::has_label)
      {
        return rbr::merge ( rbr::settings ( source = parent::data() + offset
                                                , label = parent::label()
                                                )
                          , opts
                          );
      }
      else
      {
        return rbr::merge ( rbr::settings(source = parent::data() + offset)
                          , opts
                          );
      }
    }
  };

  //================================================================================================
  //! @name Deduction guides
  //! @{
  //================================================================================================

  /// This deduction guide is provided for kwk::view to allow deduction from a list of options
  template<rbr::concepts::option... O>
  view(O const&...) -> view<typename options::element<tag::view_,rbr::settings<O...>>::type,O{}...>;

  /// This deduction guide is provided for kwk::view to allow deduction from another view's settings
  template<rbr::concepts::option... O>
  view(rbr::settings<O...> const&)
      -> view<typename options::element<tag::view_,rbr::settings<O...>>::type, O{}...>;

  //================================================================================================
  //! @}
  //================================================================================================
}
