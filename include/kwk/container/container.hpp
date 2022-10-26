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
  template<auto Tag, typename Data, typename Access, typename MetaData>
  struct  container : private MetaData, Data, Access
  {
    using value_type        = typename Data::value_type;
    using reference         = typename Data::reference;
    using const_reference   = typename Data::const_reference;
    using pointer           = typename Data::pointer;
    using const_pointer     = typename Data::const_pointer;
    using container_kind    = decltype(Tag);

    static constexpr auto static_order    = Access::static_order;
    static constexpr auto has_label       = MetaData::has_label;

    constexpr container( container_kind )
            : MetaData{ }
            , Data    { }
            , Access  { }
    {}

    constexpr container( rbr::concepts::option auto const&... params )
            : container{ rbr::settings(Tag, params...) }
    {}

    constexpr container(rbr::concepts::settings auto const& params)
            : MetaData{ params }
            , Data    { detail::block(params) }
            , Access  { params }
    {}

    static constexpr  auto kind()         noexcept  { return Tag;      }
    constexpr         auto order() const  noexcept  { return this->shape().order(); }
    constexpr         auto numel() const  noexcept  { return this->shape().numel(); }
    constexpr         auto empty() const  noexcept  { return this->size() == 0;     }

    using MetaData::label;

    constexpr auto settings() const noexcept
    {
      if constexpr(has_label)
      {
        return rbr::settings(Tag,kwk::label = label(),source = get_data(),this->shape());
      }
      else
      {
        return rbr::settings(Tag,source = get_data(),this->shape());
      }
    }

    friend std::ostream& operator<<(std::ostream& os, container const& v)
    {
      auto spaces = has_label ? "  " : "";
      auto lbl    = [&]() { if constexpr(has_label) os << v.label() << ":\n"; };

      lbl();
      if( v.empty() )
      {
        return os << spaces << "[ ]";
      }

      auto shp = v.shape();
      if constexpr( container::static_order < 3)
      {
        for_each_index( [&](auto e, auto i0, auto...)
                        {
                          if(i0 == 0) os << spaces << "[ ";
                          os << e << ' ';
                          if(i0 == get<0>(shp)-1) os << "]\n";
                        }
                      , v
                      );
      }
      else
      {
        for_each_index( [&](auto e, auto i0, auto i1, auto i2, auto... )
                        {
                          if(i0 == 0)
                          {
                            if(i1 == 0 && i2 > 0) os << '\n';
                            os << spaces << "[ ";
                          }
                          os << e << ' ';
                          if(i0 == get<0>(shp)-1) os << "]\n";
                        }
                      , v
                      );
      }

      return os;
    }

    template<std::integral... Is>
    requires(sizeof...(Is) == static_order) const_reference operator()(Is... is) const noexcept
    {
      return data(static_cast<Data const&>(*this))[ Access::index(is...) ];
    }

    template<std::integral... Is>
    requires(sizeof...(Is) == static_order) reference operator()(Is... is) noexcept
    {
      return data(static_cast<Data&>(*this))[ Access::index(is...) ];
    }

    constexpr auto get_data() const  noexcept { return data(static_cast<Data const&>(*this)); }
    constexpr auto get_data()        noexcept { return data(static_cast<Data&>(*this)); }
  };

  template<std::size_t I, auto Tag, typename D, typename A, typename M>
  constexpr auto dim(container<Tag,D,A,M> const& v) noexcept
  {
    if constexpr(I<container<Tag,D,A,M>::static_order) return get<I>(v.shape());
    else return 1;
  }
}

