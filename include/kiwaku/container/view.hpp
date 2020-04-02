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
#include <kiwaku/detail/view_span.hpp>
#include <kiwaku/detail/view_shape.hpp>
#include <iterator>
#include <array>

namespace kwk
{
  template< typename Type, auto ShapeOption
          , auto StrideOption = ShapeOption.as_stride()
          >
  struct view : detail::view_shape<ShapeOption,StrideOption>, detail::view_span<Type*>
  {
    using span_base         = detail::view_span<Type*>;
    using shape_base        = detail::view_shape<ShapeOption,StrideOption>;
    using shape_type        = typename shape_base::shape_type;
    using pointer           = typename span_base::pointer;
    using reference         = typename span_base::reference;
    using const_reference   = typename span_base::const_reference;
    using stride_type       = typename shape_base::stride_type;

    view( pointer external ) noexcept
          requires( !ShapeOption.is_dynamic_option)
        : shape_base(), span_base(external)
    {}

    view( pointer external, shape_type const& shp ) noexcept
          requires(ShapeOption.is_dynamic_option)
        : shape_base(shp), span_base(external)
    {}

    template<typename Seq>
    view(Seq&& seq) noexcept
        requires requires (Seq&& s) { std::forward<Seq>(s).data(); }
                      &&  ( !ShapeOption.is_dynamic_option )
      : shape_base(), span_base(std::forward<Seq>(seq).data())
     {}

    template<typename Seq>
    view(Seq&& seq, shape_type const& shp ) noexcept
        requires requires (Seq&& s) { std::forward<Seq>(s).data(); }
                      &&  ( ShapeOption.is_dynamic_option )
      : shape_base(shp), span_base(std::forward<Seq>(seq).data())
    {}

    using span_base::data;
    using shape_base::shape;
    using shape_base::stride;
    using shape_base::size;
    using shape_base::count;

    template<typename... Int> std::enable_if_t<(std::is_integral_v<Int> && ...), const_reference>
    operator()(Int... is) const noexcept
    {
      return this->data()[ this->index(is...) ];
    }

    template<typename... Int> std::enable_if_t<(std::is_integral_v<Int> && ...), reference>
    operator()(Int... is) noexcept
    {
      return this->data()[ this->index(is...) ];
    }

    // template< typename OtherShape
    //         , typename... Int
    //         , typename = std::enable_if_t<(std::is_integral_v<Int> && ...)>
    //         >
    // auto operator() ( OtherShape const& other_shape
    //                 , Int... origins
    //                 ) const noexcept
    // {
    //   auto offset = this->stride_.index(origins...);
    //   return view<Type,OtherShape,stride_type>( this->data_ + offset, other_shape, this->stride_);
    // }

    // view( pointer external, shape_type const& shp, Stride const& str ) noexcept
    //     : shape_base(shp,str), span_base(external, shp.numel())
    // {}

    auto reshape(shape_type const& shp) noexcept requires(ShapeOption.is_dynamic_option)
    {
      assert( shp.numel() == span_base::size() && "[view::reshape] - Incompatible shape");
      shape_base::resize(shp);
    }
  };

  template<typename T, typename S> view(T*,S const& ) -> view<T,_nD<S::static_size>>;

  template<typename C, typename S>
  view(C&,S const&) ->  view< std::remove_reference_t<decltype(*std::declval<C>().data())>
                            , _nD<S::static_size>
                            >;

  template<typename C, typename S>
  view(C const&,S const&) ->  view< std::remove_reference_t<decltype(*std::declval<C>().data())>
                                  , _nD<S::static_size>
                                  >;
}

#endif
