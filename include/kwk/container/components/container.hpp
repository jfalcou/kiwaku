//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#pragma once

#include <kwk/algorithm/for_each.hpp>
#include <kwk/container/options.hpp>
#include <kwk/detail/builder.hpp>
#include <kwk/detail/raberu.hpp>
#include <kwk/options.hpp>
#include <type_traits>

namespace kwk
{
  template<typename Tag, typename Type, auto... Os>
  struct  container : private detail::builder<Tag,Type,Os...>::metadata
                            , detail::builder<Tag,Type,Os...>::data_block
                            , detail::builder<Tag,Type,Os...>::accessor
  {
    using meta_t    = typename detail::builder<Tag,Type,Os...>::metadata;
    using span_t    = typename detail::builder<Tag,Type,Os...>::data_block;
    using access_t  = typename detail::builder<Tag,Type,Os...>::accessor;

    static constexpr auto tag = Tag{};

    using value_type        = Type;
    using reference         = typename span_t::reference;
    using const_reference   = typename span_t::const_reference;
    using pointer           = typename span_t::pointer;
    using const_pointer     = typename span_t::const_pointer;

    static constexpr auto static_order = access_t::static_order;
    static constexpr auto has_label    = meta_t::has_label;

    constexpr container( rbr::concepts::option auto const&... params )
            : container{ rbr::settings(params...) }
    {}

    constexpr container(rbr::concepts::settings auto const& params)
            : meta_t   { tag, params }
            , span_t   { tag, params }
            , access_t { tag, params }
    {}

    constexpr auto order() const noexcept{ return this->shape().order(); }

    friend std::ostream& operator<<(std::ostream& os, container const& v)
    {
      auto spaces = has_label ? "  " : "";
      auto lbl    = [&]() { if constexpr(has_label) os << v.label() << ":\n"; };

      if constexpr( container::static_order < 3)
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
  };

  template<std::size_t I, typename T, typename Tag, auto... Os>
  constexpr auto dim(container<Tag,T,Os...> const& v) noexcept
  {
    if constexpr(I<container<Tag,T,Os...>::static_order) return get<I>(v.shape());
    else return 1;
  }

  /// Retrieve the first valid index along the Ith dimension
  template<std::size_t I, typename T, typename Tag, auto... Os>
  constexpr auto first(container<Tag,T,Os...> const& v) noexcept
  {
    auto bi = options::base_index(v.tag, rbr::settings{Os...});
    return get<I>(bi.template as_position<container<Tag,T,Os...>::static_order>());
  }

  /// Retrieve the last valid index along the Ith dimension
  template<std::size_t I, typename T, typename Tag, auto... Os>
  constexpr auto last(container<Tag,T,Os...> const& v) noexcept
  {
    return first<I>(v) + dim<I>(v) - 1;
  }
}

