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
  ///@brief range_option provides a way to represent an arbitrary std::range like to extract its
  /// properties for
  ///       view/table  construction
  template<kwk::concepts::contiguous_range R>
  struct range_option : source_option<container_member_t<R>>
  {
    using type        = R;
    using source_type = std::remove_cvref_t<R>;
    using base        = source_option<container_member_t<R>>;
    using value_type  = std::remove_cvref_t<container_member_t<R>>;

    constexpr range_option()
        : base {nullptr}
        , size_(0)
    {
    }

    constexpr range_option(source_type& r)
        : base {kwk::data(r)}
        , size_(kwk::size(r))
    {
    }

    constexpr range_option(source_type const& r)
        : base {kwk::data(r)}
        , size_(kwk::size(r))
    {
    }

    constexpr range_option(source_type&& r)
        : base {kwk::data(r)}
        , size_(kwk::size(r))
    {
    }

    template<typename Self>
    constexpr auto&& operator()(this Self&& self, base::identifier_type const&)
    {
      using range_t = std::conditional_t<std::is_const_v<std::remove_reference_t<Self>>,
                                         range_option const,
                                         range_option>;
      return std::forward_like<Self>(static_cast<range_t&>(self));
    }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                         range_option const&) noexcept
    {
      return os << "Source: " << kumi::_::typer<source_type>();
    }

    constexpr auto size() const { return size_; }

    std::size_t    size_;
  };

  ///@brief Range option deduction guide
  template<kwk::concepts::contiguous_range R> range_option(R&& r) -> range_option<R>;

  ///@brief Helper to retrieve a pointer to the begining of a range_option
  template<typename T> constexpr auto source_pointer(range_option<T> const& source)
  {
    return source.data();
  }

  ///@brief Helper to retrieve the shape of a range_option
  template<typename T> constexpr auto shape_of(range_option<T> const& t)
  {
    return kwk::shape {t.size()};
  }
}

namespace kwk::config
{
  template<kwk::concepts::contiguous_range R> struct preprocess_source<R>
  {
    static constexpr auto from(R&& r) noexcept { return kwk::__::range_option {KWK_FWD(r)}; }
  };
}
