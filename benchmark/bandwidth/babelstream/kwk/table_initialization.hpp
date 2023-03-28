#pragma once

#include <kwk/kwk.hpp>
#include <iostream>
#include <chrono>
#include <cassert>
#include <string>

void table_initialization()
{
  std::size_t const width  = 2;
  std::size_t const height = 4;

  // Copy of an array into a kiwaku table
  int tab1[] = {0, 1, 2, 3, 4, 5, 6, 7}; 
  auto ktable1 = kwk::table{kwk::source = tab1, kwk::of_size(width, height)};

  // Filling a kiwaku table with kwk::for_each
  auto ktable2 = kwk::table{kwk::type = kwk::int32, kwk::of_size(width, height)};
  std::size_t c2 = 0;
  auto lamda_fill = [&c2](auto& element) {
    element = (c2++);
  };
  kwk::for_each(lamda_fill, ktable2);


  // Manually, in-place entering values with operator(...)
  auto ktable3 = kwk::table{kwk::type = kwk::int32, kwk::of_size(width, height)};
  std::size_t c3 = 0;
  for (std::size_t x = 0; x < width; ++x) {
    for (std::size_t y = 0; y < height; ++y) {
      ktable3(x, y) = c3;
      ++c3;
    }
  }

  // Accessing data with pointer kwk::table.get_data()
  // INTERNAL USAGE ONLY
  auto ktable4 = kwk::table{kwk::type = kwk::int32, kwk::of_size(width, height)};
  std::size_t c4 = 0;
  for (std::size_t x = 0; x < width; ++x) {
    for (std::size_t y = 0; y < height; ++y) {
      ktable4.get_data()[x * height + y] = c4;
      ++c4;
    }
  }

  // Helper function to test table values
  auto chk_values = [&](int x, int y, int tid1, int tid2) { // , auto v1, auto v2,
    double v1 = 0, v2 = 0;
    if (tid1 == 1) v1 = ktable1(x, y);  if (tid2 == 1) v2 = ktable1(x, y);
    if (tid1 == 2) v1 = ktable2(x, y);  if (tid2 == 2) v2 = ktable2(x, y);
    if (tid1 == 3) v1 = ktable3(x, y);  if (tid2 == 3) v2 = ktable3(x, y);
    if (tid1 == 4) v1 = ktable4(x, y);  if (tid2 == 4) v2 = ktable4(x, y);
    
    if (v1 != v2) {
      std::string err =
      "ERROR: ktable "
      + std::to_string(tid1) + " and " + std::to_string(tid2)
      + " differs at ("
      + std::to_string(x) + ", " + std::to_string(y) + ")  :  "
      + "(" + std::to_string(v1) + ") != "
      + "(" + std::to_string(v2) + ")";
      std::cerr << err << "\n";
    }
  };
  
  // Checking the values (all tables must have the same values)
  for (std::size_t x = 0; x < width; ++x) {
    for (std::size_t y = 0; y < height; ++y) {
      chk_values(x, y, 1, 2);
      chk_values(x, y, 1, 3);
      chk_values(x, y, 1, 4);
    }
  }
}