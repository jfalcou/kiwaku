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
  template<typename Data, typename Access, typename MetaData>
  struct  table  : container<kwk::table_,Data,Access,MetaData>
  {
    using parent = container<kwk::table_,Data,Access,MetaData>;

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
    static constexpr auto static_order = parent::static_order;

    //==============================================================================================
    //! @name Constructors
    //! @{
    //==============================================================================================

    /// Default constructor
    constexpr table() : parent{kwk::table_} {}

    /// Construct a table from a list of options
    constexpr table(rbr::concepts::option auto const&... opts) : table{rbr::settings{opts...}} {}

    /// Construct a table from a settings descriptor
    constexpr table(rbr::concepts::settings auto const& opts)
            : parent{ []<typename S>(S const& p)
                      { return rbr::merge(rbr::settings{kwk::table_}, p); }(opts)
                    }
    {}

    /// Move constructor
    constexpr table(table&&) = default;

    /// Move assignment operator
    constexpr table& operator=(table&&) = default;

    /// Copy constructor
    constexpr table(concepts::container<as<value_type>, shape_type{}> auto const& other)
            : table(other.settings())
    {}

    /// Copy assignment operator
    constexpr table& operator=(concepts::container<as<value_type>, shape_type{}> auto const& other)
    {
      table local(other);
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

  /// This deduction guide is provided for kwk::table to allow deduction from a list of options
  template<rbr::concepts::option... O>
  table(O const&...)
    ->  table < typename detail::builder<rbr::settings(table_, O{}...)>::memory
              , typename detail::builder<rbr::settings(table_, O{}...)>::accessor
              , typename detail::builder<rbr::settings(table_, O{}...)>::metadata
              >;

  /// This deduction guide is provided for kwk::table to allow deduction from another table's settings
  template<rbr::concepts::option... O>
  table(rbr::settings<O...> const&)
    ->  table < typename detail::builder<rbr::settings(table_, O{}...)>::memory
              , typename detail::builder<rbr::settings(table_, O{}...)>::accessor
              , typename detail::builder<rbr::settings(table_, O{}...)>::metadata
              >;

  /// This deduction guide is provided for kwk::table to allow deduction from another container
  template<concepts::container C>
  table(C const&) ->  table < typename detail::builder<C::archetype(table_)>::memory
                            , typename detail::builder<C::archetype(table_)>::accessor
                            , typename detail::builder<C::archetype(table_)>::metadata
                            >;

  //================================================================================================
  //! @}
  //================================================================================================

  /// Type helper
  template<auto... Settings> struct make_table
  {
    using type = table< typename detail::builder<rbr::settings(table_,Settings...)>::memory
                      , typename detail::builder<rbr::settings(table_,Settings...)>::accessor
                      , typename detail::builder<rbr::settings(table_,Settings...)>::metadata
                      >;
  };

  template<auto... Settings>
  using make_table_t = typename make_table<Settings...>::type;
}
