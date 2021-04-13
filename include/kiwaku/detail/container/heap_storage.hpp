//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kiwaku/allocator/block.hpp>
#include <kiwaku/container/view.hpp>
#include <cstddef>

namespace kwk::detail
{
  template<typename Type, typename Allocator, auto... Settings>
  struct heap_storage : private Allocator
                      , private kwk::view<Type,Settings...>
  {
    using parent            = kwk::view<Type,Settings...>;
    using value_type        = typename parent::value_type;
    using reference         = typename parent::reference;
    using const_reference   = typename parent::const_reference;
    using shape_type        = typename parent::shape_type;
    using stride_type       = typename parent::stride_type;
    using allocator_type    = Allocator;

    static constexpr bool is_dynamic              = parent::is_dynamic;
    static constexpr bool use_stateful_allocator  = allocator_type::is_stateful;

    //==============================================================================================
    // Interface from the view
    //==============================================================================================
    using parent::operator();
    using parent::size;
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
                  : Allocator{}, parent{nullptr, shape_type()}, capacity_{ 0 }
    {}

    heap_storage() requires(!is_dynamic)
                  : Allocator{}
                  , parent( [&]()
                            {
                              Allocator a;
                              auto b = a.allocate( sizeof(Type) * size() );
                              capacity_ = b.length;
                              return static_cast<Type*>(b.data);
                            }()
                          )
    {}

    //==============================================================================================
    // Copy constructor
    //  - set the view pointer to nullptr
    //  - if the storage substrate uses a static shape, we need to default construct it
    //  - then we perform a deep copy with strong exception guarantee
    // We perform a copy no matter what so we can't be noexcept.
    //==============================================================================================
    heap_storage( heap_storage const& that )
                : Allocator{}
                , parent{ [&]()
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
      block b{parent::data(), capacity_};
      Allocator::deallocate( b );
    }

    //==============================================================================================
    // Move constructor
    //  - Set the parent view into a null state.
    //  - Swap with the incoming rvalue
    //==============================================================================================
    heap_storage( heap_storage && that ) noexcept
                : Allocator{}
                , parent{ [&]()
                      {
                        if constexpr(is_dynamic)  return  parent{nullptr, shape_type()};
                        else                      return  parent{nullptr};
                      }()
                    }
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
                : Allocator{}
                , parent( [&]()
                          {
                            Allocator a;
                            auto b = a.allocate( sizeof(Type) * s.numel() );
                            capacity_ = b.length;
                            return static_cast<Type*>(b.data);
                          }()
                        , s
                        )
    {}

    //==============================================================================================
    // Swap - Asks parent to do the thing
    //==============================================================================================
    void swap( heap_storage& that ) noexcept
    {
      parent::swap( static_cast<parent&>(that) );
      Allocator::swap( that.allocator() );
      std::swap(capacity_,that.capacity_);
    }

    Allocator const&  allocator() const { return static_cast<Allocator const&>(*this);  }
    Allocator&        allocator()       { return static_cast<Allocator&>(*this);        }

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
      auto alloc  = that.allocator();
      auto b      = alloc.allocate( that.capacity_ );

      std::unique_ptr<Type[]> mem( static_cast<Type*>(b.data) );
      std::copy(that.begin(),that.end(),mem.get());
      parent::reset(mem.release());

      Allocator::swap(alloc);
      capacity_   = that.capacity_;
    }

    std::ptrdiff_t     capacity_;
  };
}
