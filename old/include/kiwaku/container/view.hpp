//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_CONTAINER_VIEW_HPP_INCLUDED
#define KIWAKU_CONTAINER_VIEW_HPP_INCLUDED

#include <kiwaku/shape.hpp>
#include <kiwaku/detail/ct_helpers.hpp>
#include <kiwaku/detail/view_span.hpp>
#include <kiwaku/detail/view_shape.hpp>
#include <kiwaku/misc/unit_stride.hpp>
#include <kiwaku/misc/stride.hpp>
#include <iterator>

namespace kwk
{
  template< typename Type, auto ShapeOption
          , auto StrideOption = ShapeOption.as_stride()
          >
  struct view : detail::view_shape<ShapeOption,StrideOption>, detail::view_span<Type*>
  {
    //============================================================================================
    // Extract a view from a view when the number of indexers is within the number of Dimensions
    //============================================================================================
    template<typename Index0, typename... Indexer >
    auto operator()( Index0 const& i0, Indexer const&... is ) const noexcept
    requires(   ( !std::is_integral_v<Index0> || ... || !std::is_integral_v<Indexer> )
            &&  ((sizeof...(Indexer)+1) <= shape_type::static_size)
            )
    {
      //============================================================================================
      // We compute a shape from the indexers list but without taking stride into account
      // This enables nD view to be reshaped as (n+k)D views
      //============================================================================================
      auto base   = this->shape();
      auto sth { [&]<std::size_t... X>( std::index_sequence<X...> const&, auto... p)
                  {
                    return kwk::shape{partial_reshape(i0,0,base), partial_reshape(p,X+1,base)...};
                  }( std::make_index_sequence<sizeof...(Indexer)>{}, is...)
                };

      //============================================================================================
      // This is the actual size with stepping taken into account
      //============================================================================================
      auto sh = shape_base::reshape(i0,is...);

      //============================================================================================
      // If Index0 is not of the form _(b,s,e), we can use the stride directly
      //============================================================================================
      if constexpr( !std::is_same_v<Index0, indexer> )
      {
        auto st = sth.as_stride();
        st.restride(i0,is...);

        using stride_t = detail::explicit_<stride_type>;
        using that_t = view<Type, _nD<decltype(sh)::static_size>, stride_t{}>;

        return that_t(data() + this->stride().offset(i0,is...), sh, stride_t(this->stride()));
      }
      else
      {
        kwk::stride<1+sizeof...(Indexer)> st(sth.as_stride());
        st.restride(i0,is...);

        using stride_t = detail::explicit_<decltype(st)>;
        using that_t = view<Type, _nD<decltype(sh)::static_size>, stride_t{}>;

        return that_t(data() + st.offset(i0,is...), sh, stride_t(st));
      }
    }

    //============================================================================================
    // Extract a view from a view when the number of indexers is above the number of Dimensions
    // In this case, the extra dimension must be:
    //  * _
    //  * an integer equals to the base index
    // Other cases should raise an assert or static_assert accordingly
    //============================================================================================
    template<typename Index0, typename... Indexer >
    auto operator()( Index0 const& i0, Indexer const&... is ) const noexcept
    requires(   ( !std::is_integral_v<Index0> || ... || !std::is_integral_v<Indexer> )
            &&  ((sizeof...(Indexer)+1) > shape_type::static_size)
            )
    {
      //============================================================================================
      // We compute a shape from the indexers list but without taking stride into account
      // This enables nD view to be reshaped as (n+k)D views
      //============================================================================================
      auto base   = this->shape();
      auto sth { [&]<std::size_t... X>( std::index_sequence<X...> const&, auto... p)
                  {
                    return kwk::shape{partial_reshape(i0,0,base), partial_reshape(p,X+1,base)...};
                  }( std::make_index_sequence<sizeof...(Indexer)>{}, is...)
                };

      //============================================================================================
      // This is the actual size with stepping taken into account
      //============================================================================================
      auto sh = shape_base::reshape(i0,is...);

      //============================================================================================
      // If Index0 is not of the form _(b,s,e), we can use the stride directly
      //============================================================================================
      if constexpr( !std::is_same_v<Index0, indexer> )
      {
        auto st = sth.as_stride();
        st.restride(i0,is...);

        using stride_t = detail::explicit_<decltype(st)>;
        using that_t = view<Type, _nD<decltype(sh)::static_size>, stride_t{}>;

        return that_t(data() + st.offset(i0,is...), sh, stride_t(st));
      }
      else
      {
        kwk::stride<1+sizeof...(Indexer)> st(sth.as_stride());
        st.restride(i0,is...);

        using stride_t = detail::explicit_<decltype(st)>;
        using that_t = view<Type, _nD<decltype(sh)::static_size>, stride_t{}>;

        return that_t(data() + st.offset(i0,is...), sh, stride_t(st));
      }
    }

    auto reshape(shape_type const& shp) noexcept requires(ShapeOption.is_dynamic_option)
    {
      assert( shp.numel() == span_base::size() && "[view::reshape] - Incompatible shape");
      shape_base::resize(shp);
    }
  };


}

#endif
