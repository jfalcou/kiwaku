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
#include <kwk/utility/traits/extent.hpp>

namespace kwk::__
{
  struct empty_tuple
  {
    static constexpr bool is_homogeneous = true;
    constexpr auto operator==(empty_tuple const&) const noexcept { return true; }
  };

  // TODO: move to kumi
  template <int index, typename... T>
  using type_pack_element =
#if defined(__clang__) && __has_builtin(__type_pack_element)
    __type_pack_element<index, T...>;
#else
    kumi::element_t<index, kumi::tuple<T...>>;
#endif
  template <int index, auto... NTTP>
  using type_nttp_pack_element = type_pack_element<index, decltype(NTTP)...>;

  template <auto an_axis, bool> constexpr auto axis_as_unit_aux               { an_axis       };
  template <auto an_axis      > constexpr auto axis_as_unit_aux<an_axis, true>{ an_axis.value }; // indexed axes have no 'units'/are dimensionless/plain numbers

  template <auto an_axis> constexpr auto axis_as_unit{ axis_as_unit_aux<an_axis, an_axis.is_indexed> };

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
        return empty_tuple{};
      }
      else
      {
        return [&]<std::size_t... N>(std::index_sequence<N...>)
        {
          return kumi::tuple<stored_t<type_nttp_pack_element<setup.stored[N], D...>>...>{};
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
    static constexpr  bool  is_fully_implicit = ( D.is_implicit && ... && true);
    static constexpr  bool  is_fully_explicit = ( D.is_explicit && ... && true);
    static constexpr  bool  is_untyped        = ( D.is_indexed  && ... && true);
    static constexpr  bool  is_fully_typed    = (!D.is_indexed  && ... && true);

    // Do we have runtime storage for a given index ?
    static constexpr auto contains(int i) { return setup.index[i] != -1; }

    // Default constructor
    KWK_TRIVIAL constexpr prefilled() noexcept : storage_type{} {}
    KWK_TRIVIAL constexpr prefilled(prefilled const&) =default;
    KWK_TRIVIAL constexpr prefilled& operator=(prefilled const&) =default;

    constexpr void __construct(auto def, concepts::numeric_extent auto... vs) noexcept
    {
      if constexpr(is_fully_dynamic)
      {
        using kwk::extent;
        static_cast<storage_type&>(*this) = storage_type{extent(vs)...};
      }
      else
      {
        auto const input = kumi::tie(vs...);
        kumi::for_each_index( [&]<typename I>(I, auto v)
                              {
                                if constexpr(contains(I::value))
                                {
                                  __get<I::value>() = kwk::as_dimension(v, def);
                                }
                                else
                                {
                                  KIWAKU_ASSERT ( (v == _) || v == __get<I::value>()
                                                ,   "[KWK] - Runtime/Compile-time mismatch for axis "
                                                << kumi::get<I::value>(descriptors) << " as "
                                                << v << " was provided instead."
                                                );
                                }
                              }
                            , input
                            );
      }
    }

    constexpr void __construct(auto def, concepts::named_extent auto... vs) noexcept
    {
      if constexpr(!is_fully_static)
      {
        auto const input  = rbr::settings(vs...);
        auto const desc   = rbr::keywords<kumi::tuple>(input);

        kumi::for_each( [&]<typename X>(X x)
                        {
                          constexpr auto idx = __find_axis(X{});
                          if constexpr(contains(idx))
                          {
                            (*this)[x] = kwk::as_dimension(input[x.base()], def);
                          }
                          else
                          {
                            KIWAKU_ASSERT ( (input[x.base()] == _) || input[x.base()] == (*this)[x]
                                          ,   "[KWK] - Runtime/Compile-time mismatch for axis "
                                          <<  get<idx>(descriptors) << " as " << input[x.base()]
                                          << " was provided instead."
                                          );
                          }
                        }
                      , desc
                      );
      }
    }

    // Constructor from extents
    constexpr prefilled(std::integral auto def, concepts::numeric_extent auto... vs) noexcept
    requires(sizeof...(vs) == static_size)
    : storage_type{}
    {
      __construct(def,vs...);;
    }

    constexpr prefilled(std::integral auto def, concepts::numeric_extent auto... vs) noexcept
    requires(sizeof...(vs) < static_size)
    {
      [=, this]<int... I>(std::integer_sequence<int, I...>)
      {
        this->__construct(def,((void)I,1)..., vs...);
      }(std::make_integer_sequence<int, static_size - sizeof...(vs)>{});
    }

    constexpr prefilled(std::integral auto def, concepts::named_extent auto... vs) noexcept
    requires(sizeof...(vs) <= static_size)
    : storage_type{}
    {
      __construct(def, vs...);
    }

    // Are all axis in the same order if implicit axis exist?
    template<concepts::extent... V>
    static constexpr auto __is_ordered()
    {
      if constexpr(is_fully_explicit)                 return true;
      else if constexpr(static_size != sizeof...(V))  return false;
      else
      {
        auto ff = []<auto I, typename X>(constant<I>, X)
        {
          if constexpr(rbr::concepts::option<X>) return typename X::keyword_type{};
          else return implicit<static_size - 1 - I>;
        };

        return [&]<std::size_t... I>(std::index_sequence<I...>)
        {
          return (is_along_v<decltype(D), decltype(ff(fixed<I>,V{}))> && ... && true);

        }(std::make_index_sequence<sizeof...(V)>{});
      }
    }

    template<concepts::extent... V>
    constexpr prefilled(std::integral auto def, V... vs) noexcept
    requires( !is_fully_implicit && !is_fully_explicit && (sizeof...(vs) <= static_size) && __is_ordered<V...>())
    {
      [&]<std::size_t... I>(std::index_sequence<I...>)
      {
        __construct(def, normalize_axis(implicit<static_size - 1 - I>,vs)...);
      }(std::make_index_sequence<sizeof...(vs)>{});
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
    KWK_TRIVIAL constexpr auto& operator[](std::convertible_to<std::uint32_t> auto i) noexcept
    requires(is_fully_dynamic && static_size>0)
    {
      auto const idx = static_cast<std::uint32_t>(i);

      KIWAKU_ASSERT ( contains(idx)
                    ,   "[KWK] - Access at index "  << idx
                    <<  " overwrites a compile-time value of " << kumi::to_tuple(*this)
                    );

      return get_indexed(idx);
    }

    KWK_TRIVIAL constexpr auto operator[](std::convertible_to<std::uint32_t> auto i) const noexcept
    requires(is_fully_dynamic && static_size>0)
    {
      return const_cast<prefilled &>(*this).get_indexed(static_cast<std::uint32_t>(i));
    }

    KWK_TRIVIAL constexpr auto operator[](std::convertible_to<std::uint32_t> auto i) const noexcept
    requires(!(is_fully_dynamic && static_size>0))
    {
      return as_array()[i]; // rely on std::array bounds checking
    }

    // Named access via Axis
    template<concepts::axis A>
    static constexpr auto __find_axis(A)
    {
      return kumi::locate(descriptors, []<typename M>(M) { return is_along_v<M,A>; });
    }

    template<concepts::axis A>
    KWK_TRIVIAL constexpr decltype(auto) operator[](A const a) const noexcept
    requires(__find_axis(A{}) < static_size)
    {
      return a[__get<__find_axis(A{})>()]; // return a 'typed axis' wrapper of the raw value (at least for the non mutable case)
    }

    template<concepts::axis A>
    KWK_TRIVIAL constexpr decltype(auto) operator[](A) noexcept
    requires(__find_axis(A{}) < static_size)
    {
      return __get<__find_axis(A{})>();
    }

    //==================================================================================================================
    // If an error occurs here, this means you tried to access a shape/stride value by an axis
    // descriptor which is not contained in said shape/stride.
    //  E.g:
    //    kwk::shape< width, height > x;
    //    x[depth] = 4;
    //==================================================================================================================
    template<concepts::axis A>
    requires(__find_axis(A{}) >= static_size)
    constexpr auto operator[](A) const noexcept = delete;


    // Static axis access
    template<std::size_t N>
    using axis_kind = typename type_nttp_pack_element<N, D...>::axis_kind;

    template<std::size_t N>
    static constexpr auto axis_at{__::axis_as_unit<get<N>(descriptors)>};

    template<auto AxisID>
    static constexpr auto axis{axis_at<__find_axis(AxisID)>};

    template<std::size_t axis_index>
    static constexpr auto axis_with_extent(auto const value) noexcept
    {
        auto const the_axis{get<axis_index>(descriptors)};
        if constexpr(the_axis.is_indexed)
            return value;
        else
            return the_axis[value];
    }


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
      if constexpr(is_homogeneous && is_fully_dynamic)
      {
             if constexpr(dynamic_size==0) return std::array<joker::default_type, 0>{};
        else if constexpr(dynamic_size==1) return std::array{storage().impl.member0};
        else
          return reinterpret_cast
          <
            std::array
            <
              std::tuple_element_t<0, storage_type>,
              dynamic_size
            > const &
          >
          (storage().impl.members);
      }
      else
      {
        return kumi::apply( []<typename... T>(T... m)
                            {
                              using type = typename largest_integral<T...>::type;
                              return std::array<type,static_size>{static_cast<type>(m)...};
                            }
                          , *this
                          );
      }
    }

    // Equivalence checks for same order/axis kind
    template<auto... Os>
    constexpr bool is_equivalent(prefilled<Os...> const&) const noexcept
    {
      if constexpr(static_size == prefilled<Os...>::static_size)  return (D.is_equivalent(Os) && ... );
      else                                                        return false;
    }

    // Compatiblity checks if they have the same size and don't contain conflicting static values
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

  private:
    constexpr auto& get_indexed(std::uint32_t const idx) noexcept
    {
      static_assert(is_homogeneous, "[KWK] - Dynamic axis access requires homogeneous extent types.");

      KIWAKU_ASSERT ( idx<static_size
                    , "[KWK] - Out of bounds access at index: " << idx << " for a max. of " << static_size
                    );

      // KUMI internal shortcut
      if constexpr(dynamic_size==1) return storage().impl.member0;
      else                          return storage().impl.members[idx];
    }
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
        return kwk::__::box<prefilled<get<I>(normalized)...>>{};
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
