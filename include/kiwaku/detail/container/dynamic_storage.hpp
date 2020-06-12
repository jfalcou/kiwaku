//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_DETAIL_CONTAINER_DYNAMIC_STORAGE_HPP_INCLUDED
#define KIWAKU_DETAIL_CONTAINER_DYNAMIC_STORAGE_HPP_INCLUDED

#include <kiwaku/container/view.hpp>
#include <cstddef>

namespace kwk::detail
{
  template<typename Type, auto... Settings>
  struct dynamic_storage : private kwk::view<Type,Settings...>
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

    //==============================================================================================
    // Default constructor
    //  - set the view pointer to nullptr
    //  - if the storage substrate uses a dynamic shape, we need to default construct it
    //  - otherwise, we can directly allocate new data as we know the numel() of the shape
    //    TODO: Replace new[] with a proper allocator interface call
    // In the case of a dynamic shape, we know we are noexcept as no allocation takes place.
    //==============================================================================================
    dynamic_storage()  noexcept( is_dynamic )
                  : parent{ [&]()
                            {
                              if constexpr(is_dynamic)  return  parent{nullptr, shape_type()};
                              else                      return  parent{new Type[size()]};
                            }()
                          }
    {}

    //==============================================================================================
    // Copy constructor
    //  - set the view pointer to nullptr
    //  - if the storage substrate uses a static shape, we need to default construct it
    //  - then we perform a deep copy with strong exception guarantee
    // We perform a copy no matter what so we can't be noexcept.
    //==============================================================================================
    dynamic_storage( dynamic_storage const& that )
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
    // TODO: Replace delete[] with a proper allocator interface call
    //==============================================================================================
    ~dynamic_storage() { delete[] parent::data(); }

    //==============================================================================================
    // Move constructor
    //  - Set the parent view into a null state.
    //  - Swap with the incoming rvalue
    //==============================================================================================
    dynamic_storage( dynamic_storage && that ) noexcept
                : parent{ [&]()
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
    dynamic_storage& operator=( dynamic_storage const& other )
    {
      dynamic_storage that{other};
      swap(that);
      return *this;
    }

    //==============================================================================================
    // Move assignment operator - Use construct+swap
    //==============================================================================================
    dynamic_storage& operator=( dynamic_storage&& other )
    {
      dynamic_storage that{ std::move(other) };
      swap(that);
      return *this;
    }

    //==============================================================================================
    // Construct from a shape
    // TODO: Constructs from settings(...)
    //==============================================================================================
    dynamic_storage(shape_type const& s) requires(is_dynamic) : parent{new Type[s.numel()], s} {}

    //==============================================================================================
    // Swap - Asks parent to do the thing
    //==============================================================================================
    void swap( dynamic_storage& that ) noexcept { parent::swap( static_cast<parent&>(that) ); }

/*
    template<typename SomeShape> void resize(SomeShape const& s)
    {
      storage_.resize(s.numel());
      view_ = view{storage_.data(),shape_type(s)};
    }

    template<typename SomeShape> void reshape(SomeShape const& s)
    {
      assert( s.numel() == view_.size() && "[dynamic_storage::reshape] - Incompatible shape");
      view_ = view{storage_.data(),shape_type(s)};
    }
*/

    private:

    void perform_copy(dynamic_storage const& that)
    {
      std::unique_ptr<Type[]> mem(new Type[that.size()]);
      std::copy(that.begin(),that.end(),mem.get());

      parent::reset(mem.release());
    }
  };
}

#endif
