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
    self().transform([](auto in) { return in; }, out, KWK_FWD(in) );
  }

  template<typename Func, concepts::container In>
  constexpr auto reduce(In const& in, Func f, auto init) const
  {
    self().for_each([&](auto... is) { init = f(init, in(is...)); }, in.shape() );
    return init;
  }

  template<typename Func, concepts::container In>
  constexpr auto all_of(In const& in, Func f) const
  {
    // Lorsque reduce est redéfini dans la classe appelée, ça devrait être le bon
    // reduce qui sera sélectionné (et pas celui de base)
    // TODO: à tester dans les tests
    return self().reduce(in, [f](auto a, auto e) {  return a && f(e); }, true );
  }

  template<concepts::container In>
  constexpr auto count(In const& in, auto value) const
  {
    // Choses the right method :
    // Takes the reduce(..) method of the Context if it has one
    // Otherwise takes its own reduce(..) method.

    return self().reduce(in, [value](auto a, auto e) { return a + ((e==value) ? 1 : 0); }, std::size_t{0} );
  }



  template<typename Func, concepts::container In>
  constexpr auto any_of(In const& in, Func f) const
  {
    return self().reduce(in, [f](auto a, auto e) {  return a || f(e); }, false );
  }

  template<typename Func, concepts::container In>
  constexpr auto none_of(In const& in, Func f) const
  {
    return !self().any_of(in, f);
  }

  template<typename Func, concepts::container In>
  constexpr auto count_if(In const& in, Func f) const
  {
    return self().reduce(in, [f](auto a, auto e) {  return a + (f(e) ? 1 : 0); }, std::size_t{} );
  }

  template<typename Func, concepts::container Out, concepts::container In>
  constexpr auto copy_if(Func f, Out& out, In&& in) const
  {
    self().for_each([&](auto... is) { if(f(in(is...))) out(is...) = KWK_FWD(in)(is...); }, out.shape() );
  }


















};

}
