//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once
#include <raberu/raberu.hpp>
#include <kwk/container/pick.hpp>
#include <type_traits>
#include <utility>

namespace kwk::__
{
  //================================================================================================
  // Order N general cases
  //================================================================================================
  template<auto Shape, auto Stride, bool useSpecificStrides> struct accessor
  {
    using shape_type                   = std::remove_cvref_t<decltype(Shape)>;
    using stride_type                  = std::remove_cvref_t<decltype(Stride)>;
    static constexpr auto static_order = shape_type::static_size;

    constexpr auto  size()    const noexcept  { return shape_.numel(); }
    constexpr auto  shape()   const noexcept  { return shape_;         }
    constexpr auto  stride()  const noexcept  { return stride_;        }

    KWK_TRIVIAL constexpr   accessor(): shape_ (), stride_() {}

    constexpr   accessor(rbr::concepts::settings auto const& opts)
              : shape_ ( pick(kwk::size   ,opts) )
              , stride_( pick(kwk::strides,opts) )
    {}

    constexpr auto index(auto... is)  const noexcept { return stride_.linearize(is...); }

    constexpr void swap( accessor& other ) noexcept
    {
      std::swap(shape_ , other.shape_  );
      std::swap(stride_, other.stride_ );
    }

    shape_type    shape_;
    stride_type   stride_;
  };

  //================================================================================================
  // Order N shape is fully static: no contents, only computation
  //================================================================================================
  template<auto Shape, auto Stride, bool useSpecificStrides>
  requires( Shape.is_fully_static && Stride.is_fully_static )
  struct accessor<Shape,Stride,useSpecificStrides>
  {
    using shape_type                    = std::remove_cvref_t<decltype(Shape)>;
    using stride_type                   = std::remove_cvref_t<decltype(Stride)>;
    static constexpr auto static_order  = shape_type::static_size;

    constexpr auto  size()    const noexcept  { return Shape.numel(); }
    constexpr auto  shape()   const noexcept  { return Shape;         }
    constexpr auto  stride()  const noexcept  { return Stride;        }

    KWK_TRIVIAL constexpr       accessor()  {}
    KWK_TRIVIAL constexpr       accessor(rbr::concepts::settings auto const&) noexcept {}
    KWK_TRIVIAL constexpr void  swap( accessor& ) noexcept {}

    constexpr auto index(std::integral auto... is) const noexcept { return Stride.linearize(is...); }
  };

  //================================================================================================
  // Optimization : runtime 1D shape + default unit stride
  //================================================================================================
  template<auto Shape, auto Stride>
  requires( !Shape.is_fully_static && Shape.order() == 1 )
  struct  accessor<Shape, Stride, false>
  {
    using shape_type                    = std::remove_cvref_t<decltype(Shape)>;
    using stride_type                   = std::remove_cvref_t<decltype(Stride)>;
    static constexpr auto static_order  = shape_type::static_size;

    KWK_TRIVIAL constexpr   accessor() : shape_{} {}
    KWK_TRIVIAL constexpr   accessor(rbr::concepts::settings auto const& opts)
                          : shape_ ( pick(kwk::size,opts) )
    {}

    constexpr auto        size()          const noexcept  { return get<0>(shape_);  }
    constexpr auto        shape()         const noexcept  { return shape_;          }
    constexpr stride_type stride()        const noexcept  { return {};              }
    constexpr auto        index(auto is)  const noexcept  { return is;              }

    constexpr void swap( accessor& other )        noexcept { shape_.swap( other.shape_ ); }

    shape_type shape_;
  };

  //================================================================================================
  // Optimization : runtime 1D shape + non-default stride
  //================================================================================================
  template<auto Shape, auto Stride>
  requires( !Shape.is_fully_static && Shape.order() == 1 )
  struct  accessor<Shape, Stride, true>
  {
    using shape_type                    = std::remove_cvref_t<decltype(Shape)>;
    using stride_type                   = std::remove_cvref_t<decltype(Stride)>;
    static constexpr auto static_order  = shape_type::static_size;

    constexpr   accessor() : shape_{} {}
    constexpr   accessor(rbr::concepts::settings auto const& opts)
              : shape_ ( pick(kwk::size,opts) )
              , stride_( pick(kwk::strides,opts) )
    {}

    constexpr auto        size()          const noexcept  { return get<0>(shape_);      }
    constexpr auto        shape()         const noexcept  { return shape_;              }
    constexpr stride_type stride()        const noexcept  { return stride_;             }
    constexpr auto        index(auto i0)  const noexcept  { return i0*get<0>(stride_);  }

    constexpr void swap( accessor& other ) noexcept
    {
      shape_.swap( other.shape_ );
      stride_.swap( other.stride_ );
    }

    shape_type  shape_;
    stride_type stride_;
  };

  //================================================================================================
  // Optimization : runtime 2D shape + default unit stride
  //================================================================================================
  template<auto Shape, auto Stride>
  requires( !Shape.is_fully_static && Shape.order() == 2 )
  struct  accessor<Shape, Stride, false>
  {
    using shape_type                    = std::remove_cvref_t<decltype(Shape)>;
    using stride_type                   = std::remove_cvref_t<decltype(Stride)>;
    static constexpr auto static_order  = shape_type::static_size;

    KWK_TRIVIAL constexpr accessor() : shape_{} {}
    KWK_TRIVIAL constexpr accessor(rbr::concepts::settings auto const& opts)
                        : shape_ ( pick(kwk::size,opts) )
    {}

    constexpr auto  size()    const noexcept { return shape_.numel();  }
    constexpr auto  shape()   const noexcept { return shape_;          }
    constexpr auto  stride()  const noexcept { return stride_type{get<1>(shape_),fixed<1>}; }

    constexpr auto  index(auto i0)           const noexcept { return i0; }
    constexpr auto  index(auto i1, auto i0)  const noexcept { return i0 + i1*get<1>(shape_); }

    constexpr void  swap( accessor& other ) noexcept { std::swap(shape_ , other.shape_  ); }

    shape_type  shape_;
  };

  //================================================================================================
  // Optimization : runtime 2D shape + specific stride
  //================================================================================================
  template<auto Shape, auto Stride>
  requires( !Shape.is_fully_static && Shape.order() == 2 )
  struct  accessor<Shape, Stride, true>
  {
    using shape_type                    = std::remove_cvref_t<decltype(Shape)>;
    using stride_type                   = std::remove_cvref_t<decltype(Stride)>;
    static constexpr auto static_order  = shape_type::static_size;

    constexpr accessor() : shape_{} {}
    constexpr accessor(rbr::concepts::settings auto const& opts)
              : shape_ ( pick(kwk::size,opts) )
              , stride_( pick(kwk::strides,opts) )
    {}

    constexpr auto  size()    const noexcept { return shape_.numel(); }
    constexpr auto  shape()   const noexcept { return shape_;         }
    constexpr auto  stride()  const noexcept { return stride_;        }

    constexpr auto index(auto i0)           const noexcept  { return i0; }
    constexpr auto index(auto i1, auto i0)  const noexcept  { return stride_.linearize(i1,i0); }

    constexpr void swap( accessor& other ) noexcept
    {
      shape_.swap( other.shape_ );
      stride_.swap( other.stride_ );
    }

    shape_type  shape_;
    stride_type stride_;
  };
}
