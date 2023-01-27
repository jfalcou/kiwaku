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
  //! @tparam Builder  Policy generator for current view
  //================================================================================================
  template<typename Builder>
  struct  view  : container<kwk::view_, Builder>
  {
    using parent = container<kwk::view_, Builder>;

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

    /// Associated @ref kwk::shape type
    using shape_type = typename parent::shape_type;

    /// Compile-time @ref glossary-order
    static constexpr std::int32_t static_order = parent::static_order;

    //==============================================================================================
    //! @name Constructors
    //! @{
    //==============================================================================================

    /// Default constructor
    constexpr view() : parent{kwk::view_} {}

    /// Construct a view from a list of options
    constexpr view(rbr::concepts::option auto const&... opts) : view{rbr::settings{opts...}} {}

    /// Construct a view from a settings descriptor
    constexpr view(rbr::concepts::settings auto const& opts)
            : parent{ []<typename S>(S const& p)
                      { return rbr::merge(rbr::settings{kwk::view_}, p); }(opts)
                    }
    {}

    /// Shallow copy constructor
    constexpr view(concepts::container<as<value_type>, shape_type{}> auto const& other)
            : view(other.settings())
    {}

    /// Shallow assignment operator
    constexpr view& operator=(concepts::container<as<value_type>, shape_type{}> auto const& other)
    {
      view local(other);
      parent::swap(local);
      return *this;
    }

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
  view(O const&...)
    ->  view<detail::builder<rbr::settings(view_, O{}...)>>;

  /// This deduction guide is provided for kwk::view to allow deduction from another view's settings
  template<rbr::concepts::option... O>
  view(rbr::settings<O...> const&)
    ->  view< detail::builder<rbr::settings(view_, O{}...)>>;

  /// This deduction guide is provided for kwk::view to allow deduction from another container
  template<concepts::container C>
  view(C const&)  -> view<detail::builder<C::archetype(view_)>>;

  //================================================================================================
  //! @}
  //================================================================================================

  /// Type helper
  template<auto... Settings> struct make_view
  {
    using type = view<detail::builder<rbr::settings(view_,Settings...)>>;
  };

  template<auto... Settings>
  using make_view_t = typename make_view<Settings...>::type;
}
