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

    static constexpr auto tag = tag::view_{};

    /// Underlying value type
    using value_type        = Type;

    /// Associated reference type
    using reference         = typename span_t::reference;

    /// Associated reference to const type
    using const_reference   = typename span_t::const_reference;

    /// Associated pointer type
    using pointer           = typename span_t::pointer;

    /// Associated const pointer type
    using const_pointer     = typename span_t::const_pointer;

    /// Compile-time @ref glossary-order
    static constexpr auto static_order = access_t::static_order;

    //==============================================================================================
    //! @name Constructors
    //! @{
    //==============================================================================================

    /// Construct a view from a list of options
    constexpr view( rbr::concepts::option auto const&... params )
            : view{ rbr::settings(params...) }
    {}

    /// Construct a view from a settings descriptor
    constexpr view(rbr::concepts::settings auto const& params)
            : meta_t   { tag, params }
            , span_t   { tag, params }
            , access_t { tag, params }
    {}

    //==============================================================================================
    //! @}
    //==============================================================================================

    //! Return the order of the view
    constexpr auto order() const noexcept{ return this->shape().order(); }

    /// Stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, view const& v)
    {
      auto spaces = meta_t::has_label ? "  " : "";
      auto lbl    = [&]() { if constexpr(meta_t::has_label) os << v.label() << ":\n"; };

      if constexpr( view::static_order < 3)
      {
        lbl();
        for_each( [&](auto const& c, auto i0, auto... i)
                  {
                    if(i0 == first<0>(v)) os << spaces << "[ ";
                    os << c(i0,i...) << " ";
                    if(i0 == last<0>(v)) os << "]\n";
                  }
                , v
                );
      }
      else
      {
        lbl();
        for_each( [&](auto const& c, auto i0, auto i1, auto i2, auto... i)
                  {
                    if(i0 == first<0>(v))
                    {
                      if(i1 == first<1>(v) && i2 > first<2>(v)) os << "\n";
                      os << spaces << "[ ";
                    }
                    os << c(i0,i1,i2,i...) << " ";
                    if(i0 == last<0>(v)) os << "]\n";
                  }
                , v
                );
      }

      return os;
    }

    constexpr auto settings() const noexcept
    {
      // Retrieve all basic options + correct shape value
      auto const base   = rbr::settings(Os...);
      auto const opts   = rbr::merge( rbr::settings(size = access_t::shape())
                                    , base
                                    );

      // Retrieve potential offset to rebuild proper view target
      auto const offset = options::offset(tag, opts);

      if constexpr(meta_t::has_label)
      {
        return rbr::merge ( rbr::settings( source = span_t::data() + offset
                                         , label = meta_t::label()
                                         )
                          , opts
                          );
      }
      else
      {
        return rbr::merge ( rbr::settings(source = span_t::data() + offset)
                          , opts
                          );
      }
    }

    //==============================================================================================
    //! @name Indexing operations
    //! @{
    //==============================================================================================
    template<std::integral... Is>
    requires(sizeof...(Is) <= static_order)
    const_reference operator()(Is... is) const noexcept
    {
      return span_t::data()[ access_t::index(is...) ];
    }

    template<std::integral... Is>
    requires(sizeof...(Is) <= static_order)
    reference operator()(Is... is) noexcept
    {
      return span_t::data()[ access_t::index(is...) ];
    }

    //==============================================================================================
    //! @}
    //==============================================================================================
  };

  /// Retrieve number of elements along the Ith dimension
  template<std::size_t I, typename T, auto... Os>
  constexpr auto dim(view<T,Os...> const& v) noexcept
  {
    if constexpr(I<view<T,Os...>::static_order) return get<I>(v.shape());
    else return 1;
  }

  /// Retrieve the first valid index along the Ith dimension
  template<std::size_t I, typename T, auto... Os>
  constexpr auto first(view<T,Os...> const& v) noexcept
  {
    auto bi = options::base_index(v.tag, rbr::settings{Os...});
    return get<I>(bi.template as_position<view<T,Os...>::static_order>());
  }

  /// Retrieve the last valid index along the Ith dimension
  template<std::size_t I, typename T, auto... Os>
  constexpr auto last(view<T,Os...> const& v) noexcept
  {
    return first<I>(v) + dim<I>(v) - 1;
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

