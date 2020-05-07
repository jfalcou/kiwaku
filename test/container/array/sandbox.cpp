//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include "test.hpp"
#include <kiwaku/detail/container/storage_selector.hpp>
#include <kiwaku/algorithm/all.hpp>
#include <tts/tests/relation.hpp>
#include <tts/tests/basic.hpp>
#include <vector>

namespace kwk
{
  template<typename Type, auto... Settings>
  struct array : private detail::storage_selector<Type,Settings...>::type
  {
    using storage   = detail::storage_selector<Type,Settings...>;
    using parent    = typename storage::type;
    static constexpr bool is_dynamic  = parent::is_dynamic;
    static constexpr bool use_heap    = storage::use_heap;

    using shape_type = typename parent::shape_type;

    array() : parent{} {}
    array(shape_type const& s) noexcept requires(use_heap) : parent{s} {}

    using parent::swap;
/*
    array( array const& that ) requires(!is_dynamic) : view_type{nullptr}
    {
      std::unique_ptr<Type[]> mem(new Type[that.size()]);
      std::copy(that.begin(),that.end(),mem.get());

      view_type::reset(mem.release());
    }

    array( array && that ) noexcept requires(!is_dynamic) : view_type{nullptr}
    {
      swap(that);
    }

    array& operator=( array const& other ) requires(!is_dynamic)
    {
      array that{other};
      swap(that);
      return *this;
    }

    array& operator=( array&& other ) requires(!is_dynamic)
    {
      array that{ std::move(other) };
      swap(that);
      return *this;
    }

    void swap( array& that ) requires(!is_dynamic)
    {
      auto ptr = that.data();
      auto old_ptr = view_type::reset(ptr);
      that.reset(old_ptr);
    }

    template<typename SomeShape>
    array(SomeShape const& s) : storage_(s.numel()), view_(storage_.data(),s)
    {}

    template<typename SomeShape> void resize(SomeShape const& s)
    {
      storage_.resize(s.numel());
      view_ = view{storage_.data(),shape_type(s)};
    }

    template<typename SomeShape> void reshape(SomeShape const& s)
    {
      assert( s.numel() == view_.size() && "[array::reshape] - Incompatible shape");
      view_ = view{storage_.data(),shape_type(s)};
    }
*/

    auto as_view() const { return view{data(),shape()}; }

    using parent::size;
    using parent::count;
    using parent::data;
    using parent::begin;
    using parent::end;
    using parent::shape;
    using parent::stride;
    using parent::operator();
  };
}

TTS_CASE( "Build a 1D array with constexpr shape settings" )
{
  kwk::array<float,kwk::_1D> array(kwk::_1D(7));

  //TTS_EQUAL( sizeof(array), array.size()*sizeof(float) );
  TTS_EQUAL( array.size() , 7ULL             );
  TTS_EQUAL( array.count(), 7                );
  TTS_EQUAL( array.shape(), kwk::of_shape(7) );

  // auto v = array.as_view();

  // TTS_EQUAL( sizeof(array), v.size()*sizeof(float) );
  // TTS_EQUAL( v.size() , 7ULL             );
  // TTS_EQUAL( v.count(), 7                );
  // TTS_EQUAL( v.shape(), kwk::of_shape(7) );

  // kwk::array<float,kwk::_1D(7),kwk::heap_> arrayx;
  // TTS_EQUAL( sizeof(arrayx), sizeof(void*)    );
  // TTS_EQUAL( arrayx.size() , 7ULL             );
  // TTS_EQUAL( arrayx.count(), 7                );
  // TTS_EQUAL( arrayx.shape(), kwk::of_shape(7) );

  // kwk::array<float,kwk::_1D> arrayb( kwk::of_shape(77) );

  // TTS_EQUAL( sizeof(arrayb), 2*sizeof(void*)  );
  // TTS_EQUAL( arrayb.size() , 77ULL             );
  // TTS_EQUAL( arrayb.count(), 77                );
  // TTS_EQUAL( arrayb.shape(), kwk::of_shape(77) );

  float k = 1.f;
  for(auto& e : array)
    e = 10*(k++);

  for(auto e : array)
    std::cout << e << " ";
  std::cout << "\n";

  kwk::array<float,kwk::_1D> arrayb(kwk::_1D(15));

  k = 1.f;
  for(auto& e : arrayb)
    e = 10*(k++);

  arrayb.swap(array);

  for(auto e : array)
    std::cout << e << " ";
  std::cout << "\n";

  for(auto e : arrayb)
    std::cout << e << " ";
  std::cout << "\n";

  TTS_EQUAL( arrayb.size() , 7ULL             );
  TTS_EQUAL( arrayb.count(), 7                );
  TTS_EQUAL( arrayb.shape(), kwk::of_shape(7) );

/*
  kwk::array<float,kwk::_1D(7)> arrayc;
  arrayc = array;

  for(auto e : arrayc)
    std::cout << e << " ";
  std::cout << "\n";


  kwk::array<float,kwk::_1D(7)> arrayd = std::move(arrayc);

  for(auto e : arrayd)
    std::cout << e << " ";
  std::cout << "\n";

std::cout << arrayc.data() << "\n";


  kwk::array<float,kwk::_1D(7)> arraye;

  arraye = std::move(arrayb);
  for(auto e : arraye)
    std::cout << e << " ";
  std::cout << "\n";

std::cout << arrayb.data() << "\n";

  //TTS_EXPECT( (std::equal( array.begin(), array.end(), &ref[0])) );
  //TTS_EXPECT( (kwk::all ( [&](auto i) { return array(i) == ref[i]; }, array.shape())) );
*/
}
