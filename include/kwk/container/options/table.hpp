//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#pragma once

#include <kwk/allocator/heap_allocator.hpp>
#include <kwk/container/options/forward.hpp>
#include <kwk/detail/stack_block.hpp>
#include <kwk/detail/heap_block.hpp>
#include <kwk/detail/raberu.hpp>
#include <kwk/options.hpp>

namespace kwk::tag { struct table_ {}; }

namespace kwk::options
{
  // The shape has retrieved from the options or error occurs
  template<rbr::concepts::settings Settings>
  constexpr auto shape(tag::table_ const&, Settings const& p) noexcept
  {
    return p[ kwk::size | of_size(0,1) ];
  }

  // For table, we infer the type from :
  template<rbr::concepts::settings Settings>
  struct element<tag::table_, Settings>
  {
    //  - type_ if present
    using has_type = rbr::result::fetch_t<type_{} | type<rbr::unknown_key>, Settings>;

    using type = typename has_type::type;

    static_assert ( !std::same_as<type, rbr::unknown_key>
                  , "[KWK] - Error: table must specify type"
                  );
  };

  // For table, we compute the data_block from the shape and allocator
  template<rbr::concepts::settings Settings>
  constexpr auto block(tag::table_ const& m, Settings const& p) noexcept
  {
    auto shp    = options::shape(m, p);
    using shp_t = decltype(shp);
    using type  = typename element<tag::table_, Settings>::type;

    if constexpr( shp_t::is_fully_static )
    {
      constexpr auto offset = options::offset(tag::table_{}, Settings{});
      return detail::stack_block<type, shp_t{}.numel(), offset>{};
    }
    else
    {
      return detail::heap_block<type> { p[ kwk::allocator | heap_allocator{} ]
                                      , shp.numel(), options::offset(tag::table_{}, p)
                                      };
    }
  }

}
