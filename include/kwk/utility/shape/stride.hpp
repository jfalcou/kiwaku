//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kwk/utility/shape/shape_descriptor.hpp>
#include <kwk/utility/shape/shape.hpp>
#include <kwk/utility/shape/storage_order.hpp>
#include <kwk/utility/fixed.hpp>
#include <kwk/settings/shape.hpp>
#include <array>

namespace kwk
{
  namespace __
  {
    struct stride_id;

    //==================================================================================================================
    // Compute stride descriptor from a shape descriptor and a custom layout permutation at compile time
    //==================================================================================================================
    consteval shape_descriptor make_stride_descriptor(shape_descriptor s_desc, storage_order_descriptor order)
    {
      shape_descriptor st_desc;
      st_desc.ndim = s_desc.ndim;

      if (s_desc.ndim > 0)
      {
        // Generate the permutation array dynamically based on the shape's ndim
        int perm[kwk::config::max_dimensions] = {};
        for (int i = 0; i < s_desc.ndim; ++i) { perm[i] = order.generator(i, s_desc.ndim); }

        // Fastest varying dimension (the last one in the permutation) has stride 1
        st_desc.dims[perm[s_desc.ndim - 1]] = 1;

        // Compute strides backwards according to permutation mapping
        for (int i = s_desc.ndim - 2; i >= 0; --i)
        {
          int curr_dim = perm[i];
          int prev_dim = perm[i + 1];

          if (st_desc.dims[prev_dim] == 0 || s_desc.dims[prev_dim] == 0)
            st_desc.dims[curr_dim] = 0; // Propagate dynamic dimensionality
          else st_desc.dims[curr_dim] = st_desc.dims[prev_dim] * s_desc.dims[prev_dim];
        }
      }

      st_desc.fully_dynamic = true;
      for (int i = 0; i < st_desc.ndim; ++i)
      {
        if (st_desc.dims[i] != 0) st_desc.fully_dynamic = false;
      }

      return st_desc;
    }
  }

  //====================================================================================================================
  /**
    @ingroup shape-utility
    @brief Stride of multi-dimensional container (Supports Arbitrary Layouts)
  **/
  //====================================================================================================================
  template<shape_descriptor Descriptor> struct stride : private __::as_sequence<Descriptor>::type
  {
    using element_type = stride;
    using type = stride;
    using identifier_type = __::stride_id;
    using label_type = kumi::str;
    using size_type = kwk::config::default_size_type;

    constexpr auto operator()(identifier_type const&) const { return *this; }

    static constexpr label_type label() { return kumi::str{"Stride"}; }

    static constexpr auto descriptor = Descriptor;
    static constexpr auto ndim = Descriptor.ndim;

    using storage_type = __::as_sequence<Descriptor>::type;

    constexpr stride() = default;

    template<std::convertible_to<size_type>... S>
    requires(sizeof...(S) == Descriptor.ndim && storage_type::template follow_mapping<kumi::tuple<S...>>())
    constexpr stride(S... s) : storage_type{s...}
    {
    }

    template<shape_descriptor D> friend constexpr bool operator==(stride const& a, stride<D> const& b) noexcept
    {
      if constexpr (Descriptor.ndim != D.ndim) return false;
      else return kumi::to_tuple(a) == kumi::to_tuple(b);
    }

    storage_type const& self() const { return static_cast<storage_type const&>(*this); }

    storage_type& self() { return static_cast<storage_type&>(*this); }

    // Arbitrary Layout Construct
    template<shape_descriptor ShapeDesc, storage_order_descriptor Order>
    requires(__::make_stride_descriptor(ShapeDesc, Order) == Descriptor)
    constexpr stride(shape<ShapeDesc> const& shp, storage_order_t<Order> order)
      : stride(shp, order, std::make_index_sequence<ShapeDesc.ndim>{})
    {
    }

    template<std::size_t I> KWK_TRIVIAL friend constexpr decltype(auto) get(stride const& s)
    {
      if constexpr (I >= stride::ndim) return kwk::fixed<0>;
      else return get<I>(s.self());
    }

    template<std::size_t I> KWK_TRIVIAL friend constexpr decltype(auto) get(stride& s)
    {
      if constexpr (I >= stride::ndim) return kwk::fixed<0>;
      else return get<I>(s.self());
    }

  private:
    template<std::size_t K, shape_descriptor ShapeDesc> static constexpr auto get_dim_val(shape<ShapeDesc> const& shp)
    {
      if constexpr (ShapeDesc.dims[K] != static_cast<int>(kwk::_)) return kwk::fixed<ShapeDesc.dims[K]>;
      else return get<K>(shp);
    }

    //==================================================================================================================
    // Helper for Arbitrary layout: stride[I] = product of dimensions following it in the permutation
    //==================================================================================================================
    template<std::size_t I, shape_descriptor ShapeDesc, storage_order_descriptor Order>
    static constexpr auto get_stride_val(shape<ShapeDesc> const& shp, storage_order_t<Order>)
    {
      constexpr int nbdim = ShapeDesc.ndim;

      constexpr auto perm = []() {
        std::array<int, nbdim> p{};
        for (int k = 0; k < nbdim; ++k) p[static_cast<std::size_t>(k)] = Order.generator(k, ndim);
        return p;
      }();

      constexpr int pos = [&]() {
        for (int k = 0; k < nbdim; ++k)
          if (static_cast<std::size_t>(perm[static_cast<std::size_t>(k)]) == I) return k;
        return ndim;
      }();

      if constexpr (pos >= nbdim - 1) { return kwk::fixed<1>; }
      else
      {
        return [&]<int... K>(std::integer_sequence<int, K...>) {
          return (kwk::fixed<1> * ... * get_dim_val<perm[pos + 1 + K]>(shp));
        }(std::make_integer_sequence<int, nbdim - 1 - pos>{});
      }
    }

    template<shape_descriptor ShapeDesc, storage_order_descriptor Order, std::size_t... I>
    constexpr stride(shape<ShapeDesc> const& shp, storage_order_t<Order> order, std::index_sequence<I...>)
      : storage_type{get_stride_val<I>(shp, order)...}
    {
    }
  };

  template<shape_descriptor ShapeDesc, storage_order_descriptor Order>
  stride(shape<ShapeDesc> const&, storage_order_t<Order>) -> stride<__::make_stride_descriptor(ShapeDesc, Order)>;

  template<std::convertible_to<kwk::config::default_size_type>... S>
  requires(sizeof...(S) > 0)
  stride(S...) -> stride<__::make_descriptor<S...>()>;

  //@brief utility to linearize a position based on the stride
  template<shape_descriptor SD, std::convertible_to<std::ptrdiff_t>... Is>
  constexpr auto linearize(stride<SD> const& s, Is... is) noexcept
  requires(sizeof...(Is) == stride<SD>::ndim)
  {
    return [&]<std::size_t... I>(std::index_sequence<I...>) {
      return (0 + ... + (get<I>(s) * is));
    }(std::make_index_sequence<stride<SD>::ndim>{});
  }
}

//======================================================================================================================
// Structured Binding Support
//======================================================================================================================
#if !defined(KWK_DOXYGEN_INVOKED)
template<kwk::shape_descriptor Descriptor>
struct std::tuple_size<kwk::stride<Descriptor>> : std::integral_constant<std::size_t, Descriptor.ndim>
{
};

template<std::size_t I, kwk::shape_descriptor Descriptor> struct std::tuple_element<I, kwk::stride<Descriptor>>
{
  using type = decltype(get<I>(std::declval<kwk::stride<Descriptor>>()));
};
#endif
