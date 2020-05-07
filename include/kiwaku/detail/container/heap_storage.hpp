//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_DETAIL_CONTAINER_HEAP_STORAGE_HPP_INCLUDED
#define KIWAKU_DETAIL_CONTAINER_HEAP_STORAGE_HPP_INCLUDED

#include <kiwaku/container/view.hpp>
#include <cstddef>

namespace kwk::detail
{
  template<typename Type, auto... Settings>
  struct heap_storage : private kwk::view<Type,Settings...>
  {
    using parent          = kwk::view<Type,Settings...>;
    using value_type      = typename parent::value_type;
    using reference       = typename parent::reference;
    using const_reference = typename parent::const_reference;
    // using shape_type      = decltype(Shape);
    // using stride_type     = decltype(Stride);
    using shape_type = typename parent::shape_type;

    static constexpr bool is_dynamic  = parent::is_dynamic;

    //==============================================================================================
    // Interface from the view
    //==============================================================================================
    using parent::operator();
    using parent::size;
    using parent::count;
    using parent::shape;
    using parent::stride;
    using parent::data;
    using parent::begin;
    using parent::end;

    heap_storage()          requires(!is_dynamic)  : parent{new Type[size()]} {}
    heap_storage() noexcept requires( is_dynamic)  : parent{nullptr, shape()} {}

    heap_storage(shape_type const& s) requires(is_dynamic) : parent{new Type[s.numel()], s} {}

    heap_storage( heap_storage const& that ) requires(is_dynamic) : parent{nullptr,that.shape()}
    {
      perform_copy(that);
    }

    heap_storage( heap_storage const& that ) requires(!is_dynamic) : parent{nullptr}
    {
      perform_copy(that);
    }

    ~heap_storage() { delete[] parent::data(); }

    // TODO : swap into view()
    void swap( heap_storage& that ) noexcept
    {
      auto ptr = that.data();
      auto old_ptr = parent::reset(ptr);
      that.reset(old_ptr);

      if constexpr(is_dynamic)
      {
        auto shp = shape();
        parent::reshape(that.shape());
        that.reshape(shp);
      }
    }

    heap_storage( heap_storage && that ) noexcept requires(is_dynamic) : heap_storage{}
    {
      swap(that);
    }

    heap_storage( heap_storage && that ) noexcept requires(!is_dynamic) : heap_storage{}
    {
      swap(that);
    }


    /*
    heap_storage& operator=( heap_storage const& other ) requires(!is_dynamic)
    {
      heap_storage that{other};
      swap(that);
      return *this;
    }

    heap_storage& operator=( heap_storage&& other ) requires(!is_dynamic)
    {
      heap_storage that{ std::move(other) };
      swap(that);
      return *this;
    }

    template<typename SomeShape>
    heap_storage(SomeShape const& s) : storage_(s.numel()), view_(storage_.data(),s)
    {}

    template<typename SomeShape> void resize(SomeShape const& s)
    {
      storage_.resize(s.numel());
      view_ = view{storage_.data(),shape_type(s)};
    }

    template<typename SomeShape> void reshape(SomeShape const& s)
    {
      assert( s.numel() == view_.size() && "[heap_storage::reshape] - Incompatible shape");
      view_ = view{storage_.data(),shape_type(s)};
    }
*/

    private:

    void perform_copy(heap_storage const& that)
    {
      std::unique_ptr<Type[]> mem(new Type[that.size()]);
      std::copy(that.begin(),that.end(),mem.get());

      parent::reset(mem.release());
    }
  };
}

#endif
