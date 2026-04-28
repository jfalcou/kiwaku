//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kwk/container/traits.hpp>

namespace kwk
{
  //====================================================================================================================
  // view: The main container-view class
  // Private inheritance of both shape and stride guarantees Empty Base Class Optimization
  //====================================================================================================================
  template<typename Source, view_options Opts>
  struct KWK_STRUCT_ABI view : private shape<Opts.shape_>, private stride<Opts.stride_>
  {
  private:
    struct build_tag
    {
    };

    template<typename Src, typename Shp, typename Str>
    constexpr view(build_tag, Src&& src, Shp&& shp, Str&& str)
      : shape_type(KWK_FWD(shp)), stride_type(KWK_FWD(str)), target_(source_pointer(KWK_FWD(src)))
    {
    }

  public:
    using shape_type = kwk::shape<Opts.shape_>;
    using stride_type = kwk::stride<Opts.stride_>;

    using value_type = container_base_t<Source>;

    using reference = std::add_lvalue_reference<value_type>;
    using const_reference = std::add_lvalue_reference<std::add_const_t<value_type>>;
    using pointer = std::add_pointer_t<value_type>;
    using const_pointer = std::add_pointer_t<value_type const>;

    static constexpr auto ndim = shape_type::ndim;
    static constexpr auto kind = as<value_type>();
    static constexpr auto itemsize = sizeof(value_type);

    template<int Flags, kumi::concepts::product_type Values>
    requires(Opts.valid_)
    constexpr view(options<Flags, Values> const& opts)
      : view(build_tag{},
             opts[kwk::source],
             __::view_traits<options<Flags, Values>>::get_shape(opts),
             __::view_traits<options<Flags, Values>>::get_stride(opts))
    {
    }

    template<kumi::concepts::field... Options>
    requires(Opts.valid_)
    constexpr view(Options const&... opts) : view(options{opts...})
    {
    }

    template<kumi::concepts::field... Options>
    requires(!Opts.valid_)
    constexpr view(Options const&...) = delete;

    constexpr shape_type const& shape() const noexcept { return static_cast<shape_type const&>(*this); }

    constexpr stride_type const& stride() const noexcept { return static_cast<stride_type const&>(*this); }

    constexpr auto size() const noexcept { return shape().size(); }

    constexpr pointer data(this auto&& self) { return KWK_FWD(self).target_; }

    //==================================================================================================================
    /*
      Access operators
    */
    //==================================================================================================================
    template<kumi::concepts::product_type T>
    decltype(auto) operator[](this auto&& self, T t) noexcept
    requires(kumi::size_v<T> == ndim)
    {
      return kumi::apply([&](auto... i) -> decltype(auto) { return std::forward_like<decltype(self)>(self)(i...); }, t);
    }

    template<std::integral... Is>
    decltype(auto) operator[](this auto&& self, Is... is) noexcept
    requires(sizeof...(Is) == ndim)
    {
      return std::forward_like<decltype(self)>(self.data()[linearize(self.stride(), is...)]);
    }

    template<concepts::slicer... S>
    auto operator[](this auto&& self, S const&... s) noexcept
    requires(sizeof...(S) == ndim)
    {
      return kwk::view{kwk::source = self.data() + kwk::origin(self.shape(), storage_order_t<Opts.order_>{}, s...),
                       kwk::reshape(self.shape(), s...), kwk::restride(self.stride(), s...)};
    }

  private:
    pointer target_;
  };

  //====================================================================================================================
  // Deduction Guides
  //====================================================================================================================
  template<kumi::concepts::field... Opts>
  view(Opts const&... opts) -> view<typename __::view_traits<__::make_bag_t<Opts...>>::source_type,
                                    __::view_traits<__::make_bag_t<Opts...>>::make_options()>;

  template<int Flags, kumi::concepts::product_type Values>
  view(options<Flags, Values> const& opts) -> view<typename __::view_traits<options<Flags, Values>>::source_type,
                                                   __::view_traits<options<Flags, Values>>::make_options()>;
}
