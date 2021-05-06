//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_CONTAINER_ARRAY_HPP_INCLUDED
#define KIWAKU_CONTAINER_ARRAY_HPP_INCLUDED

#include <kiwaku/container/view.hpp>
#include <vector>

namespace kwk
{
  template< typename Type
          , typename Shape
          >
  struct array
  {
    using view_type         = view<Type,Shape>;
    using shape_type        = Shape;
    using reference         = typename view_type::reference;
    using const_reference   = typename view_type::const_reference;

    array() : storage_{}, view_{storage_.data(),storage_.size()} {}

    template<typename SomeShape>
    array(SomeShape const& s) : storage_(s.numel()), view_(storage_.data(),s)
    {}

    template<typename SomeShape> void resize(SomeShape const& s)
    {
      storage_.resize(s.numel());
      view_ = view{storage_.data(),shape_type(s)};
    }

    template<typename SomeShape> void reshape(SomeShape const& s)
    {
      assert( s.numel() == view_.size() && "[array::reshape] - Incompatible shape");
      view_ = view{storage_.data(),shape_type(s)};
    }

    view_type as_view() const { return static_cast<view_type>(*this); }

    template<typename... Int> std::enable_if_t<(std::is_integral_v<Int> && ...), const_reference>
    operator()(Int... is) const noexcept
    {
      return view_(is...);
    }

    template<typename... Int> std::enable_if_t<(std::is_integral_v<Int> && ...), reference>
    operator()(Int... is) noexcept
    {
      return view_(is...);
    }

    template< typename OtherShape, typename... Int
            , typename = std::enable_if_t<(std::is_integral_v<Int> && ...)>
            >
    auto operator() ( OtherShape const& other_shape
                    , Int... origins
                    ) const noexcept
    {
      return view_(other_shape,origins...);
    }

    constexpr std::size_t     size()          const noexcept { return view_.size();     }
    constexpr std::size_t     size(int dim)   const noexcept { return view_.size(dim);  }
    constexpr std::ptrdiff_t  count()         const noexcept { return view_.count();    }
    constexpr std::ptrdiff_t  count(int dim)  const noexcept { return view_.count(dim); }

    auto data()   const noexcept { return view_.data();   }
    constexpr auto shape()  const noexcept { return view_.shape();   }

    private:
    std::vector<Type> storage_;
    view<Type,Shape>  view_;
  };
}

#endif
