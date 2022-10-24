//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <concepts>
#include <kwk/detail/raberu.hpp>
#include <kwk/settings/size.hpp>
#include <kwk/settings/type.hpp>
#include <kwk/container/settings/category.hpp>

namespace kwk::concepts
{
  //================================================================================================
  //! @brief Basic container concept
  //!
  //! A @ref kwk::concepts::basic_container is a type exhibiting the basic interface of a
  //! nD container.
  //================================================================================================
  template<typename T>
  concept basic_container = requires(T const& t)
  {
    typename T::value_type;
    typename T::reference;
    typename T::const_reference;
    typename T::pointer;
    typename T::const_pointer;

    { t.order() };
    { t.numel() };
    { t.empty() };
    { t.shape() };
  };

  //================================================================================================
  // Check if two types are the same
  // If your compiler gives you an error here, you tried to pass a container of a given value type T
  // a value type of U was expected.
  //================================================================================================
  template<typename T, typename U>
  concept same_value_type = std::same_as<T,U>;

  //================================================================================================
  // Check if two shapes are similar
  // If your compiler gives you an error here, you tried to pass a container of a given Shape type
  // that can't be represented as the ExpectedShape
  //================================================================================================
  template<typename Shape, auto ExpectedShape>
  concept same_shape = ExpectedShape.is_similar(Shape{});

  //================================================================================================
  //! @brief Container concept
  //!
  //! A @ref kwk::concepts::container is a type exhibiting the basic interface of a
  //! nD container and having a given set of settings.
  //================================================================================================
  template<typename T, auto... Os>
  concept container =   basic_container<T>
                    &&  same_value_type < typename T::value_type
                                        , typename rbr::result::fetch_t < type
                                                                        | as<typename T::value_type>
                                                                        , decltype(Os)...
                                                                        >::type
                                  >
                    &&  same_shape< typename T::shape_type
                                  , rbr::fetch(size|(typename T::shape_type{}), Os...)
                                  >;

  //================================================================================================
  //! @brief Table concept
  //!
  //! A @ref kwk::concepts::table is a @ref kwk::concepts::container type with the semantic of a
  //! @ref kwk::table.
  //================================================================================================
  template<typename T, auto... Settings>
  concept table =  container<T,Settings...> && (kwk::table_ == T::container_kind);

  //================================================================================================
  //! @brief View concept
  //!
  //! A @ref kwk::concepts::view is a @ref kwk::concepts::container type with the semantic of a
  //! @ref kwk::view.
  //================================================================================================
  template<typename T, auto... Settings>
  concept view =  container<T,Settings...> && (kwk::view_ == T::container_kind);
}
