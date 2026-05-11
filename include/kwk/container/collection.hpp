//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

namespace kwk
{
  //====================================================================================================================
  // collection: The main container-view class
  // Private inheritance of both shape and stride guarantees Empty Base Class Optimization
  //====================================================================================================================
  template<typename Kind, typename... Opts> struct KWK_STRUCT_ABI collection : private Opts...
  {
  public:
    using option_type = decltype(options{std::declval<Opts>()...});
    using shape_type = decltype(std::declval<option_type>()[kwk::__::shape_id{}]);
    using stride_type = decltype(std::declval<option_type>()[kwk::__::stride_id{}]);
    using storage_order_type = decltype(std::declval<option_type>()[kwk::__::storage_order_id{}]);

    using source_type = Kind;
    using value_type = container_base_t<Kind>;
    using reference = std::add_lvalue_reference<value_type>;
    using const_reference = std::add_lvalue_reference<std::add_const_t<value_type>>;
    using pointer = std::add_pointer_t<value_type>;
    using const_pointer = std::add_pointer_t<value_type const>;

    static constexpr auto ndim = shape_type::ndim;
    static constexpr auto kind = as<value_type>();
    static constexpr auto itemsize = sizeof(value_type);

    //==================================================================================================================
    /*
      Constructors
    */
    //==================================================================================================================
  public:
    template<int Flags, kumi::concepts::product_type Values>
    // requires(  ) need to check that options and Opts are the same basically
    KWK_TRIVIAL constexpr collection(options<Flags, Values> const& opts)
      : Opts(opts[kumi::identifier_of<Opts>()])..., target_(source_pointer(opts[kwk::source]))
    {
    }

    template<kumi::concepts::field... Options>
    requires(sizeof...(Options) == (sizeof...(Opts) + 1))
    KWK_TRIVIAL constexpr collection(Options const&... opts) : collection(options{opts...})
    {
    }

    // template<kwk::concepts::collection<decltype(kwk::source = std::declval<source_type>()), Opts...> C>
    // KWK_TRIVIAL constexpr collection(C const& other)
    //   : collection(other.data(), other.shape(), other.stride())
    //{
    // }

    template<kumi::concepts::field... Options>
    requires(sizeof...(Options) != (sizeof...(Opts) + 1))
    constexpr collection(Options const&...) = delete;

    //==================================================================================================================
    /*
      Properties
    */
    //==================================================================================================================
    [[nodiscard]] KWK_TRIVIAL constexpr shape_type const& shape() const noexcept
    requires(!std::same_as<shape_type, kumi::unit>)
    {
      return static_cast<shape_type const&>(*this);
    }

    [[nodiscard]] KWK_TRIVIAL constexpr stride_type const& stride() const noexcept
    requires(!std::same_as<stride_type, kumi::unit>)
    {
      return static_cast<stride_type const&>(*this);
    }

    [[nodiscard]] KWK_TRIVIAL constexpr storage_order_type const& storage_order() const noexcept
    requires(!std::same_as<storage_order_type, kumi::unit>)
    {
      return static_cast<storage_order_type const&>(*this);
    }

    [[nodiscard]] KWK_TRIVIAL constexpr auto size() const noexcept
    requires(!std::same_as<shape_type, kumi::unit>)
    {
      return shape().size();
    }

    [[nodiscard]] KWK_TRIVIAL constexpr pointer data(this auto&& self) { return KWK_FWD(self).target_; }

    //==================================================================================================================
    /*
      Access operators
    */
    //==================================================================================================================
    template<kumi::concepts::product_type T>
    decltype(auto) operator[](this auto&& self, T&& t) noexcept
    requires(kumi::size_v<T> == ndim)
    {
      return kumi::apply(
        [&](auto&&... i) -> decltype(auto) { return std::forward_like<decltype(self)>(self)(KWK_FWD(i)...); },
        KWK_FWD(t));
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
      auto src = (kwk::source = self.data() + kwk::origin(self.shape(), self.storage_order(), s...));
      auto shp = kwk::reshape(self.shape(), s...);
      auto strd = kwk::restride(self.stride(), s...);

      return kwk::collection<Kind, decltype(shp), decltype(strd)>{src, shp, strd};
    }

  private:
    pointer target_;
  };

  template<typename Kind, kumi::concepts::field... Opts>
  requires(!kumi::concepts::uniquely_named<Opts...> || !kumi::concepts::fully_named<Opts...> ||
           !kumi::concepts::uniquely_labeled<Opts...>)
  struct collection<Kind, Opts...>
  {
    static_assert(kumi::concepts::uniquely_named<Opts...>, "[KWK] - Duplicate option in collection definition");
    static_assert(kumi::concepts::uniquely_labeled<Opts...>,
                  "[KWK] - Duplicate option representation in collection definition");
    static_assert(kumi::concepts::fully_named<Opts...>, "[KWK] - Anonymous option usage in collection definition");

    template<typename... Ts> collection(Ts&&...) = delete;
  };

  //====================================================================================================================
  // Deduction Guides
  //====================================================================================================================
  template<kumi::concepts::field Source, kumi::concepts::field... Opts>
  collection(Source const& s, Opts const&... opts) -> collection<typename std::remove_cvref_t<Source>::type, Opts...>;

  // template<int Flags, kumi::concepts::product_type Values>
  // collection(options<Flags, Values> const& opts) -> collection<>;
  // collection<typename __::view_traits<options<Flags, Values>>::source_type,
  //__::view_traits<options<Flags, Values>>::make_options()>;
}
