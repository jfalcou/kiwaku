//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once
#include <kwk/detail/raberu.hpp>
#include <kwk/container/pick.hpp>
#include <type_traits>
#include <utility>

namespace kwk::detail
{
  //================================================================================================
  // Order N general cases
  //================================================================================================
  template<auto Shape, auto Stride> struct accessor
  {
    using shape_type                   = std::remove_cvref_t<decltype(Shape)>;
    using stride_type                  = std::remove_cvref_t<decltype(Stride)>;
    static constexpr auto static_order = shape_type::static_size;

    constexpr auto  size()    const noexcept  { return shape_.numel(); }
    constexpr auto  shape()   const noexcept  { return shape_;         }
    constexpr auto  stride()  const noexcept  { return stride_;        }

    constexpr   accessor(): shape_ (), stride_() {}

    constexpr   accessor(rbr::concepts::settings auto const& opts)
              : shape_ ( pick(kwk::size   ,opts) )
              , stride_( pick(kwk::strides,opts) )
    {}

    constexpr auto index(auto... is)  const noexcept { return stride_.linearize(is...); }

    constexpr void reshape( shape_type const& s ) noexcept
    {
      shape_  = s;
      stride_ = as_stride(s);
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
    static constexpr auto static_order  = shape_type::static_size;

    constexpr auto  size()    const noexcept  { return Shape.numel(); }
    constexpr auto  shape()   const noexcept  { return Shape;         }
    constexpr auto  stride()  const noexcept  { return Stride;        }

    constexpr       accessor()  {}
    constexpr       accessor(rbr::concepts::settings auto const&) noexcept {}
    constexpr void  swap( accessor& ) noexcept {}

    constexpr auto index(std::integral auto... is) const noexcept { return Stride.linearize(is...); }
  };

  //================================================================================================
  // Optimization : runtime 1D shape + unit stride
  // Expected sizeof : sizeof(void*) + sizeof(shape[0])
  //================================================================================================
  template<auto Shape, auto Stride>
  requires( !Shape.is_fully_static && Shape.order() == 1 && Stride.is_unit )
  struct  accessor<Shape, Stride>
  {
    using shape_type                    = std::remove_cvref_t<decltype(Shape)>;
    using stride_type                   = std::remove_cvref_t<decltype(Stride)>;
    static constexpr auto static_order  = shape_type::static_size;

    constexpr   accessor() : shape_{} {}
    constexpr   accessor(rbr::concepts::settings auto const& opts)
              : shape_ ( pick(kwk::size,opts) )
    {}

    constexpr auto        size()          const noexcept  { return get<0>(shape_);  }
    constexpr auto        shape()         const noexcept  { return shape_;          }
    constexpr stride_type stride()        const noexcept  { return {};              }
    constexpr auto        index(auto is)  const noexcept  { return is;              }

    constexpr void reshape( shape_type const& s ) noexcept { shape_ = s; }
    constexpr void swap( accessor& other )        noexcept { shape_.swap( other.shape_ ); }

    shape_type shape_;
  };

  //================================================================================================
  // Optimization : runtime 2D shape + unit stride
  // Expected sizeof : sizeof(void*) + sizeof(shape)
  //================================================================================================
  template<auto Shape, auto Stride>
  requires( !Shape.is_fully_static && Shape.order() == 2 && Stride.is_unit )
  struct  accessor<Shape, Stride>
  {
    using shape_type                    = std::remove_cvref_t<decltype(Shape)>;
    using stride_type                   = std::remove_cvref_t<decltype(Stride)>;
    static constexpr auto static_order  = shape_type::static_size;

    constexpr accessor() : shape_{} {}
    constexpr accessor(rbr::concepts::settings auto const& opts)
              : shape_ ( pick(kwk::size,opts) )
    {}

    constexpr std::ptrdiff_t  size()  const noexcept { return shape_.numel();  }
    constexpr auto            shape() const noexcept { return shape_;          }

    constexpr auto stride() const noexcept { return stride_type{fixed<1>,get<1>(shape_)}; }

    constexpr void reshape( shape_type const& s ) { shape_ = s; }

    constexpr auto index(auto i0)           const noexcept { return i0; }
    constexpr auto index(auto i0, auto i1)  const noexcept { return i1 + i0*get<1>(shape_); }

    constexpr void swap( accessor& other ) noexcept { shape_.swap( other.shape_ ); }

    shape_type  shape_;
  };
}
