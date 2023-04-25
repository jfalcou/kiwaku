//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kwk/detail/abi.hpp>
#include <kwk/detail/assert.hpp>
#include <kwk/detail/kumi.hpp>
#include <kwk/detail/raberu.hpp>
#include <kwk/detail/sequence/helpers.hpp>
#include <kwk/utility/traits/as_dimension.hpp>

namespace kwk::__
{
  //====================================================================================================================
  // Prefilled tuple data-structure : mixed static/dynamic storage of value
  //====================================================================================================================
  template<auto... D>
  struct prefilled2
  {
    using is_product_type = void;

    static constexpr  auto  descriptors   = kumi::tuple{D...};
    static constexpr  auto  setup         = map_descriptor<D...>();
    static constexpr  auto  dynamic_size  = setup.count;

    static constexpr auto make_storage()
    {
      return [&]<std::size_t... N>(std::index_sequence<N...>)
      {
        using base = decltype(kumi::make_tuple(D...));
        return kumi::tuple<stored_t<kumi::element_t<setup.stored[N], base>>... >{};
      }(std::make_index_sequence<dynamic_size>{});
    }

    using storage_type = decltype(make_storage());

    static constexpr  auto  static_size       = sizeof...(D);
    static constexpr  bool  is_fully_static   = dynamic_size == 0;
    static constexpr  bool  is_dynamic        = !is_fully_static;
    static constexpr  bool  is_homogeneous    = storage_type::is_homogeneous;

    // Do we have runtime storage for a given index ?
    static constexpr auto contains(int i) { return setup.index[i] != -1;  }

    // Default constructor
    KWK_TRIVIAL constexpr prefilled2() noexcept : storage_{} {}
    KWK_TRIVIAL constexpr prefilled2(prefilled2 const&) =default;

    // Constructor from extents
    constexpr prefilled2(std::integral auto def, auto... vs) noexcept
    requires(sizeof...(vs) == static_size)
    {
      auto const input = kumi::tie(vs...);
      kumi::for_each_index( [&](auto i, auto& src) { src= kwk::as_dimension(get<setup.stored[i]>(input), def); }
                          , storage_
                          );
    }

    // Constructor from another prefilled with same size
    template<auto... Os>
    constexpr prefilled2(prefilled2<Os...> const& other) noexcept
    requires(sizeof...(Os) == static_size)
    {
      kumi::for_each_index
      ( [&]<typename V>(auto i, V value)
        {
          if constexpr(contains(i)) get<i>(*this) = value;
          else                      KIWAKU_ASSERT (   value == get<i>(*this)
                                                  ,   "[KWK] - Runtime/Compile-time mismatch at index "
                                                  <<  i << " - Expected " << get<i>(*this)
                                                  << " but " << value << " was provided"
                                                  );
        }
      , other
      );
    }

    // Static tuple access impl
    template<std::size_t N>
    KWK_TRIVIAL constexpr auto __get() const noexcept
    {
      if constexpr(contains(N)) return get<setup.index[N]>(storage_);
      else                      return fixed<get<N>(descriptors).value>;
    }

    template<std::size_t N>
    KWK_TRIVIAL constexpr auto& __get() noexcept
    {
      if constexpr(contains(N)) return get<setup.index[N]>(storage_);
      else                      return fixed<get<N>(descriptors).value>;
    }

    // Static tuple access
    template<std::size_t N>
    KWK_TRIVIAL friend constexpr auto get(prefilled2 const& s) noexcept
    requires(N>=0 && N<static_size)
    {
      return s.__get<N>();
    }

    template<std::size_t N>
    KWK_TRIVIAL friend constexpr auto& get(prefilled2& s) noexcept
    requires(N>=0 && N<static_size)
    {
      return s.__get<N>();
    }

    // Dynamic access - We rely on KUMI internals for speed and ease of detection
    KWK_TRIVIAL constexpr auto operator[](std::convertible_to<std::size_t> auto i) const noexcept
    requires(is_dynamic && static_size>0)
    {
      static_assert(is_homogeneous, "[KWK] - Dynamic axis access requires homogeneous extent types.");
      auto const idx = static_cast<std::size_t>(i);

      KIWAKU_ASSERT ( idx<static_size
                    , "[KWK] - Out of bounds access at index: " << idx << " for a max. of " << static_size
                    );

      KIWAKU_ASSERT ( contains(idx)
                    ,   "[KWK] - Access at index "  << idx
                    <<  " overwrites a compile-time value of " << kumi::to_tuple(*this)
                    );

      // KUMI internal shortcut
      if constexpr(dynamic_size==1) return storage_.impl.member0;
      else                          return storage_.impl.members[idx];
    }

    KWK_TRIVIAL constexpr auto& operator[](std::convertible_to<std::size_t> auto i) noexcept
    requires(is_dynamic && static_size>0)
    {
      static_assert(is_homogeneous, "[KWK] - Dynamic axis access requires homogeneous extent types.");
      auto const idx = static_cast<std::size_t>(i);

      KIWAKU_ASSERT ( idx<static_size
                    , "[KWK] - Out of bounds access at index: " << idx << " for a max. of " << static_size
                    );

      KIWAKU_ASSERT ( contains(idx)
                    ,   "[KWK] - Access at index "  << idx
                    <<  " overwrites a compile-time value of " << kumi::to_tuple(*this)
                    );

      // KUMI internal shortcut
      if constexpr(dynamic_size==1) return storage_.impl.member0;
      else                          return storage_.impl.members[idx];
    }

    // Named access via Axis
    template<concepts::axis A>
    static constexpr auto __find_axis(A)
    {
      return kumi::locate(descriptors, []<typename M>(M) { return is_along_v<M,A>; });
    }

    template<concepts::axis A>
    KWK_TRIVIAL constexpr auto operator[](A) const noexcept
    requires(__find_axis(A{}) < static_size)
    {
      return __get<__find_axis(A{})>();
    }

    //==================================================================================================================
    // If an error occurs here, this means you tried to access a shape/stride value by an axis
    // descriptor which is not contained in said shape/stride.
    //  E.g:
    //    kwk::shape< width * height > x;
    //    x[depth] = 4;
    //==================================================================================================================
    template<concepts::axis A>
    requires(__find_axis(A{}) >= static_size)
    constexpr auto operator[](A) const noexcept = delete;

    // Total size of the tuple
    static constexpr std::int32_t size() noexcept { return static_size; }

    // Swap prefilled_array's contents
    KWK_TRIVIAL void swap( prefilled2& other )                    noexcept { std::swap( storage_, other.storage_ ); }
    KWK_TRIVIAL friend void swap( prefilled2& x, prefilled2& y )  noexcept { x.swap(y); }

    // Equivalence checks for same order/axis kind
    template<auto... Os>
    constexpr bool is_equivalent(prefilled2<Os...> const&) const noexcept
    {
      if constexpr(static_size == prefilled2<Os...>::static_size) return (D.is_equivalent(Os) && ... );
      else return false;
    }

    //==================================================================================================================
    // Internal compressed tuple storage
    //==================================================================================================================
    storage_type storage_;
  };

  //====================================================================================================================
  // Prefilled type computation helper
  //====================================================================================================================
  template<auto... D>
  struct prefilled2_t
  {
    static constexpr auto normalized = kwk::__::normalize_extents(D...);

    static constexpr auto make()
    {
      return []<std::size_t... I>(std::index_sequence<I...>)
      {
        return kwk::__::box< prefilled2<get<I>(normalized)...> >{};
      }(std::make_index_sequence<sizeof...(D)>{});
    }

    static constexpr auto duplicate_axis()
    {
      return []<std::size_t... I>(std::index_sequence<I...>)
      {
        return !kwk::__::all_unique<typename kumi::element_t<I,decltype(normalized)>::axis_kind...>;
      }(std::make_index_sequence<sizeof...(D)>{});
    }

    using type = typename decltype(make())::type;
  };
}

namespace kwk
{
  template<std::size_t N,auto... Desc>
  KWK_PURE KWK_TRIVIAL constexpr auto get(__::prefilled2<Desc...> const& s) noexcept
  requires(N>=0 && N<__::prefilled2<Desc...>::static_size)
  {
    return s.template __get<N>();
  }

  template<std::size_t N,auto... Desc>
  KWK_PURE KWK_TRIVIAL constexpr auto& get(__::prefilled2<Desc...>& s) noexcept
  requires(N>=0 && N<__::prefilled2<Desc...>::static_size)
  {
    return s.template __get<N>();
  }
}

//======================================================================================================================
// Tuple interface adaptation
//======================================================================================================================
template<auto... Desc>
struct  std::tuple_size<kwk::__::prefilled2<Desc...>>
      : std::integral_constant<std::int32_t,sizeof...(Desc)>
{};

template<std::size_t N, auto... Desc>
struct  std::tuple_element<N, kwk::__::prefilled2<Desc...>>
{
  using type = std::remove_cvref_t<decltype(get<N>(std::declval<kwk::__::prefilled2<Desc...>>()))>;
};
