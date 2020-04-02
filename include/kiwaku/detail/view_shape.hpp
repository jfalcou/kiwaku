//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_DETAIL_VIEW_SHAPE_HPP_INCLUDED
#define KIWAKU_DETAIL_VIEW_SHAPE_HPP_INCLUDED

#include <kiwaku/shape.hpp>
#include <array>

namespace kwk::detail
{
  //================================================================================================
  // Default case is for when everything is static
  //================================================================================================
  template<auto ShapeOption, auto StrideOption>
  struct view_shape
  {
    using shape_type  = decltype(ShapeOption);
    using stride_type = decltype(StrideOption);

    constexpr std::size_t     size()          const noexcept  { return ShapeOption.numel();  }
    constexpr std::ptrdiff_t  count()         const noexcept  { return ShapeOption.numel();  }
    constexpr std::size_t     size(int dim)   const noexcept  { return ShapeOption[dim];     }
    constexpr std::ptrdiff_t  count(int dim)  const noexcept  { return ShapeOption[dim];     }

    template<typename... Int>
    constexpr std::ptrdiff_t index(Int... is) const noexcept { return StrideOption.index(is...); }

    constexpr auto shape()  const noexcept { return ShapeOption;  }
    constexpr auto stride() const noexcept { return StrideOption; }
  };

  //================================================================================================
  // Optimisation : 1D shape + unit stride
  //================================================================================================
  template<auto ShapeOption, auto StrideOption>
  requires(   ShapeOption.is_dynamic_option &&  ShapeOption.static_size == 1
          &&  StrideOption.is_unit_stride
          )
  struct  view_shape<ShapeOption, StrideOption>
  {
    using shape_type  = typename decltype(ShapeOption)::shape_type;
    using stride_type = typename decltype(StrideOption)::stride_type;

    constexpr view_shape( shape_type const& shp ) : shape_(shp) {}

    constexpr std::size_t     size()                    const noexcept  { return shape_[0];   }
    constexpr std::size_t     size(int dim)             const noexcept  { return shape_[dim]; }
    constexpr std::ptrdiff_t  count()                   const noexcept  { return shape_[0];   }
    constexpr std::ptrdiff_t  count(int dim)            const noexcept  { return shape_[dim]; }
    constexpr auto            index(std::ptrdiff_t is)  const noexcept  { return is;          }
    constexpr auto            shape()                   const noexcept  { return shape_;      }
    constexpr stride_type     stride()                  const noexcept  { return {};          }

    private:
    shape_type shape_;
  };

  //================================================================================================
  // Dynamic 2D shape with unit stride
  //================================================================================================
  template<auto ShapeOption, auto StrideOption>
  requires(   ShapeOption.is_dynamic_option &&  ShapeOption.static_size == 2
          &&  StrideOption.is_unit_stride
          )
  struct  view_shape<ShapeOption, StrideOption>
  {
    using shape_type  = typename decltype(ShapeOption)::shape_type;
    using stride_type = typename decltype(StrideOption)::stride_type;

    constexpr view_shape( shape_type const& shp ) : shape_(shp), numel_{shp.numel()} {}

    constexpr std::size_t     size()          const noexcept  { return numel_;  }
    constexpr std::size_t     size(int dim)   const noexcept  { return shape_[dim]; }

    constexpr std::ptrdiff_t  count()         const noexcept  { return numel_;  }
    constexpr std::ptrdiff_t  count(int dim)  const noexcept  { return shape_[dim]; }

    constexpr auto        shape()  const noexcept { return shape_;      }
    constexpr stride_type stride() const noexcept { return {shape_[0]}; }

    constexpr std::ptrdiff_t index(std::ptrdiff_t i0, std::ptrdiff_t i1) const noexcept
    {
      return i0 + i1*shape_[0];
    }

    private:
    shape_type      shape_;
    std::ptrdiff_t  numel_;
  };

  //================================================================================================
  // Dynamic nD shape with unit stride
  //================================================================================================
  template<auto ShapeOption, auto StrideOption>
  requires(   ShapeOption.is_dynamic_option &&  ShapeOption.static_size > 2
          &&  StrideOption.is_unit_stride
          )
  struct  view_shape<ShapeOption, StrideOption>
        : std::array<std::ptrdiff_t, ShapeOption.static_size-2>
  {
    using shape_type  = typename decltype(ShapeOption)::shape_type;
    using stride_type = typename decltype(StrideOption)::stride_type;
    using parent = std::array<std::ptrdiff_t, ShapeOption.static_size-2>;

    constexpr view_shape( shape_type const& shp ) : shape_(shp), numel_{shp.numel()}
    {
      parent::operator[](0) = shape_[0]*shape_[1];

      [&]<std::size_t... Idx>(std::index_sequence<Idx...> const&, auto const& shp)
      {
        ((parent::operator[](Idx+1) = parent::operator[](Idx) * shp[Idx+2]),...);
      }( std::make_index_sequence<ShapeOption.static_size-3>(), shape_ );
    }

    constexpr std::size_t     size()          const noexcept  { return numel_;  }
    constexpr std::size_t     size(int dim)   const noexcept  { return shape_[dim]; }

    constexpr std::ptrdiff_t  count()         const noexcept  { return numel_;  }
    constexpr std::ptrdiff_t  count(int dim)  const noexcept  { return shape_[dim]; }

    constexpr auto shape()  const noexcept { return shape_;  }
    constexpr auto stride() const noexcept
    {
      return  [&]<std::size_t... Idx>(std::index_sequence<Idx...> const&, auto const& shp)
              {
                return stride_type{shp[0], parent::operator[](Idx)...};
              }( std::make_index_sequence<ShapeOption.static_size-2>(), shape_);
    }

    template<typename I0, typename I1, typename... Is>
    constexpr std::ptrdiff_t index(I0 i0, I1 i1, Is... is) const noexcept
    {
      auto base = i0 + i1*shape_[0];
      auto l = linearize(base, std::make_index_sequence<sizeof...(Is)>(), is...);
      return l;
    }

    private:

    shape_type      shape_;
    std::ptrdiff_t  numel_;

    template<typename... Is, std::size_t... Idx>
    constexpr auto linearize( std::ptrdiff_t base
                            , std::index_sequence<Idx...> const&, Is... is
                            ) const noexcept
    {
      return (base + ... + (is*parent::operator[](Idx)) );
    };
  };

  //================================================================================================
  // Dynamic nD shape with non-unit stride ?
  //================================================================================================
  template<auto ShapeOption, auto StrideOption>
  requires( !StrideOption.is_unit_stride )
  struct  view_shape<ShapeOption, StrideOption>
  {
    using shape_type  = typename decltype(ShapeOption)::shape_type;
    using stride_type = typename decltype(StrideOption)::stride_type;

    constexpr view_shape( shape_type const& shp )
            : shape_(shp), stride_(shp), numel_{shp.numel()}
    {}

    constexpr std::size_t     size()          const noexcept  { return numel_;  }
    constexpr std::size_t     size(int dim)   const noexcept  { return shape_[dim]; }

    constexpr std::ptrdiff_t  count()         const noexcept  { return numel_;  }
    constexpr std::ptrdiff_t  count(int dim)  const noexcept  { return shape_[dim]; }

    constexpr auto shape()  const noexcept  { return shape_;  }
    constexpr auto stride() const noexcept  { return stride_; }

    template<typename... Is>
    constexpr std::ptrdiff_t index(Is... is) const noexcept { return stride_.index(is...); }

    private:

    shape_type      shape_;
    stride_type     stride_;
    std::ptrdiff_t  numel_;
  };
}

#endif
