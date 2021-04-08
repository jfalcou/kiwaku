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

#include <kiwaku/allocator/allocator.hpp>
#include <kiwaku/allocator/heap.hpp>
#include <kiwaku/container/view.hpp>
#include <cstddef>
#include <memory>

namespace kwk::detail
{
  template<typename Type, auto... Settings>
  struct heap_storage : private kwk::view<Type,Settings...>
  {
    using parent            = kwk::view<Type,Settings...>;
    using value_type        = typename parent::value_type;
    using reference         = typename parent::reference;
    using const_reference   = typename parent::const_reference;
    using shape_type        = typename parent::shape_type;
    using stride_type       = typename parent::stride_type;

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

    //==============================================================================================
    // Default constructor
    //  - set the view pointer to nullptr
    //  - if the storage substrate uses a dynamic shape, we need to default construct it
    //  - otherwise, we can directly allocate new data as we know the numel() of the shape
    // In the case of a dynamic shape, we know we are noexcept as no allocation takes place.
    //==============================================================================================
    heap_storage()  noexcept requires(is_dynamic)
                  : parent{nullptr, shape_type()}
                  , allocator_(heap_allocator{})
                  , capacity_{ bytes{0} }
    {
    }

    heap_storage() requires(!is_dynamic)
                  : parent( [&]()
                            {
                              heap_allocator a;
                              auto b = a.allocate( as_bytes<Type>(elements{count()}) );
                              capacity_ = b.length;
                              return static_cast<Type*>(b.data);
                            }()
                          )
                  , allocator_(heap_allocator{})
    {
    }

    //==============================================================================================
    // Copy constructor
    //  - set the view pointer to nullptr
    //  - if the storage substrate uses a static shape, we need to default construct it
    //  - then we perform a deep copy with strong exception guarantee
    // We perform a copy no matter what so we can't be noexcept.
    //==============================================================================================
    heap_storage( heap_storage const& that )
                : parent{ [&]()
                          {
                            if constexpr(is_dynamic)  return  parent{nullptr,that.shape()};
                            else                      return  parent{nullptr};
                          }()
                        }
    {
      perform_copy(that);
    }

    //==============================================================================================
    // Destructor gonna destruct
    //==============================================================================================
    ~heap_storage()
    {
      if(parent::data())
      {
        block b{parent::data(), capacity_};
        allocator_.deallocate( b );
      }
    }

    //==============================================================================================
    // Move constructor
    //  - Set the parent view into a null state.
    //  - Swap with the incoming rvalue
    //==============================================================================================
    heap_storage( heap_storage && that ) noexcept
                : parent{ [&]()
                      {
                        if constexpr(is_dynamic)  return  parent{nullptr, shape_type()};
                        else                      return  parent{nullptr};
                      }()
                    }
                , allocator_{}
    {
      swap(that);
    }

    //==============================================================================================
    // Move assignment operator - Use construct+swap
    //==============================================================================================
    heap_storage& operator=( heap_storage const& other )
    {
      heap_storage that{other};
      swap(that);
      return *this;
    }

    //==============================================================================================
    // Move assignment operator - Use construct+swap
    //==============================================================================================
    heap_storage& operator=( heap_storage&& other )
    {
      heap_storage that{ std::move(other) };
      swap(that);
      return *this;
    }

    //==============================================================================================
    // Construct from a shape
    //==============================================================================================
    heap_storage(shape_type const& s) requires(is_dynamic)
                : parent( [&]()
                          {
                            heap_allocator a;
                            auto b = a.allocate( as_bytes<Type>(elements{s.numel()}) );
                            capacity_ = b.length;
                            return static_cast<Type*>(b.data);
                          }()
                        , s
                        )
                  , allocator_(heap_allocator{})
    {}

    //==============================================================================================
    // Swap - Asks parent to do the thing
    //==============================================================================================
    void swap( heap_storage& that ) noexcept
    {
      parent::swap( static_cast<parent&>(that) );
      allocator_.swap( that.allocator_ );
      std::swap(capacity_,that.capacity_);
    }

/*
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
      auto alloc  = that.allocator_;
      auto b      = alloc.allocate( as_bytes<Type>(elements{that.capacity_}) );

      std::unique_ptr<Type[]> mem( static_cast<Type*>(b.data) );
      std::copy(that.begin(),that.end(),mem.get());
      parent::reset(mem.release());

      allocator_.swap(alloc);
      capacity_   = that.capacity_;
    }

    allocator allocator_;
    bytes     capacity_;
  };
}

#endif
