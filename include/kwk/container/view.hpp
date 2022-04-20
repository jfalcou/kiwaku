//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#pragma once

#include <kwk/algorithm/for_each.hpp>
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
  struct  view  : private detail::view_builder<Type,Os...>::metadata
                        , detail::view_builder<Type,Os...>::data_block
                        , detail::view_builder<Type,Os...>::accessor
  {
    using meta_t    = typename detail::view_builder<Type,Os...>::metadata;
    using span_t    = typename detail::view_builder<Type,Os...>::data_block;
    using access_t  = typename detail::view_builder<Type,Os...>::accessor;

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
            : meta_t { tag::view_{}, params }
            , span_t{ tag::view_{}, params }
            , access_t { tag::view_{}, params }
    {}

    //==============================================================================================
    //! @}
    //==============================================================================================

    //! Return the rank of the view
    constexpr auto rank() const noexcept{ return this->shape().rank(); }

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

    /// Swap the contents with another view
    void swap(view& other) noexcept
    {
      access_t::swap( static_cast<access_t&>(other) );
      span_t::swap( static_cast<span_t&>(other) );
    }

    /// Swap the contents of two views
    friend void swap(view& a, view& b) noexcept { a.swap(b); }

    /// Stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, view const& v)
    {
      auto spaces = meta_t::has_label ? "  " : "";
      auto lbl    = [&]() { if constexpr(meta_t::has_label) os << v.label() << ":\n"; };

      if constexpr( view::static_rank < 3)
      {
        lbl();
        for_each( [&](auto const& c, auto i0, auto... i)
                  {
                    if(i0 == 0) os << spaces << "[ ";
                    os << c(i0,i...) << " ";
                    if(i0 == dim<0>(c)-1) os << "]\n";
                  }
                , v
                );
      }
      else
      {
        lbl();
        for_each( [&](auto const& c, auto i0, auto i1, auto i2, auto... i)
                  {
                    if(i0 == 0)
                    {
                      if(i1 == 0 && i2 > 0) os << "\n";
                      os << spaces << "[ ";
                    }
                    os << c(i0,i1,i2,i...) << " ";
                    if(i0 == dim<0>(c)-1) os << "]\n";
                  }
                , v
                );
      }

      return os;
    }

    constexpr auto settings() const noexcept
    {
      return rbr::merge ( rbr::settings( source  = span_t::data()
                                            , size    = access_t::shape()
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
  view(O const&...) -> view<typename options::element<tag::view_,rbr::settings<O...>>::type,O{}...>;

  /// This deduction guide is provided for kwk::view to allow deduction from another view's settings
  template<rbr::concepts::option... O>
  view(rbr::settings<O...> const&)
      -> view<typename options::element<tag::view_,rbr::settings<O...>>::type, O{}...>;

  //================================================================================================
  //! @}
  //================================================================================================
}

