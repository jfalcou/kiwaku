//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

namespace kwk::__
{
  ///@brief array_option provides a way to represent a contiguous_static_range to extract it's properties for view/table
  /// construction
  template<kwk::concepts::contiguous_static_range R> struct array_option : source_option<container_base_t<R>>
  {
    using type = R;
    using source_type = std::remove_cvref_t<R>;
    using base = source_option<container_base_t<R>>;
    using value_type = std::remove_cvref_t<container_member_t<R>>;

    constexpr array_option() : base{nullptr} {};
    constexpr array_option(source_type& r) : base{container_base_address(r)} {};
    constexpr array_option(source_type const& r) : base{container_base_address(r)} {};
    constexpr array_option(source_type&& r) : base{container_base_address(r)} {};

    static constexpr auto size = kumi::container_size_v<R>;

    constexpr auto operator()(base::identifier_type) const { return *this; }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                         array_option const&) noexcept
    {
      return os << "Source: " << kumi::_::typer<source_type>();
    }
  };

  ///@brief Array option deduction guide
  template<kwk::concepts::contiguous_static_range R> array_option(R&& r) -> array_option<R>;

  ///@brief Helper to retrieve the pointer to the begining of an array_option
  template<typename T> constexpr auto source_pointer(array_option<T> const& source)
  {
    return source.data();
  }

  ///@brief Helper to retrieve a the shape of an array_option
  template<typename T> constexpr auto shape_of(array_option<T> const&)
  {
    return kwk::shape<container_shape_v<T>>{};
  }
}

namespace kwk::config
{
  template<kwk::concepts::contiguous_static_range R> struct preprocess_source<R>
  {
    static constexpr auto from(R&& r) noexcept { return kwk::__::array_option{KWK_FWD(r)}; }
  };
}
