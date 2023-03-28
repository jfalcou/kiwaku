#include <kwk/kwk.hpp>
#include <iostream>
#include <chrono>
#include <cassert>
#include <string>
#include "table_initialization.hpp"
#include "view_initialization.hpp"

// Compile with :
// clang++-15 kwk_tests.cpp -o exe -O3 -std=c++20 -I"/home/data_not_sync/logiciels/kiwaku/include" && ./exe

// CMake
/*

export CXX=clang++-15 && cd .. && rm -fr build \ 
&& cmake -B build -S cmake -DCMAKE_PREFIX_PATH="/home/data_not_sync/logiciels/kiwaku/include;/home/data_not_sync/logiciels/kiwaku/cmake" \
&& cd build && make

*/
// -DCMAKE_CXX_STANDARD=20 -DCMAKE_BUILD_TYPE=Release 

// 

// void kiwaku_tables()
// {
//   int width  = 10;
//   int height = 10;
//   auto t1 = kwk::table{kwk::type = kwk::int8, kwk::of_size(width, height)};
//   auto i = t1.at(2, 3);
//   std::cout << "Value = " << i << "\n";
// }



#if 0
void sum_all()
{
  uint const width  = 10;
  uint const height = 10;

  auto ktable = kwk::table{kwk::type = kwk::int8, kwk::of_size(width, height)};
  auto ptr = ktable.get_data();

  uint expected = 0;
  for (uint i = 0; i < width*height; ++i) {
    ptr[i] = i;
    expected += i;
  }

  std::chrono::high_resolution_clock::time_point t1 =
  std::chrono::high_resolution_clock::now();

  uint kcalc = 0;

  auto lb =
    [
    //   fw = static_cast<float>(width)
    // , fh = static_cast<float>(height)
      expected
    , &kcalc
    ]
    (auto& e,  auto pos) 
    {
      // auto[x,y] = pos;
      // float fx = x / fw;
      // float fy = y / fh;
      kcalc += e;
    };

  kwk::for_each( lb , ktable );

  std::cout 
  << "calc     = " << kcalc
  << "expected = " << expected
  << "\n";
}
#endif

template <class T>
void func2(auto&, std::initializer_list<T> list)
{
  for( auto elem : list )
  {
    std::cout << elem << std::endl ;
  }
}

void func3(auto& e, auto... p)
{
  ((std::cout << p << " "), ...) ;
  std::cout << " = " << e;
  std::cout << "\n";
}

// void sum_all()
// {
//   uint const width  = 10; uint const height = 10;
//   auto ktable = kwk::table{kwk::type = kwk::int8, kwk::of_size(width, height)};
//   auto ptr = ktable.get_data();
//   ktable(0, 0) = 'A';


//   // ?????
//   auto lb = [] (auto& e, auto... ps) { func3(e, ps...); };
//   // auto lb = [] (auto& e, auto... ps) { func2(e, {ps...}); };
//   kwk::for_each_index( lb , ktable );

//   // auto lb = [] (auto& e) {};
//   // kwk::for_each( lb , ktable );

// }



void sum_all()
{
  uint const width  = 2; uint const height = 4;
  int data[] = {0, 1, 2, 3, 4, 5, 6, 7}; 
  auto ktable = kwk::table{kwk::source = data, kwk::of_size(width, height)};
  // auto ktable = kwk::table{kwk::source = {0, 1, 2, 3, 4, 5, 6, 7}, kwk::of_size(width, height)};
  // auto ptr = ktable.get_data(); pas fait pour, interface interne uniquement, pas pour les utilisateurs
  ktable(0, 0) = 'A';
  // Initializer list should be a valid way to initialize a kwk::table (problem with range_source const)

  // Exemples à faire
  // Faire std::transform / reduce en Kiwaku directement
  // à la std::transform (source != dest)

  // Faire copy (for_each_index avec capture)
  // Question à répondre : Est-ce qu'il y a besoin de paramètres variadiques ?


  // À un momnent, regarder les slicers et écrire les tests

  // Faire un doc markdown pour l'utilisation de kwk via 

  // Installer la dernière version de dpcpp (repo Intel ?) ou passer à Linux Mint 20

  // Installer Kiwaku avec Sasa


  // ?????
  auto lb = [] (auto& e, auto... ps) { func3(e, ps...); };
  // auto lb = [] (auto& e, auto... ps) { func2(e, {ps...}); };
  kwk::for_each_index( lb , ktable );

  // auto lb = [] (auto& e) {};
  // kwk::for_each( lb , ktable );

}

int main()
{
  table_initialization();
  view_initialization();
  // sum_all();
  std::cout << "It works !\n";
  return 0;
}