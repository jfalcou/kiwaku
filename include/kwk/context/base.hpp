//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/detail/abi.hpp>
#include <kwk/detail/kumi.hpp>
#include <cstddef>
#include <utility>

namespace kwk
{

template<typename Context>
struct base_context
{
  protected:
  Context&        self()        { return static_cast<Context&>(*this);       }
  Context const&  self() const  { return static_cast<Context const&>(*this); }

  public:

  template< typename Func, concepts::container Out
          , concepts::container C0, concepts::container... Cs
          >
  constexpr auto transform(Func f, Out& out, C0&& c0, Cs&&... cs) const
  {
    self().for_each([&](auto... is) { out(is...) = f(KWK_FWD(c0)(is...), KWK_FWD(cs)(is...)...); }, out.shape() );
  }

  template<concepts::container Out, concepts::container In>
  constexpr auto copy(Out& out, In&& in) const
  {
    this->transform([](auto in) { return in; }, out, KWK_FWD(in) );
  }

  // //==============================================================================================
  // // n-Dimensional for_each generator
  // // Piles up single for() lambda across each dimension then run them
  // //==============================================================================================
  // template<typename Func, typename Dims>
  // constexpr auto generate_for_each(Func f, Dims const& ds) const
  // {
  //   auto loops = kumi::fold_left ( [](auto acc, auto m)
  //                                   {
  //                                     return [=](auto... is)
  //                                     {
  //                                       for(to_int_t<decltype(m)> i = 0;i<m;++i)
  //                                         acc(is...,i);
  //                                     };
  //                                   }
  //                                 , ds
  //                                 , [&](auto... is) { return f(is...); }
  //                                 );
  //   loops();
  //   return f;
  // }

  // template<typename Func, typename Dims>
  // constexpr auto generate_for_until(Func f, Dims const& ds) const
  // {
  //   bool found = false;
  //   auto loops = kumi::fold_left ( [&found](auto acc, auto m)
  //                                   {
  //                                     return [&found,acc,m](auto... is)
  //                                     {
  //                                       for(to_int_t<decltype(m)> i = 0;!found && i<m;++i)
  //                                         acc(is...,i);
  //                                     };
  //                                   }
  //                                 , ds
  //                                 , [&](auto... is) { found = f(is...); }
  //                                 );
  //   loops();
  //   return f;
  // }
};

}
