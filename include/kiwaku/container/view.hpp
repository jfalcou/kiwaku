//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_CONTAINER_VIEW_HPP_INCLUDED
#define KIWAKU_CONTAINER_VIEW_HPP_INCLUDED

#include <kiwaku/detail/container/view_span.hpp>
#include <kiwaku/detail/container/view_access.hpp>
#include <kiwaku/settings.hpp>
#include <kiwaku/stride.hpp>
#include <kiwaku/shape.hpp>

namespace kwk
{
  template<typename Type, auto... Settings>
  struct  view
        : detail::view_access < options::shape(settings(Settings...))
                              , options::stride(settings(Settings...))
                              >
        , detail::view_span<Type*>
  {
    //==============================================================================================
    // Constexpr properties extracted from settings
    //==============================================================================================
    static constexpr auto all_settings    = settings(Settings...);
    static constexpr auto shape_settings  = options::shape(all_settings);
    static constexpr auto stride_settings = options::stride(all_settings);

    static constexpr bool is_dynamic  = shape_settings.is_dynamic;

    using span_base       = detail::view_span<Type*>;
    using access_base     = detail::view_access<shape_settings,stride_settings>;
    using shape_type      = typename access_base::shape_type;
    using stride_type     = typename access_base::stride_type;

    //==============================================================================================
    // Sequence like dependent types
    //==============================================================================================
    using pointer         = typename span_base::pointer;
    using reference       = typename span_base::reference;
    using const_reference = typename span_base::const_reference;

    //==============================================================================================
    // Construct a view from a pointer
    //==============================================================================================
    view( pointer ptr ) noexcept requires( !is_dynamic )
        : access_base(), span_base(ptr)
    {}

    view( pointer ptr, shape_type const& shp ) noexcept requires( is_dynamic )
        : access_base(shp), span_base(ptr)
    {}

    //==============================================================================================
    // Construct a view from a C-style array - Requires constexpr shape
    //==============================================================================================
    template<std::size_t N>
    view(const Type (&s)[N]) noexcept requires (!is_dynamic) : view(&s[0])
    {}

    template<std::size_t N>
    view(const Type (&s)[N], shape_type const& shp) noexcept requires (is_dynamic)
        : view(&s[0], shp)
    {}

    //==============================================================================================
    // Construct a view from a ContiguousSequence
    //==============================================================================================
    template<typename Seq>
    view(Seq&& seq) noexcept
        requires  requires (Seq&& s) { std::forward<Seq>(s).data(); } &&  ( !is_dynamic )
        : view(std::forward<Seq>(seq).data())
     {}

    template<typename Seq>
    view(Seq&& seq, shape_type const& shp ) noexcept
        requires  requires (Seq&& s) { std::forward<Seq>(s).data(); } &&  ( is_dynamic )
        : view(std::forward<Seq>(seq).data(), shp)
     {}

    //==============================================================================================
    // Access value at a given position in the view
    //==============================================================================================
    template<typename... Int>
    const_reference operator()(Int... is) const noexcept requires(std::is_integral_v<Int> && ...)
    {
      return this->data()[ this->index(is...) ];
    }

    template<typename... Int>
    reference operator()(Int... is) noexcept requires(std::is_integral_v<Int> && ...)
    {
      return this->data()[ this->index(is...) ];
    }

    //==============================================================================================
    // Swap view with same settings
    //==============================================================================================
    void swap(view& other) noexcept
    {
      span_base::swap( static_cast<span_base&>(other) );
      access_base::swap( static_cast<access_base&>(other) );
    }

    //==============================================================================================
    // Reset the view to a new piece of data
    //==============================================================================================
    using span_base::reset;

    //==============================================================================================
    // Beginning of the data block
    //==============================================================================================
    auto begin()        { return this->data(); }
    auto begin() const  { return this->data(); }

    //==============================================================================================
    // End of the data block
    // For view with static shape, we need to check if begin() is not nullptr
    //==============================================================================================
    auto end()
    {
      if constexpr(is_dynamic)  return begin() + this->size();
      else                      return begin() == nullptr ? begin() : begin() + this->size();
    }

    auto end() const
    {
      if constexpr(is_dynamic)  return begin() + this->size();
      else                      return begin() == nullptr ? begin() : begin() + this->size();
    }

    //==============================================================================================
    // Access data about pointer, shape, stride etc...
    //==============================================================================================
    using span_base::data;
    using access_base::shape;
    using access_base::stride;
    using access_base::size;
    using access_base::count;
  };

  //================================================================================================
  // Deduction guides
  //================================================================================================
  template<typename T, typename S>  view(T*      , S const&) -> view<T,_nD<S::static_size>>;
  template<typename T, typename S>  view(T const*, S const&) -> view<T const,_nD<S::static_size>>;

  template<typename T, std::size_t N> view(const T (&)[N])  -> view<T, _1D(N)>;

  template<typename C, typename S>
  view(C&,S const&)       -> view<detail::value_type_of<C>, _nD<S::static_size>>;

  template<typename C, typename S>
  view(C const&,S const&) -> view<detail::value_type_of<C> const, _nD<S::static_size>>;
}

#endif
