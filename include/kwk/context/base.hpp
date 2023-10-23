//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

// TODO : ça ne compile plus si j'inclus container
// #include <kwk/container.hpp>

#include <kwk/detail/abi.hpp>
#include <kwk/detail/kumi.hpp>
#include <cstddef>
#include <utility>

// #include <kwk/algorithm/algos/for_each.hpp>
// #include <kwk/detail/abi.hpp>
// #include <cstddef>
// #include <utility>

#include <kwk/utility/coordinates.hpp>
#include <optional>

#include <kwk/detail/algorithm/for_until.hpp> // for find
// #include <kwk/algorithm/views/reverse.hpp>
// TODO URGENT : l'include de reverse fait tout planter : error: ‘cpu’ was not declared in this scope

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


  // TODO:
  // kwk::coordinates(it, out.shape());



  // vvv BINARY_SEARCH vvv
  // template <typename Func, concepts::container Out>
  // constexpr auto lower_bound(Out const& out, auto value, Func func) const
  // {
  //   auto count = out.numel();
  //   auto first = 0;
  //   auto it = 0; 
  //   auto step = 0;

  //   while (count > 0)
  //   {
  //       it = first;
  //       step = count / 2; 
  //       it += step;

  //       auto p = kwk::coordinates(it, out.shape());
  //       auto pos = std::apply([](auto... i) { return kumi::tuple{i...}; }, p);
  //       if (func(out(pos), value))
  //       {
  //           first = ++it; 
  //           count -= step + 1; 
  //       }
  //       else
  //           count = step;
  //   }
  //   return (first < out.numel()) ?  std::optional<std::array<int,Out::static_order>>{kwk::coordinates(it, out.shape())} 
  //                                 :  std::nullopt;
  // }

  // // template <concepts::container Out>
  // // constexpr auto lower_bound(Out const& out, auto value) const
  // // {
  // //   return self().lower_bound(out, value, [](auto e, auto i){return e<i;});
  // // }

  // template <typename Func, concepts::container Out>
  // constexpr auto upper_bound(Out const& out, auto value, Func func) const
  // {
  //   auto count = out.numel();
  //   auto first = 0;
  //   auto it = 0; 
  //   auto step = 0;

  //   while (count > 0)
  //   {
  //       it = first; 
  //       step = count / 2; 
  //       it += step;
 
  //       auto p = kwk::coordinates(it, out.shape());
  //       auto pos = std::apply([](auto... i) { return kumi::tuple{i...}; }, p);
  //       if (!func(value, out(pos)))
  //       {
  //           first = ++it; 
  //           count -= step + 1; 
  //       }
  //       else
  //           count = step;
  //   }
  //   return (first < out.numel()) ?  std::optional<std::array<int,Out::static_order>>{kwk::coordinates(it, out.shape())} 
  //                                 :  std::nullopt;
  // }

  // // template <concepts::container Out>
  // // constexpr auto upper_bound(Out const& out, auto value) const
  // // {
  // //   return self().upper_bound( out, value, [](auto e, auto i){return e<i;});
  // // }

  // template <typename Func, concepts::container Out>
  // constexpr bool binary_search(Out const& out, auto value, Func func) const
  // {
  //   auto first = kwk::coordinates(0, out.shape());
  //   auto f = std::apply([](auto... i) { return kumi::tuple{i...}; }, first);

  //   if (func(value, out(f))) return false;

  //   auto p = self().lower_bound(out, value, func);
  //   bool outbound;

  //   if(p)
  //     outbound = false;
  //   else
  //     outbound = true;

  //   return (!outbound);
  // }

  // template <concepts::container Out>
  // constexpr bool binary_search(Out const& out, auto value) const
  // {
  //   return self().binary_search(out, value, [](auto e, auto i){return e<i;});
  // }
  // ^^^ BINARY_SEARCH ^^^



  // // vvv FIND vvv
  template<typename Container, typename Check>
  auto find_if(Container const& c, Check f) const
  {
    auto pos = kumi::generate<Container::static_order>(-1);

    // TODO: make for_until a context method?
    kwk::__::for_until( [&](auto... is)
                        {
                          if(f(c(is...)))
                          {
                            pos = kumi::tuple{is...};
                            return true;
                          }
                          return false;
                        }
                      , c.shape()
                      );

    return pos;
  }

  template <concepts::container Out>
  constexpr auto find(Out const& out, auto value) const
  {
    return self().find_if(out, [&](auto e){return e == value;});
  }

  template <typename Func, concepts::container Out>
  constexpr auto find_if_not(Out const& out, Func f) const
  {
    return self().find_if(out, [f](auto x){return !f(x);});
  }

  template <concepts::container Out, concepts::container Values>
  constexpr auto find_first_of(Out const& out, Values const& in) const
  {
    return self().find_if(out, [&](auto e)
    {
      return self().any_of(in, [&](auto x){return (x==e);});
    });
  }

  // TODO: je n'ai pas pu inclure kwk::reverse (ça fait planter cpu)

  // template <typename Func, concepts::container Out>
  // constexpr auto find_last_if(Out const& out, Func f) const
  // {
  //   auto res = self().find_if(kwk::reverse(out), f);

  //   // TODO: self().for_each?
  //   kumi::for_each([](auto s, auto& r)
  //   {
  //     r = s-1-r;
  //   }, out.shape(), res);

  //   return res;
  // }

  // template <concepts::container Out>
  // constexpr auto find_last(Out const& out, auto value) const
  // {
  //   return self().find_last_if(out, [&](auto e){return e == value;});
  // }

  // template <typename Func, concepts::container Out>
  // constexpr auto find_last_if_not(Out const& out, Func f) const
  // {
  //   return self().find_last_if(out, [f](auto x){return !f(x);});
  // }
  // // ^^^ FIND ^^^

























};

}
