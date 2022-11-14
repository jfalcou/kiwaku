//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/algorithm/for_each.hpp>
#include <kwk/container/pick.hpp>
#include <kwk/detail/container/builder.hpp>
#include <kwk/detail/memory/block.hpp>
#include <type_traits>

namespace kwk
{
  template<auto Tag, auto... Os>
  struct  container : private detail::builder<rbr::settings{Tag,Os...}>::metadata
                            , detail::builder<rbr::settings{Tag,Os...}>::data_block
                            , detail::builder<rbr::settings{Tag,Os...}>::accessor
  {
    using meta_t    = typename detail::builder<rbr::settings{Tag,Os...}>::metadata;
    using span_t    = typename detail::builder<rbr::settings{Tag,Os...}>::data_block;
    using access_t  = typename detail::builder<rbr::settings{Tag,Os...}>::accessor;

    constexpr container( rbr::concepts::option auto const&... params )
            : container{ rbr::settings(Tag, params...) }
    {}

    constexpr container(rbr::concepts::settings auto const& params)
            : meta_t   { params }
            , span_t   { detail::block(params) }
            , access_t { params }
    {}

    using value_type        = typename span_t::value_type;
    using reference         = typename span_t::reference;
    using const_reference   = typename span_t::const_reference;
    using pointer           = typename span_t::pointer;
    using const_pointer     = typename span_t::const_pointer;

    static constexpr auto static_order = access_t::static_order;
    static constexpr auto has_label    = meta_t::has_label;

    constexpr auto order() const noexcept { return this->shape().order(); }
    constexpr auto numel() const noexcept { return this->shape().numel(); }
    constexpr auto empty() const noexcept { return this->size() == 0; }

    using meta_t::label;

    friend std::ostream& operator<<(std::ostream& os, container const& v)
    {
      auto spaces = has_label ? "  " : "";
      auto lbl    = [&]() { if constexpr(has_label) os << v.label() << ":\n"; };

      lbl();
      if( v.empty() )
      {
        return os << spaces << "[ ]";
      }

      if constexpr( container::static_order < 3)
      {
        for_each( [&](auto const& c, auto i0, auto... i)
                  {
                    if(i0 == 0) os << spaces << "[ ";
                    os << c(i0,i...) << ' ';
                    if(i0 == get<0>(v.shape())-1) os << "]\n";
                  }
                , v
                );
      }
      else
      {
        for_each( [&](auto const& c, auto i0, auto i1, auto i2, auto... i)
                  {
                    if(i0 == 0)
                    {
                      if(i1 == 0 && i2 > 0) os << '\n';
                      os << spaces << "[ ";
                    }
                    os << c(i0,i1,i2,i...) << ' ';
                    if(i0 == get<0>(v.shape())-1) os << "]\n";
                  }
                , v
                );
      }

      return os;
    }

    template<std::integral... Is>
    requires(sizeof...(Is) == static_order) const_reference operator()(Is... is) const noexcept
    {
      return data(static_cast<span_t const&>(*this))[ access_t::index(is...) ];
    }

    template<std::integral... Is>
    requires(sizeof...(Is) == static_order) reference operator()(Is... is) noexcept
    {
      return data(static_cast<span_t&>(*this))[ access_t::index(is...) ];
    }

    constexpr auto get() const  noexcept { return data(static_cast<span_t const&>(*this)); }
    constexpr auto get()        noexcept { return data(static_cast<span_t&>(*this)); }
  };

  template<std::size_t I, auto Tag, auto... Os>
  constexpr auto dim(container<Tag, Os...> const& v) noexcept
  {
    if constexpr(I<container<Tag,Os...>::static_order) return get<I>(v.shape());
    else return 1;
  }
}

