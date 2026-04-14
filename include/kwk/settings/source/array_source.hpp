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
  template<kwk::concepts::contiguous_static_range R> struct array_option : source_option<container_base_t<R>>
  {
    using source_type = std::remove_cvref_t<R>;
    using base = source_option<container_base_t<R>>;

    using value_type = std::remove_cvref_t<container_member_t<R>>;

    using reference = std::add_lvalue_reference<value_type>;
    using const_reference = std::add_lvalue_reference<std::add_const_t<value_type>>;
    using pointer = std::add_pointer_t<value_type>;
    using const_pointer = std::add_pointer_t<value_type const>;

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

  template<typename T> constexpr auto source_pointer(array_option<T> const& source)
  {
    return source.data();
  }

  template<typename T> constexpr auto shape_of(array_option<T> const&)
  {
    return kwk::shape<container_shape_v<T>>{};
  }
}

namespace kwk::config
{
  template<kwk::concepts::contiguous_static_range R> struct preprocess_source<R>
  {
    static constexpr auto from(R&& r) noexcept { return kwk::__::array_option<R>(KWK_FWD(r)); }
  };
}
