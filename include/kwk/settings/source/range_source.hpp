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
  template<kwk::concepts::contiguous_range R> struct range_option : source_option<container_member_t<R>>
  {
    using source_type = std::remove_cvref_t<R>;
    using base = source_option<container_member_t<R>>;

    constexpr range_option() : base{nullptr}, size_(0) {};

    constexpr range_option(source_type& r) : base{std::data(r)}, size_(std::size(r)) {}

    constexpr range_option(source_type const& r) : base{std::data(r)}, size_(std::size(r)) {}

    constexpr range_option(source_type&& r) = delete;

    constexpr auto operator()(base::identifier_type) const { return *this; }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                         range_option const&) noexcept
    {
      return os << "Source: " << kumi::_::typer<source_type>();
    }

    constexpr auto size() const { return size_; }

    std::size_t size_;
  };

  template<typename T> constexpr auto storage(range_option<T> const& source)
  {
    return source.data();
  }

  template<typename T> constexpr auto shape_of(range_option<T> const& t)
  {
    // int due to shape taking int atm
    return kwk::shape{static_cast<int>(t.size())};
  }
}

namespace kwk::config
{
  template<kwk::concepts::contiguous_range R> struct preprocess_source<R>
  {
    static constexpr auto from(R&& r) noexcept { return kwk::__::range_option<R>{KWK_FWD(r)}; }
  };
}
