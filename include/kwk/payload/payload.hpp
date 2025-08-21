//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <concepts>
#include <kwk/concepts/container.hpp>
// #include <kwk/context/base.hpp>
#include <kwk/detail/abi.hpp>
#include <cstddef>
#include <utility>

namespace kwk::payload
{ 
  
  template<typename T>
  class in
  {
    public:
      in(kwk::concepts::container& container_) : container(container_) {}
      void is_payload() {}
      void in_payload() {}
    
    // private:
      T& container;
  };

  template<typename T>
  class out
  {
    public:
      out(kwk::concepts::container& container_) : container(container_) {}
      void is_payload() {}
      void out_payload() {}
    
    // private:
      T& container;
  };

  template<typename T>
  class inout
  {
    public:
      inout(kwk::concepts::container& container_) : container(container_) {}
      void is_payload() {}
      void inout_payload() {}
    
    // private:
      T& container;
  };

  // template<kwk::concepts::container T>
  // in(T const&) -> in<typename T::value_type>;

}