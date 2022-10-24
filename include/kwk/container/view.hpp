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
  //! @brief Non-owning, contiguous multi-dimensional container
  //!
  //! @tparam Os    Variadic list of settings describing current's view behavior
  //================================================================================================
  template<typename Data, typename Access, typename MetaData>
  struct  view  : container<kwk::view_,Data,Access,MetaData>
  {
    using parent = container<kwk::view_,Data,Access,MetaData>;

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
    constexpr view(rbr::concepts::option auto const&... opts) : view{rbr::settings{opts...}} {}

    /// Construct a view from a settings descriptor
    constexpr view(rbr::concepts::settings auto const& opts)
            : parent{ []<typename S>(S const& p)
                      { return rbr::merge(rbr::settings{kwk::view_}, p); }(opts)
                    }
    {}

    //==============================================================================================
    //! @}
    //==============================================================================================
  };

  //================================================================================================
  //! @name Deduction guides
  //! @{
  //================================================================================================

  /// This deduction guide is provided for kwk::view to allow deduction from a list of options
  template<rbr::concepts::option... O>
  view(O const&...) -> view < typename detail::builder<view_,O{}...>::memory
                            , typename detail::builder<view_,O{}...>::accessor
                            , typename detail::builder<view_,O{}...>::metadata
                            >;

  /// This deduction guide is provided for kwk::view to allow deduction from another view's settings
  template<rbr::concepts::option... O>
  view(rbr::settings<O...> const&)  -> view < typename detail::builder<view_,O{}...>::memory
                                            , typename detail::builder<view_,O{}...>::accessor
                                            , typename detail::builder<view_,O{}...>::metadata
                                            >;

  //================================================================================================
  //! @}
  //================================================================================================
}
