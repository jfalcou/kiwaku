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
  struct empty_tuple { static constexpr bool is_homogeneous = false; };

  template<auto... D>
  struct prefilled_base
  {
    static constexpr  auto  descriptors   = kumi::tuple{D...};
    static constexpr  auto  setup         = map_descriptor<D...>();
    static constexpr  auto  dynamic_size  = setup.count;

    static constexpr auto make_storage()
    {
      if constexpr(dynamic_size == 0)
      {
        return empty_tuple {};
      }
      else
      {
        return [&]<std::size_t... N>(std::index_sequence<N...>)
        {
          using base = decltype(kumi::make_tuple(D...));
          return kumi::tuple<stored_t<kumi::element_t<setup.stored[N], base>>... >{};
        }(std::make_index_sequence<dynamic_size>{});
      }
    }

    using type = decltype(make_storage());
  };

  //====================================================================================================================
  // Prefilled tuple data-structure : mixed static/dynamic storage of value
  //====================================================================================================================
  template<auto... D>
  struct prefilled : prefilled_base<D...>::type
  {
    using is_product_type = void;

    static constexpr  auto  descriptors   = prefilled_base<D...>::descriptors;
    static constexpr  auto  setup         = prefilled_base<D...>::setup;
    static constexpr  auto  dynamic_size  = prefilled_base<D...>::dynamic_size;

    using storage_type = typename prefilled_base<D...>::type;

    static constexpr  auto  static_size       = sizeof...(D);
    static constexpr  bool  is_fully_static   = dynamic_size == 0;
    static constexpr  bool  is_dynamic        = !is_fully_static;
    static constexpr  bool  is_fully_dynamic  = dynamic_size == static_size;
    static constexpr  bool  is_homogeneous    = storage_type::is_homogeneous;

    // Do we have runtime storage for a given index ?
    static constexpr auto contains(int i) { return setup.index[i] != -1;  }

    // Default constructor
    KWK_TRIVIAL constexpr prefilled() noexcept : storage_type{} {}
    KWK_TRIVIAL constexpr prefilled(prefilled const&) =default;
    KWK_TRIVIAL constexpr prefilled& operator=(prefilled const&) =default;

    // Constructor from constant
    constexpr prefilled(std::integral auto def) noexcept
    {
      kumi::for_each_index([&](auto, auto& src) { src = def; }, *this);
    }

    // Constructor from extents
    constexpr prefilled(std::integral auto def, auto... vs) noexcept
    //requires(sizeof...(vs) == static_size)
    {
      if constexpr(!is_fully_static)
      {
        auto const input = kumi::tie(vs...);
        kumi::for_each_index( [&]<typename I>(I, auto& src)
                              {
                                constexpr bool is_inner = setup.stored[I::value] == (static_size-1);
                                auto const d = is_inner ? def : 1;

                                src = kwk::as_dimension(get<setup.stored[I::value]>(input), d);
                              }
                            , static_cast<storage_type&>(*this)
                            );
      }
    }

    // Constructor from another prefilled with same size
    template<auto... Os>
    constexpr prefilled(prefilled<Os...> const& other) noexcept
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

    // Static axis access
    template<std::size_t N>
    KWK_TRIVIAL constexpr auto axis() const noexcept
    {
      return get<N>(descriptors).base();
    }

    // Static tuple access impl
    template<std::size_t N>
    KWK_TRIVIAL constexpr decltype(auto)  __get() const & noexcept
    {
      if constexpr(contains(N)) return storage_type::operator[](kumi::index<setup.index[N]>);
      else                      return fixed<get<N>(descriptors).value>;
    }

    template<std::size_t N>
    KWK_TRIVIAL constexpr decltype(auto)  __get() const && noexcept
    {
      if constexpr(contains(N)) return storage_type::operator[](kumi::index<setup.index[N]>);
      else                      return fixed<get<N>(descriptors).value>;
    }

    template<std::size_t N>
    KWK_TRIVIAL constexpr decltype(auto)  __get() & noexcept
    {
      if constexpr(contains(N)) return storage_type::operator[](kumi::index<setup.index[N]>);
      else                      return fixed<get<N>(descriptors).value>;
    }

    template<std::size_t N>
    KWK_TRIVIAL constexpr decltype(auto)  __get() && noexcept
    {
      if constexpr(contains(N)) return storage_type::operator[](kumi::index<setup.index[N]>);
      else                      return fixed<get<N>(descriptors).value>;
    }

    // Static tuple access
    template<std::size_t N>
    KWK_TRIVIAL friend constexpr decltype(auto) get(prefilled const& s) noexcept
    requires(N>=0 && N<static_size)
    {
      return s.__get<N>();
    }

    template<std::size_t N>
    KWK_TRIVIAL friend constexpr decltype(auto) get(prefilled& s) noexcept
    requires(N>=0 && N<static_size)
    {
      return s.__get<N>();
    }

    template<std::size_t N>
    KWK_TRIVIAL friend constexpr decltype(auto) get(prefilled const&& s) noexcept
    requires(N>=0 && N<static_size)
    {
      return static_cast<prefilled const&&>(s).__get<N>();
    }

    template<std::size_t N>
    KWK_TRIVIAL friend constexpr decltype(auto) get(prefilled&& s) noexcept
    requires(N>=0 && N<static_size)
    {
      return static_cast<prefilled &&>(s).__get<N>();
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
      if constexpr(dynamic_size==1) return storage().impl.member0;
      else                          return storage().impl.members[idx];
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
      if constexpr(dynamic_size==1) return storage().impl.member0;
      else                          return storage().impl.members[idx];
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

    // Split the descriptors
    template<int N> static constexpr auto split() noexcept
    {
      return []<int... I>(std::integer_sequence<int,I...>)
      {
        return prefilled<get<(static_size - N) + I>(descriptors)...>{};
      }(std::make_integer_sequence<int,N>{});
    }

    // Conversion to std::array
    constexpr decltype(auto) as_array() const noexcept
    {
      return kumi::apply( []<typename... T>(T... m)
                          {
                            using type = typename largest_integral<T...>::type;
                            return std::array<type,static_size>{static_cast<type>(m)...};
                          }
                        , *this
                        );
    }

    // Swap prefilled_array's contents
    KWK_TRIVIAL void swap( prefilled& other )                    noexcept { std::swap( storage(), other.storage() ); }
    KWK_TRIVIAL friend void swap( prefilled& x, prefilled& y )  noexcept { x.swap(y); }

    // Equivalence checks for same order/axis kind
    template<auto... Os>
    constexpr bool is_equivalent(prefilled<Os...> const&) const noexcept
    {
      if constexpr(static_size == prefilled<Os...>::static_size)  return (D.is_equivalent(Os) && ... );
      else                                                        return false;
    }

    // Compatiblity checks  if they have the same size and don't contain conflicting static values
    template<auto... Os>
    constexpr bool is_compatible(prefilled<Os...> const& other) const noexcept
    {
      if constexpr(static_size != prefilled<Os...>::static_size)  return false;
      else
      {
        return  kumi::fold_left
                ( [](bool acc, auto const& t)
                  {
                    auto[e,f] = t;
                    return __checker(e, f, acc, acc);
                  }
                , kumi::zip(descriptors,other.descriptors)
                , true
                );
      }
    }

    template<auto... Os>
    constexpr bool is_similar(prefilled<Os...> const& other) const noexcept
    {
      if constexpr(static_size != prefilled<Os...>::static_size)  return false;
      else
      {
        return kumi::fold_left( [](bool acc, auto const& t)
                                {
                                  auto[e,f] = t;
                                  return __checker(e, f, false, acc);
                                }
                              , kumi::zip(descriptors,other.descriptors)
                              , true
                              );
      }
    }

    template<typename E, typename F>
    static KWK_FORCEINLINE constexpr bool __checker(E e, F f, auto def, auto acc) noexcept
    {
      if constexpr(E::is_dynamic)                         return acc;
      else if constexpr(!E::is_dynamic && F::is_dynamic)  return def;
      else                                                return acc && (e.value == f.value);
    }

    //==================================================================================================================
    // Internal compressed tuple storage
    //==================================================================================================================
    KWK_TRIVIAL constexpr storage_type &        storage() &       { return static_cast<storage_type&>(*this);         }
    KWK_TRIVIAL constexpr storage_type const&   storage() const&  { return static_cast<storage_type const&>(*this);   }
    KWK_TRIVIAL constexpr storage_type &&       storage() &&      { return static_cast<storage_type&&>(*this);        }
    KWK_TRIVIAL constexpr storage_type const&&  storage() const&& { return static_cast<storage_type const&&>(*this);  }
  };

  //====================================================================================================================
  // Prefilled type computation helper
  //====================================================================================================================
  template<auto... D>
  struct prefilled_t
  {
    static constexpr auto normalized = kwk::__::normalize_extents(D...);

    static constexpr auto make()
    {
      return []<std::size_t... I>(std::index_sequence<I...>)
      {
        return kwk::__::box< prefilled<get<I>(normalized)...> >{};
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
  KWK_PURE KWK_TRIVIAL constexpr decltype(auto) get(__::prefilled<Desc...> const& s) noexcept
  requires(N>=0 && N<__::prefilled<Desc...>::static_size)
  {
    return s.template __get<N>();
  }

  template<std::size_t N,auto... Desc>
  KWK_PURE KWK_TRIVIAL constexpr decltype(auto) get(__::prefilled<Desc...>& s) noexcept
  requires(N>=0 && N<__::prefilled<Desc...>::static_size)
  {
    return s.template __get<N>();
  }

  template<std::size_t N,auto... Desc>
  KWK_PURE KWK_TRIVIAL constexpr decltype(auto) get(__::prefilled<Desc...> const&& s) noexcept
  requires(N>=0 && N<__::prefilled<Desc...>::static_size)
  {
    return static_cast<__::prefilled<Desc...> const &&>(s).template __get<N>();
  }

  template<std::size_t N,auto... Desc>
  KWK_PURE KWK_TRIVIAL constexpr decltype(auto) get(__::prefilled<Desc...>&& s) noexcept
  requires(N>=0 && N<__::prefilled<Desc...>::static_size)
  {
    return static_cast<__::prefilled<Desc...>&&>(s).template __get<N>();
  }
}

//======================================================================================================================
// Tuple interface adaptation
//======================================================================================================================
template<auto... Desc>
struct  std::tuple_size<kwk::__::prefilled<Desc...>>
      : std::integral_constant<std::int32_t,sizeof...(Desc)>
{};

template<std::size_t N, auto... Desc>
struct  std::tuple_element<N, kwk::__::prefilled<Desc...>>
{
  using type = std::remove_cvref_t<decltype(std::declval<kwk::__::prefilled<Desc...>>().template __get<N>())>;
};
