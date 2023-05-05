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
  template<auto Tag, typename Builder>
  struct  container : private Builder::metadata, Builder::memory, Builder::accessor
  {
    using data_t            = typename Builder::memory;
    using access_t          = typename Builder::accessor;
    using meta_t            = typename Builder::metadata;
    using value_type        = typename data_t::value_type;
    using reference         = typename data_t::reference;
    using const_reference   = typename data_t::const_reference;
    using pointer           = typename data_t::pointer;
    using const_pointer     = typename data_t::const_pointer;
    using shape_type        = typename access_t::shape_type;
    using container_kind    = decltype(Tag);

    static constexpr std::int32_t static_order  = access_t::static_order;
    static constexpr bool         has_label     = meta_t::has_label;

    constexpr container( container_kind ) noexcept
            : meta_t{}, data_t{}, access_t{}
    {}

    constexpr container( rbr::concepts::option auto const&... params )
            : container{ rbr::merge(rbr::settings(params...), rbr::settings(Tag)) }
    {}

    constexpr container(rbr::concepts::settings auto const& params)
            : meta_t{ params }, data_t{ params }, access_t{ params }
    {}

    static constexpr  auto          kind()         noexcept  { return Tag;      }
    constexpr         std::int32_t  order() const  noexcept  { return this->shape().order(); }
    constexpr         auto          numel() const  noexcept  { return this->shape().numel(); }
    constexpr         bool          empty() const  noexcept  { return this->size() == 0;     }

    using meta_t::label;

    void swap(container& other) noexcept
    {
      meta_t::swap(other);
      data_t::swap(other);
      access_t::swap(other);
    }

    friend void swap(container& a,container& b) noexcept { a.swap(b); }

    static constexpr auto archetype() noexcept
    {
      return rbr::settings(as<value_type>, shape_type{});
    }

    static constexpr auto archetype(auto tag) noexcept
    {
      return rbr::settings(tag, as<value_type>, shape_type{});
    }

    constexpr auto settings() const noexcept
    {
      return rbr::settings(source = pointer(get_data()), this->shape());
    }

    constexpr auto settings(auto tag) const noexcept
    {
      return rbr::settings(tag, source = pointer(get_data()), this->shape());
    }

    friend std::ostream& operator<<(std::ostream& os, container const& v)
    {
      auto spaces = has_label ? "  " : "";
      auto lbl    = [&]() { if constexpr(has_label) os << v.label() << ":\n"; };

      lbl();
      if( v.empty() ) return os << spaces << "[ ]";

      auto shp = v.shape();
      for_each_index( [&](auto e, auto... is)
                      {
                        auto idx = kumi::tuple{is...};
                        if constexpr( container::static_order >= 3)
                        {
                          auto panel = kumi::extract(idx,kumi::index<container::static_order-2>);
                          if(     panel == kumi::tuple{0,0}
                              &&  kumi::get<container::static_order-3>(idx) > 0
                            )
                          os << '\n';
                        }

                        if(back(idx) == 0) os << spaces << "[ ";
                        os << e << ' ';
                        if(back(idx) == kumi::back(shp)-1) os << "]\n";
                      }
                    , v
                    );

      return os;
    }

    template<kumi::sized_product_type<static_order> Pos>
    const_reference operator()(Pos p) const noexcept
    {
      return kumi::apply([&](auto... i) -> const_reference { return (*this)(i...); }, p);
    }

    template<kumi::sized_product_type<static_order> Pos>
    reference operator()(Pos p) noexcept
    {
      return kumi::apply([&](auto... i) -> reference { return (*this)(i...); }, p);
    }

    template<std::integral... Is>
    requires(sizeof...(Is) == static_order) const_reference operator()(Is... is) const noexcept
    {
      return data(static_cast<data_t const&>(*this))[ access_t::index(is...) ];
    }

    template<std::integral... Is>
    requires(sizeof...(Is) == static_order) reference operator()(Is... is) noexcept
    {
      return data(static_cast<data_t&>(*this))[ access_t::index(is...) ];
    }

    constexpr auto get_data() const  noexcept { return data(static_cast<data_t const&>(*this)); }
    constexpr auto get_data()        noexcept { return data(static_cast<data_t&>(*this)); }
  };

  template<std::size_t I, auto Tag, typename B>
  constexpr auto dim(container<Tag,B> const& v) noexcept
  {
    if constexpr(I<container<Tag,B>::static_order) return get<I>(v.shape());
    else return 1;
  }
}

