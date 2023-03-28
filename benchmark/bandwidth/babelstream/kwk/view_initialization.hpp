#pragma once

#include "kwk/kwk.hpp"
#include <iostream>
#include <chrono>
#include <cassert>
#include <string>

void view_initialization()
{
  std::size_t const width  = 2;
  std::size_t const height = 4;

  // View of an array
  int tab1[] = {0, 1, 2, 3, 4, 5, 6, 7}; 
  auto kview1 = kwk::view{kwk::source = tab1, kwk::of_size(width, height)};


  // Helper function to test view values
  auto chk_values = [&](int x, int y, int tid1, int tid2) { // , auto v1, auto v2,
    double v1 = 0, v2 = 0;
    if (tid1 == 1) v1 = kview1(x, y); if (tid2 == 1) v2 = kview1(x, y);
    if (tid1 == 2) v2 = tab1[x*height + y];   if (tid2 == 2) v2 = tab1[x*height + y];
    
    if (v1 != v2) {
      std::string err =
      "ERROR: kview "
      + std::to_string(tid1) + " and " + std::to_string(tid2)
      + " differs at ("
      + std::to_string(x) + ", " + std::to_string(y) + ")  :  "
      + "(" + std::to_string(v1) + ") != "
      + "(" + std::to_string(v2) + ")";
      std::cerr << err << "\n";
    }
  };
  
  // Checking the values
  for (std::size_t x = 0; x < width; ++x) {
    for (std::size_t y = 0; y < height; ++y) {
      chk_values(x, y, 1, 2);
    }
  }
}