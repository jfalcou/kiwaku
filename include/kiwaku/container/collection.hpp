//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kiwaku/fwd.hpp>
#include <kiwaku/memory/blob.hpp>
#include <kiwaku/utility/layout.hpp>
#include <kiwaku/utility/accessor.hpp>
#include <kiwaku/settings/kind.hpp>
#include <kiwaku/settings/options.hpp>
#include <kiwaku/container/properties.hpp>
#include <array>
#include <tuple>
#include <utility>

namespace kwk
{
  // `collection` is forward-declared in `kiwaku/fwd.hpp` (the single place that declares it).
  //
  // `collection` is a *variadic* template over its component types. Each component is a concrete
  // property type (storage, layout, scheme, accessor, kind) and `collection` holds them through a
  // single `properties` intermediate that simply (publicly) inherits the pack. Because a collection
  // *has* its properties, the `properties` base is inherited privately; the components are still
  // reached through `operator->` (`collection->shape()`, `collection->kind()`, ...) while
  // `operator[]` is re-exposed directly. The `layout` component couples the shape and stride; the
  // option-ordered pack is exactly the argument order of the collection constructor, so CTAD
  // deduces it directly from `kwk::collection{ storage, layout, scheme, accessor, kind }`.

  //====================================================================================================================
  /*
    collection : the concrete container-view object.

    `collection` is a *variadic* template over its component types. It publicly inherits a single
    `kwk::__::properties` object that itself publicly inherits each component type (storage, layout,
    scheme, accessor, kind), so the whole pack is reachable through the base classes. `operator->`
    returns the properties base, so `collection->shape()`, `collection->kind()`,
    `collection->get<K>()` all resolve there directly — no per-property forwarding in the shell.
    The component pack is order-independent (each option is located in `properties` by what it is),
    but the builder always assembles it in the canonical construction order.

    The pack is exactly the constructor's argument order: `kwk::collection< Storage, Layout, Scheme,
    Accessor, Kind >`. The builder (`builder_t::operator()`) and every internal sub-view (per-field
    `get<I>`, slice/tile `make_view`) construct it as `kwk::collection{ storage, layout, scheme,
    accessor, kind }`, and CTAD deduces the variadic pack from those five component arguments, the
    layout coupling the shape and stride. The component assembly is thus fixed in one place
    (`make_collection`) and identical for every collection type.

    collection is the concrete output of the builder; table (owning) and view (borrowed) factories
    both produce a collection through the builder's CTAD.
  */
  //====================================================================================================================
  template<typename... Opts> struct collection : private __::properties<Opts...>
  {
    using impl_type = __::properties<Opts...>;

    // Re-expose the object interface that `properties` provides. Under private inheritance a
    // collection *holds* its properties (it is not one), so only the members that are meant to be
    // called directly on the collection object are brought in publicly; the component interface is
    // reached through `operator->` below.
    using impl_type::operator[];

    // Size type of the space, inherited from the layout.
    using size_type                 = typename impl_type::size_type;

    // The scalar content type of a single element of this space.
    using value_type                = typename impl_type::value_type;

    // Required by the container concepts.
    using source_type               = void;

    // The number of dimensions of this space (a compile-time constant derived from the layout).
    static constexpr size_type ndim = impl_type::ndim;

    //==================================================================================================================
    /*
      Construction

      `collection` is a plain value type constructed directly as `kwk::collection{ storage, layout,
      scheme, accessor, kind }`. Both the builder (`builder_t::operator()`) and every internal
      sub-view (per-field `get<I>`, slice/tile `make_view`) construct it through the single
      `make_collection` authority, so the component assembly is fixed in one place and identical for
      every collection type.

      The constructor is a forwarding template : it accepts the five component arguments in any
      value category and forwards them to the `properties` base. CTAD is driven by the explicit
      deduction guide below, which decays each deduced component so an lvalue `layout` argument does
      not inject reference types into the pack : the guide maps the five arguments onto
      `kwk::collection<Storage, Layout, Scheme, Accessor, Kind>` and is preferred over the
      constructor's implicit guide.
    */
    //==================================================================================================================
    template<typename... Args>
    constexpr collection(Args&&... args)
        : impl_type(KWK_FWD(args)...)
    {
    }

    //==================================================================================================================
    /*
      Options interface : operator-> returns a pointer to the properties base so that
      collection->shape(), collection->get<K>(), collection->kind() all resolve there. The
      properties base publicly inherits every component type, so those members are reachable through
      the arrow without any forwarding declarations in this shell.
    */
    //==================================================================================================================
    constexpr impl_type*       operator->() noexcept { return static_cast<impl_type*>(this); }
    constexpr impl_type const* operator->() const noexcept
    {
      return static_cast<impl_type const*>(this);
    }

    // `operator[]` is re-exposed above via the using-declaration, so `collection[i]`,
    // `collection[...]` and `collection[sl...]` resolve directly on the object; every component
    // member (`shape`, `kind`, `get<I>`, ...) is reached through `operator->`.
  };

  //====================================================================================================================
  /*
    Deduction guide : CTAD for a variadic class template needs the pack mapped from the five
    component arguments. Each deduced component is decayed (`remove_cvref_t`) so an lvalue `layout`
    argument does not inject reference types into the pack; the result is
    `kwk::collection<Storage, Layout, Scheme, Accessor, Kind>`. The explicit guide is preferred over
    the constructor's implicit `collection(Opts&&...) -> collection<Opts...>` guide (which would
    keep reference types for such lvalue arguments).
  */
  //====================================================================================================================
  template<typename Storage,
           typename Layout,
           typename Scheme,
           typename Accessor,
           typename Kind>
  collection(Storage&&, Layout&&, Scheme&&, Accessor&&, Kind&&)
      -> collection<std::remove_cvref_t<Storage>,
                    std::remove_cvref_t<Layout>,
                    std::remove_cvref_t<Scheme>,
                    std::remove_cvref_t<Accessor>,
                    std::remove_cvref_t<Kind>>;

  //======================================================================================================================
  /* Per-field (SoA) access : `kwk::get<I>(c)` returns a borrowed scalar view over field I's fiber.
   */
  //======================================================================================================================
  template<std::size_t I, typename... Opts>
    requires(kumi::concepts::product_type<typename kwk::collection<Opts...>::value_type>)
  [[nodiscard]] constexpr auto get(kwk::collection<Opts...> const& c)
  {
    return c->template get<I>();
  }
}

//======================================================================================================================
/* Tuple protocol : collection reflects the kind when it is a product type. */
//======================================================================================================================
namespace std
{
  template<std::size_t I, typename... Opts>
    requires(kumi::concepts::product_type<typename kwk::collection<Opts...>::value_type> &&
             I < kumi::size_v<typename kwk::collection<Opts...>::value_type>)
  struct tuple_element<I, kwk::collection<Opts...>>
      : std::tuple_element<I, typename kwk::collection<Opts...>::value_type>
  {
  };

  template<typename... Opts>
    requires(kumi::concepts::product_type<typename kwk::collection<Opts...>::value_type>)
  struct tuple_size<kwk::collection<Opts...>>
      : std::tuple_size<typename kwk::collection<Opts...>::value_type>
  {
  };
}
