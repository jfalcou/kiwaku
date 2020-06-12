//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_DETAIL_CONTAINER_VIEW_ACCESS_HPP_INCLUDED
#define KIWAKU_DETAIL_CONTAINER_VIEW_ACCESS_HPP_INCLUDED

#include <kiwaku/detail/container/stride_helpers.hpp>
#include <kiwaku/detail/container/linearize.hpp>
#include <kiwaku/detail/ct_helpers.hpp>
#include <array>

namespace kwk::detail
{
  //================================================================================================
  // view_access centralizes all the members dealing with the view shape & extent
  // It is heavily optimized for size in all cases to be able to limit the footprint of view and
  // to enable pass-by-value semantic at no costs.
  //================================================================================================
  template<auto Shape, auto Stride> struct view_access;

  //================================================================================================
  // view_access : Full static case - no storage for anything
  //================================================================================================
  template<auto Shape, auto Stride>
  requires( !Shape.is_dynamic && !Stride.is_dynamic )
  struct  view_access<Shape, Stride>
  {
    using shape_type  = decltype(Shape);
    using stride_type = decltype(Stride);

    constexpr std::size_t     size()          const noexcept  { return Shape.numel(); }
    constexpr std::ptrdiff_t  count()         const noexcept  { return Shape.numel(); }
    constexpr std::size_t     size(int dim)   const noexcept  { return Shape[dim];    }
    constexpr std::ptrdiff_t  count(int dim)  const noexcept  { return Shape[dim];    }
    constexpr auto shape()                    const noexcept  { return Shape;         }
    constexpr auto stride()                   const noexcept  { return Stride;        }

    template<typename... Int>
    constexpr std::ptrdiff_t index(Int... is) const noexcept { return Stride.index(is...); }

    void swap( view_access& other ) noexcept {}
  };

  //================================================================================================
  // Optimization : runtime 1D shape + unit stride
  // Expected sizeof : sizeof(void*) + sizeof(std::ptrdiff_t)
  //================================================================================================
  template<auto Shape, auto Stride>
  requires( Shape.is_dynamic && Shape.static_size == 1 && Stride.is_unit )
  struct  view_access<Shape, Stride>
  {
    using shape_type  = typename decltype(Shape)::shape_type;
    using stride_type = typename decltype(Stride)::stride_type;

    constexpr view_access( shape_type const& shp ) : shape_(shp) {}

    constexpr std::ptrdiff_t  count()                   const noexcept  { return shape_[0];   }
    constexpr std::ptrdiff_t  count(int dim)            const noexcept  { return shape_[dim]; }
    constexpr std::size_t     size()                    const noexcept  { return count();     }
    constexpr std::size_t     size(int dim)             const noexcept  { return count(dim);  }
    constexpr auto            shape()                   const noexcept  { return shape_;      }
    constexpr stride_type     stride()                  const noexcept  { return {};          }
    constexpr auto            index(std::ptrdiff_t is)  const noexcept  { return is;          }

    constexpr void reshape( shape_type const& s ) { shape_ = s; }

    void swap( view_access& other ) noexcept
    {
      shape_.swap( other.shape_ );
    }

    private:
    shape_type shape_;
  };

  //================================================================================================
  // Optimization : runtime 2D shape + unit stride
  // We don't store a complete unit stride as its only non-trivial value is already in shape[0].
  //================================================================================================
  template<auto Shape, auto Stride>
  requires( Shape.is_dynamic && Shape.static_size == 2 && Stride.is_unit )
  struct  view_access<Shape, Stride>
  {
    using shape_type  = typename decltype(Shape)::shape_type;
    using stride_type = typename decltype(Stride)::stride_type;

    constexpr view_access( shape_type const& shp ) : shape_(shp), numel_{shp.numel()} {}

    constexpr std::ptrdiff_t  count()         const noexcept  { return numel_;                  }
    constexpr std::ptrdiff_t  count(int dim)  const noexcept  { return shape_[dim];             }
    constexpr std::size_t     size()          const noexcept  { return count();                 }
    constexpr std::size_t     size(int dim)   const noexcept  { return count(dim);              }
    constexpr auto            shape()         const noexcept  { return shape_;                  }
    constexpr decltype(auto)  stride()        const noexcept  { return stride_type{shape_[0]};  }

    constexpr void reshape( shape_type const& s ) { shape_ = s; }

    constexpr std::ptrdiff_t index(std::ptrdiff_t i0, std::ptrdiff_t i1) const noexcept
    {
      return i0 + i1*this->shape_[0];
    }

    constexpr std::ptrdiff_t index(std::ptrdiff_t i0) const noexcept
    {
      return i0;
    }

    void swap( view_access& other ) noexcept
    {
      shape_.swap( other.shape_ );
      std::swap(numel_, other.numel_);
    }

    protected:
    shape_type      shape_;
    std::ptrdiff_t  numel_;
  };

  //================================================================================================
  // Optimization : runtime nD shape + unit stride
  // The stride data are stored in the minimalist array required.
  // The full stride object is reconstructed on demand.
  //================================================================================================
  template<auto Shape, auto Stride>
  requires( Shape.is_dynamic && Shape.static_size > 2 && Stride.is_unit )
  struct  view_access<Shape, Stride>
        : private std::array<std::ptrdiff_t, Shape.static_size-2>
  {
    using parent        = std::array<std::ptrdiff_t, Shape.static_size-2>;
    using shape_type    = typename decltype(Shape)::shape_type;
    using stride_type   = typename decltype(Stride)::stride_type;

    constexpr view_access( shape_type const& shp ) : shape_(shp), numel_{shp.numel()}
    {
      parent::operator[](0) = shape_[0]*shape_[1];

      detail::constexpr_for<Shape.static_size-3>
      (
        [&]<std::ptrdiff_t I>( std::integral_constant<std::ptrdiff_t,I> const&)
        {
          parent::operator[](I+1) = parent::operator[](I) * shp[I+2];
        }
      );
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
                return stride_type{unit_type{}, shp[0], parent::operator[](Idx)...};
              }( std::make_index_sequence<Shape.static_size-2>(), shape_);
    }

    constexpr void reshape( shape_type const& s ) { shape_ = s; }

    template<typename I0> constexpr std::ptrdiff_t index(I0 i0) const noexcept { return i0; }

    template<typename I0, typename I1, typename... Is>
    constexpr std::ptrdiff_t index(I0 i0, I1 i1, Is... is) const noexcept
    {
      // All indexing starts with this base
      auto base = i0 + i1*shape_[0];

      if constexpr(sizeof...(Is)>0)
      {
        return  base + linearize( std::make_index_sequence<sizeof...(Is)>()
                                , static_cast<parent const&>(*this)
                                , is...
                                );
      }
      else
      {
        return base;
      }
    }

    void swap( view_access& other ) noexcept
    {
      shape_.swap( other.shape_ );
      std::swap(numel_, other.numel_);
    }

    protected:
    shape_type      shape_;
    std::ptrdiff_t  numel_;
  };
}

#endif
