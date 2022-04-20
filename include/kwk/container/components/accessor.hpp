//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#pragma once
#include <kwk/detail/raberu.hpp>
#include <kwk/container/options.hpp>
#include <type_traits>
#include <utility>

namespace kwk::detail
{
  //================================================================================================
  // Order N general cases
  //================================================================================================
  template<auto Shape, auto Stride> struct accessor
  {
    using shape_type                  = std::remove_cvref_t<decltype(Shape)>;
    using stride_type                 = std::remove_cvref_t<decltype(Stride)>;
    static constexpr auto static_order = shape_type::static_order;

    constexpr auto  size()    const noexcept  { return shape_.numel(); }
    constexpr auto  shape()   const noexcept  { return shape_;         }
    constexpr auto  stride()  const noexcept  { return stride_;        }

    template<rbr::concepts::option... Opts>
    constexpr   accessor(auto const& tag, rbr::settings<Opts...> const& opts)
              : shape_ ( options::shape(tag,opts) )
              , stride_( options::stride(tag,opts) )
    {}

    constexpr auto index(auto i0)     const noexcept { return i0*get<0>(stride_);   }
    constexpr auto index(auto... is)  const noexcept { return stride_.index(is...); }

    constexpr void reshape( shape_type const& s ) noexcept
    {
      shape_  = s;
      stride_ = s.as_stride();
    }

    constexpr void reshape( shape_type const& s, stride_type const& st ) noexcept
    {
      shape_  = s;
      stride_ = st;
    }

    constexpr void swap( accessor& other ) noexcept
    {
      shape_.swap( other.shape_ );
      stride_.swap( other.stride_ );
    }

    shape_type    shape_;
    stride_type   stride_;
  };

  //================================================================================================
  // Order N shape is fully static: no contents, only computation
  //================================================================================================
  template<auto Shape, auto Stride>
  requires( Shape.is_fully_static )
  struct accessor<Shape,Stride>
  {
    using shape_type                    = std::remove_cvref_t<decltype(Shape)>;
    using stride_type                   = std::remove_cvref_t<decltype(Stride)>;
    static constexpr auto static_order = shape_type::static_order;

    constexpr auto  size()    const noexcept  { return Shape.numel(); }
    constexpr auto  shape()   const noexcept  { return Shape;         }
    constexpr auto  stride()  const noexcept  { return Stride;        }

    template<rbr::concepts::option... Opts>
    constexpr   accessor(auto const&, rbr::settings<Opts...> const& )
    {}

    constexpr void swap( accessor& ) noexcept {}

    template<std::integral... Is>
    constexpr auto index(Is... is) const noexcept { return Stride.index(is...); }
  };

  //================================================================================================
  // Optimization : runtime 1D shape + unit/implicit stride
  // Expected sizeof : sizeof(void*) + sizeof(shape[0])
  //================================================================================================
  template<auto Shape, auto Stride>
  requires( !Shape.is_fully_static && Shape.order() == 1 && Stride.is_unit )
  struct  accessor<Shape, Stride>
  {
    using shape_type                    = std::remove_cvref_t<decltype(Shape)>;
    using stride_type                   = std::remove_cvref_t<decltype(Stride)>;
    static constexpr auto static_order = shape_type::static_order;

    template<rbr::concepts::option... Opts>
    constexpr   accessor(auto const& tag, rbr::settings<Opts...> const& opts)
              : shape_ ( options::shape(tag,opts) )
    {}

    constexpr auto        size()                    const noexcept  { return get<0>(shape_);  }
    constexpr auto        shape()                   const noexcept  { return shape_;          }
    constexpr stride_type stride()                  const noexcept  { return {};              }
    constexpr auto        index(std::ptrdiff_t is)  const noexcept  { return is;              }

    constexpr void reshape( shape_type const& s ) { shape_ = s; }

    constexpr void swap( accessor& other ) noexcept
    {
      shape_.swap( other.shape_ );
    }

    shape_type shape_;
  };

  //================================================================================================
  // Optimization : runtime 2D shape + unit & implicit stride
  // Expected sizeof : sizeof(void*) + sizeof(shape)
  //================================================================================================
  template<auto Shape, auto Stride>
  requires(   !Shape.is_fully_static  && Shape.order() == 2
          &&  Stride.is_unit          && Stride.is_implicit
          )
  struct  accessor<Shape, Stride>
  {
    using shape_type                    = std::remove_cvref_t<decltype(Shape)>;
    using stride_type                   = std::remove_cvref_t<decltype(Stride)>;
    static constexpr auto static_order = shape_type::static_order;

    template<rbr::concepts::option... Opts>
    constexpr   accessor(auto const& tag, rbr::settings<Opts...> const& opts)
              : shape_ ( options::shape(tag,opts) )
    {}

    constexpr std::ptrdiff_t  size()    const noexcept  { return shape_.numel();              }
    constexpr auto            shape()   const noexcept  { return shape_;                      }
    constexpr auto            stride()  const noexcept  { return stride_type{unit_,get<0>(shape_)}; }

    constexpr void reshape( shape_type const& s ) { shape_ = s; }

    constexpr auto index(auto i0)           const noexcept { return i0; }
    constexpr auto index(auto i0, auto i1)  const noexcept { return i0 + i1*get<0>(shape_); }

    constexpr void swap( accessor& other ) noexcept { shape_.swap( other.shape_ ); }

    shape_type  shape_;
  };
}
