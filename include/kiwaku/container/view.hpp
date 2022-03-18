//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kiwaku/detail/view_builder.hpp>
#include <kiwaku/detail/raberu.hpp>
#include <kiwaku/options.hpp>
#include <type_traits>

namespace kwk
{
  template<typename Type, auto... Os>
  struct  view  : detail::view_builder<Type,Os...>::span_type
                , detail::view_builder<Type,Os...>::access_type
                , detail::view_builder<Type,Os...>::meta_type
  {
    using meta_type     = typename detail::view_builder<Type,Os...>::meta_type;
    using span_type     = typename detail::view_builder<Type,Os...>::span_type;
    using access_type   = typename detail::view_builder<Type,Os...>::access_type;

    using iterator          = typename span_type::iterator;
    using const_iterator    = typename span_type::const_iterator;

    using reference         = typename span_type::reference;
    using const_reference   = typename span_type::const_reference;

    static constexpr auto static_nbdims = access_type::static_nbdims;

    template<rbr::concepts::option... Opts>
    constexpr view( Opts const&... params )
            : view{rbr::settings(params...)}
    {}

    template<rbr::concepts::option... Opts>
    constexpr view(rbr::settings<Opts...> const& params)
            : span_type{ params[source | ptr_source<Type>{} ].as_span() }
            , access_type { params }
            , meta_type { params }
    {}

    constexpr auto settings() const noexcept
    {
      return rbr::merge ( rbr::settings ( source  = span_type::data()
                                        , size    = access_type::shape()
                                        , strides = access_type::stride()
                                        )
                        , rbr::settings(Os...)
                        );
    }

    /// Returns an iterator to the end
    iterator        end()         { return span_type::begin() + access_type::size(); }

    /// Returns an iterator to the end
    const_iterator  end()   const { return span_type::begin() + access_type::size(); }

    /// Returns a const iterator to the end
    const_iterator  cend()  const { return span_type::begin() + access_type::size(); }

    /// Swap the contents of two views
    void swap(view& other) noexcept
    {
      access_type::swap( static_cast<access_type&>(other) );
      span_type::swap( static_cast<span_type&>(other) );
    }

    //==============================================================================================
    //! @name Indexing operations
    //! @{
    //==============================================================================================
    template<std::integral... Is>
    requires(sizeof...(Is) <= static_nbdims)
    const_reference operator()(Is... is) const noexcept
    {
      return span_type::data()[ access_type::index(is...) ];
    }

    template<std::integral... Is>
    requires(sizeof...(Is) <= static_nbdims)
    reference operator()(Is... is) noexcept
    {
      return span_type::data()[ access_type::index(is...) ];
    }
    //==============================================================================================
    //! @}
    //==============================================================================================
  };

  /// Retrieve size along the Ith dimension
  template<std::size_t I, typename T, auto... Os>
  constexpr auto dim(view<T,Os...> const& v) noexcept
  {
    if constexpr(I<view<T,Os...>::static_nbdims) return get<I>(v.shape());
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

