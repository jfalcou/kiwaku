//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kiwaku/detail/detail.hpp>
#include <kiwaku/memory/elements.hpp>

namespace kwk
{
  namespace __
  {
    // Identifier of the scheme option (part of the uniform "field over itself" option protocol).
    struct scheme_id : kumi::identifier<scheme_id>
    {
      friend constexpr auto to_str(scheme_id) { return kumi::str {"Scheme"}; }
    };

    //==================================================================================================================
    // scheme: organizes elements over a shared blob.
    //
    // A scheme is the "sheaf over a blob": it says *how* the blob's bytes are carved into typed
    // points. When `Kind` is not a product type every position is a homogeneous `Kind` fiber
    // (uniform mode). When `Kind` is a product type (a kumi record) each field becomes its own
    // fiber with its own byte offset in the blob, giving a structure-of-arrays storage (SoA mode).
    //
    // The scheme itself stores only the per-field `Offsets...`; the blob base pointer is supplied
    // by the caller (the blob / collection), so a scheme is a lightweight geometry and not tied to
    // a particular storage instance.
    //==================================================================================================================
    template<typename Kind, typename... Offsets> struct scheme_t : compressed_tuple<Offsets...>
    {
      //==============================================================================================================
      // Uniform construction backbone : every option follows the same "field over itself" protocol
      // (element_type / type / identifier_type / label_type / label() / operator()(identifier)),
      // so a scheme composes naturally in a properties pack alongside the other options.
      //==============================================================================================================
      using element_type    = scheme_t;
      using type            = scheme_t;
      using identifier_type = scheme_id;
      using label_type      = kumi::str;

      template<typename Self> constexpr auto&& operator()(this Self&& self, identifier_type const&)
      {
        using self_t = std::
            conditional_t<std::is_const_v<std::remove_reference_t<Self>>, scheme_t const, scheme_t>;
        return std::forward_like<Self>(static_cast<self_t&>(self));
      }

      //==================================================================================================================
      // A scheme is a property over itself : it exposes its own value through a `scheme()`
      // accessor.
      //==================================================================================================================
      template<typename Self>
      [[nodiscard]] KWK_ABI constexpr decltype(auto) scheme(this Self&& self) noexcept
      {
        using fwd_t = decltype(std::forward_like<Self>(std::declval<scheme_t>()));
        return static_cast<fwd_t>(self);
      }

      static constexpr label_type label() { return kumi::str {"Scheme"}; }

      //! The fiber type : the scalar (uniform) or the record of scalars (SoA).
      using kind_type                               = Kind;
      //! Placeholder for the per-field geometry (wired by the collection layer in Phase 3+).
      // using layout_type                             = void;

      //! The tuple of per-field byte offsets held by this scheme.
      using offsets_type                            = compressed_tuple<Offsets...>;

      //! Whether this space is a product space (each field is its own SoA fiber).
      static constexpr bool        is_product_space = kumi::concepts::product_type<Kind>;

      //! Number of fibers (1 for a uniform space, one per field for a product space).
      static constexpr std::size_t field_count      = kumi::function::size_or_v<Kind, 1>;

      //! Initializes the per-field offsets held by this scheme.
      template<typename... Offs>
        requires(sizeof...(Offs) == sizeof...(Offsets))
      constexpr scheme_t(Offs&&... offs) noexcept(
          std::is_nothrow_constructible_v<offsets_type, Offs&&...>)
          : offsets_type {KWK_FWD(offs)...}
      {
      }

      //================================================================================================================
      // data(base): returns the fiber pointer(s) given the blob base.
      //   - uniform : the single fiber pointer `base`.
      //   - product : a kumi::tuple of one byte-pointer per field (`base + o_0`, `base + o_1`,
      //   ...).
      //================================================================================================================
      template<typename Self> constexpr auto data(this Self&& self, auto* begin)
      {
        if constexpr(!is_product_space) return begin;
        else return kumi::map([ & ](auto e) { return e + begin; }, offsets_cast(self));
      }

      //================================================================================================================
      // operator[](base, index): resolve the `index`-th point of the space over the blob base.
      //   - uniform : a reference to `Kind` (a flat, strictly-positive array).
      //   - product : a kumi::tuple of per-field typed references (one per SoA fiber).
      //================================================================================================================
      template<typename Self>
      constexpr decltype(auto) operator[](this Self& self, auto* base, std::size_t index)
      {
        if constexpr(!is_product_space)
          return *resolve(element<Kind, 1> {index * sizeof(Kind)}, base);
        else
        {
          // Each field is its own SoA fiber: field `I` lives at `data(base)[I]` (a byte pointer
          // already offset to that field's region). Advance within the fiber by `index` scalars.
          return kumi::map_index(
              [ & ](auto I, auto&& e)
              {
                using curr = kumi::element_t<I, Kind>;
                return *resolve(element<curr, I + 1> {index * sizeof(curr)},
                                static_cast<std::byte*>(KWK_FWD(e)));
              },
              self.data(base));
        }
      }

    private:
      // The offsets, viewed through the base compressed_tuple (supporting kumi::get).
      static constexpr offsets_type& offsets_cast(auto& self)
      {
        return static_cast<offsets_type&>(self);
      }

    public:
      //================================================================================================================
      // The per-field offsets are the base compressed_tuple; retrieve them with
      // kumi::get<I>(scheme) via the base's ADL-friend, or read them through data()/operator[].
      //================================================================================================================
      template<typename CharT, typename Traits>
      friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                           scheme_t const&) noexcept
      {
        return os << "Scheme";
      }
    };

    //! Alias for the scheme type (natural name of the property).
    template<typename Kind, typename... Offsets> using scheme = scheme_t<Kind, Offsets...>;
  }
}

//======================================================================================================================
// Scheme record protocol: a `scheme` is a kumi record so the SoA per-field offsets are accessible
// through the product-type machinery (kumi::get<I>(scheme), kumi::size_v, ...). A `blob` is
// deliberately *not* a product type: it is a raw opaque byte region, so no tuple_size/tuple_element
// are provided for it.
//======================================================================================================================
namespace kumi
{
  template<typename Kind, typename... Offsets>
  struct is_record_type<kwk::__::scheme_t<Kind, Offsets...>> : std::true_type
  {
  };
}
