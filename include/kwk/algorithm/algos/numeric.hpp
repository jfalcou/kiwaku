//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <kwk/concepts/container.hpp>
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/detail/abi.hpp>
#include <cstddef>
#include <utility>

// ALGO:
/*
partial_sum -> à voir si duplicata inclusive scan

adjacent_difference -> à voir à part
*/

namespace kwk
{
  template<typename Func_R, typename Func_T, concepts::container In>
  constexpr auto transform_reduce(In const& in1, In const& in2, auto init, Func_R R, Func_T T)
  {
    // return error for in1.shape() != in2.shape()
    kwk::for_each([&](auto... is) { init = R(init, T(in1(is...), in2(is...))); }, in1.shape() );
    return init;
  }

  // voir pour avoir version func_T et init sans func_R ?
  template<typename Func_T, concepts::container In>
  constexpr auto transform_reduce(In const& in1, In const& in2, auto init, Func_T T)
  {
    // return error for in1.shape() != in2.shape()
    return kwk::transform_reduce(in1, in2, init, [](auto r, auto d){ return (r+d); },  T);
  }

  template<typename Func_T, concepts::container In>
  constexpr auto transform_reduce(In const& in1, In const& in2, auto init)
  {
    // return error for in1.shape() != in2.shape()
    return kwk::transform_reduce(in1, in2, init, [](auto i1, auto i2){ return (i1+i2); });
  }

  template<typename Func_T, concepts::container In>
  constexpr auto transform_reduce(In const& in1, In const& in2)
  {
    // return error for in1.shape() != in2.shape()
    return kwk::transform_reduce(in1, in2, typename In::value_type{});
  }

  template<typename Func_1, typename Func_2, concepts::container In>
  constexpr auto inner_product(In const& in1, In const& in2, auto init, Func_1 f1, Func_2 f2)
  {
    if(in1.shape() != in2.shape()) return -1;
    //pareil que for each element in in1, in2 ?
    kwk::for_each([&](auto... is) { init = f1(init, f2(in1(is...), in2(is...))); }, in1.shape() );
    return init;
  }

  template<concepts::container In>
  constexpr auto inner_product(In const& in1, In const& in2, auto init)
  {
    return kwk::inner_product(in1, in2, init,
          [](auto r, auto d){ return (r+d); },
          [](auto i1, auto i2){ return (i1*i2); });
  }

  template<concepts::container In>
  constexpr auto inner_product(In const& in1, In const& in2)
  {
    return kwk::inner_product(in1, in2, typename In::value_type{});
  }

  template< typename Func1, typename Func2, concepts::container In, concepts::container Out >
  constexpr auto transform_exclusive_scan(const In in, Out&  out, auto init, Func1 f1, Func2 f2)
  {
    auto sum = init;
    kwk::for_each([&](auto... is)
    {
      out(is...) = sum;
      sum = f1(sum, f2(in(is...)));
    }, in.shape() );
  }

  template< typename Func, concepts::container In, concepts::container Out >
  constexpr auto exclusive_scan(const In in, Out&  out, auto init, Func f)
  {
    kwk::transform_exclusive_scan(in, out, init, f, [](auto e){return e;});
  }

  template< concepts::container In, concepts::container Out >
  constexpr auto exclusive_scan(const In in, Out&  out, auto init)
  {
    kwk::exclusive_scan(in, out, init, [](auto e, auto i){return (e + i);});
  }

  // a corriger
  template< typename Func1, typename Func2, concepts::container In, concepts::container Out >
  constexpr auto transform_inclusive_scan(const In in, Out&  out, auto init, Func1 f1, Func2 f2)
  {
    auto sum = init;
    kwk::for_each([&](auto... is)
    {
      sum = f1(sum, f2(in(is...)));
      out(is...) = sum;
    }, in.shape() );
  }

  template< typename Func, concepts::container In, concepts::container Out >
  constexpr auto inclusive_scan(const In in, Out&  out, auto init, Func f)
  {
    kwk::transform_inclusive_scan(in, out, init, f, [](auto e){return e;});
  }

  template< concepts::container In, concepts::container Out >
  constexpr auto inclusive_scan(const In in, Out&  out, auto init)
  {
    kwk::inclusive_scan(in, out, init, [](auto e, auto i){return (e + i);});
  }
}
