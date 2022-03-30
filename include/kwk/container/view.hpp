//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#pragma once

#include <kwk/detail/view_builder.hpp>
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
  struct  view  : private detail::view_builder<Type,Os...>::meta_type
                        , detail::view_builder<Type,Os...>::span_type
                        , detail::view_builder<Type,Os...>::access_type
  {
    using meta_t    = typename detail::view_builder<Type,Os...>::meta_type;
    using span_t    = typename detail::view_builder<Type,Os...>::span_type;
    using access_t  = typename detail::view_builder<Type,Os...>::access_type;

    /// Underlying value type
    using value_type      = Type;

    /// Associated iterator type
    using iterator          = typename span_t::iterator;

    /// Associated  const iterator type
    using const_iterator    = typename span_t::const_iterator;

    /// Associated reference type
    using reference         = typename span_t::reference;

    /// Associated reference to const type
    using const_reference   = typename span_t::const_reference;

    /// Associated pointer type
    using pointer         = typename span_t::pointer;

    /// Associated const pointer type
    using const_pointer   = typename span_t::const_pointer;

    /// Compile-time @ref glossary-rank
    static constexpr auto static_rank = access_t::static_rank;

    //==============================================================================================
    //! @name Constructors
    //! @{
    //==============================================================================================

    /// Construct a view from a list of settings
    template<rbr::concepts::option... Opts>
    constexpr view( Opts const&... params )
            : view{rbr::settings(params...)}
    {}

    /// Construct a view from a settings descriptor
    template<rbr::concepts::option... Opts>
    constexpr view(rbr::settings<Opts...> const& params)
            : meta_t { params }
            , span_t{ params[source | ptr_source<Type>{} ].as_span() }
            , access_t { params }
    {}

    //==============================================================================================
    //! @}
    //==============================================================================================

    //==============================================================================================
    //! @name Range interface
    //! @{
    //==============================================================================================
    /// Returns an iterator to the beginning
    constexpr iterator        begin()         { return span_t::begin(); }

    /// Returns an iterator to the beginning
    constexpr const_iterator  begin()   const { return span_t::begin(); }

    /// Returns a const iterator to the beginning
    constexpr const_iterator  cbegin()  const { return span_t::begin(); }

    /// Returns an iterator to the end
    iterator        end()         { return begin() + access_t::size(); }

    /// Returns an iterator to the end
    const_iterator  end()   const { return begin() + access_t::size(); }

    /// Returns a const iterator to the end
    const_iterator  cend()  const { return begin() + access_t::size(); }

    //==============================================================================================
    //! @}
    //==============================================================================================

    /// Swap the contents of two views
    void swap(view& other) noexcept
    {
      access_t::swap( static_cast<access_t&>(other) );
      span_t::swap( static_cast<span_t&>(other) );
    }

    constexpr auto settings() const noexcept
    {
      return rbr::merge ( rbr::settings ( source  = span_t::data()
                                        , size    = access_t::shape()
                                        , strides = access_t::stride()
                                        )
                        , rbr::settings(Os...)
                        );
    }

    //==============================================================================================
    //! @name Indexing operations
    //! @{
    //==============================================================================================
    template<std::integral... Is>
    requires(sizeof...(Is) <= static_rank)
    const_reference operator()(Is... is) const noexcept
    {
      return span_t::data()[ access_t::index(is...) ];
    }

    template<std::integral... Is>
    requires(sizeof...(Is) <= static_rank)
    reference operator()(Is... is) noexcept
    {
      return span_t::data()[ access_t::index(is...) ];
    }

    //==============================================================================================
    //! @}
    //==============================================================================================
  };

  /// Retrieve size along the Ith dimension
  template<std::size_t I, typename T, auto... Os>
  constexpr auto dim(view<T,Os...> const& v) noexcept
  {
    if constexpr(I<view<T,Os...>::static_rank) return get<I>(v.shape());
    else return 1;
  }

  //================================================================================================
  //! @name Deduction guides
  //! @{
  //================================================================================================

  /// This deduction guide is provided for kwk::view to allow deduction from a list of options
  template<rbr::concepts::option... O>
  view(O const&...) -> view<typename rbr::fetch_t<kwk::source,O...>::span_type::base_type, O{}...>;

  /// This deduction guide is provided for kwk::view to allow deduction from another view's settings
  template<rbr::concepts::option... O>
  view(rbr::settings<O...> const&)
      -> view<typename rbr::fetch_t<kwk::source,O...>::span_type::base_type, O{}...>;

  //================================================================================================
  //! @}
  //================================================================================================
}

