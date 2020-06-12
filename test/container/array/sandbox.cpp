//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include "test.hpp"
#include <kiwaku/container/array.hpp>
#include <kiwaku/algorithm/all.hpp>
#include <tts/tests/relation.hpp>
#include <tts/tests/basic.hpp>
#include <vector>

TTS_CASE( "Dynamic storage, constexpr 1D shape array" )
{
  kwk::array<float,kwk::_1D(9),kwk::dynamic_> a;
  TTS_EQUAL( sizeof(a), sizeof(void*)   );
  TTS_EQUAL( a.size() , 9ULL            );
  TTS_EQUAL( a.count(), 9               );
  TTS_EQUAL( a.shape(), kwk::of_shape(9));
}

TTS_CASE( "Automatic storage, constexpr 1D shape array" )
{
  kwk::array<float,kwk::_1D(9),kwk::stack_> a;
  TTS_EQUAL( sizeof(a), a.size()*sizeof(float) );
  TTS_EQUAL( a.size() , 9ULL            );
  TTS_EQUAL( a.count(), 9               );
  TTS_EQUAL( a.shape(), kwk::of_shape(9));
}

TTS_CASE( "Dynamic storage, runtime 1D shape array" )
{
  kwk::array<float,kwk::_1D,kwk::dynamic_> a(kwk::_1D(9));
  TTS_EQUAL( sizeof(a), sizeof(void*) + sizeof(a.count()) );
  TTS_EQUAL( a.size() , 9ULL                              );
  TTS_EQUAL( a.count(), 9                                 );
  TTS_EQUAL( a.shape(), kwk::of_shape(9)                  );
}

/*
TTS_CASE( "Build a 1D array with constexpr shape settings" )
{
  // auto v = array.as_view();

  // TTS_EQUAL( sizeof(array), v.size()*sizeof(float) );
  // TTS_EQUAL( v.size() , 7ULL             );
  // TTS_EQUAL( v.count(), 7                );
  // TTS_EQUAL( v.shape(), kwk::of_shape(7) );

  // kwk::array<float,kwk::_1D(7),kwk::dynamic_> arrayx;
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
  for(auto& e : arraya)
    e = 10*(k++);

  std::cout << "arraya:\n";
  for(auto e : arraya)
    std::cout << e << " ";
  std::cout << "\n";

  // kwk::array<float,kwk::_1D> arrayb(kwk::_1D(15));

  // k = 1.f;
  // for(auto& e : arrayb)
  //   e = 10*(k++);

  // std::cout << "arrayb:\n";
  // for(auto e : arrayb)
  //   std::cout << e << " ";
  // std::cout << "\n";

  // arrayb.swap(arraya);

  // std::cout << "array:\n";
  // for(auto e : arraya)
  //   std::cout << e << " ";
  // std::cout << "\n";

  // std::cout << "arrayb:\n";
  // for(auto e : arrayb)
  //   std::cout << e << " ";
  // std::cout << "\n";

  kwk::array<float,kwk::_1D> arrayd;
  std::cout << "arrayd: " << arrayd.shape() << "\n";

  kwk::array<float,kwk::_1D(9),kwk::dynamic_> arrayc = std::move(arraya);

  std::cout << "arrayc:\n";
  for(auto e : arrayc)
    std::cout << e << " ";
  std::cout << "\n";

  std::cout << "arraya:\n";

  for(auto e : arraya)
    std::cout << e << " ";
  std::cout << "\n";


  // TTS_EQUAL( arrayb.size() , 9ULL             );
  // TTS_EQUAL( arrayb.count(), 9                );
  // TTS_EQUAL( arrayb.shape(), kwk::of_shape(9) );
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
}
*/
