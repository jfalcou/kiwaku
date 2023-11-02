//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/concepts/container.hpp>
#include <kwk/concepts/sycl_proxy.hpp>
#include <kwk/context/sycl/proxy.hpp>
#include <kwk/context/base.hpp>
#include <kwk/detail/algorithm/for_each.hpp>
#include <kwk/detail/algorithm/for_until.hpp>
#include <kwk/detail/abi.hpp>
#include <cstddef>
#include <utility>

namespace kwk
{

// J'essaie déjà de faire avec kwk::sycl au lieu de kwk::sycl::context ou kwk::context::sycl
// Joël trouve ça mieux d'utiliser kwk::sycl::context pour qu'on puisse mettre des sous-fonctions
// utiles dans kwk::sycl

namespace sycl
{

  //CONTEXT SYCL
  struct context : private ::sycl::queue, public kwk::base_context<context>
  {
    using parent = ::sycl::queue;
    using parent::parent; // forwards every constructor of queue to context.
    // "using parent::parent" permet d'utiliser tous les constructeurs de ::sycl::queue
    // sans avoir à les redéfinir
  
    static auto in   (kwk::concepts::container auto const& c) { return proxy::in{c};    }
    static auto out  (kwk::concepts::container auto& c)       { return proxy::out{c};   }
    static auto inout(kwk::concepts::container auto& c)       { return proxy::inout{c}; }

    template<typename Func>
    void for_each(Func f, concepts::sycl::proxy auto&& p0, concepts::sycl::proxy auto&&... ps)
    {
      std::cout 
      << "Running on device: "
      << parent::get_device().get_info<::sycl::info::device::name>()
      << "\n";

      parent::submit([&](::sycl::handler &h) 
      {
        // Maps each sycl proxy to an accessor
        auto accs = kumi::map (
                                [&](auto& b) { return b.access(h); }
                              , kumi::tuple{p0, ps...}
                              );

        h.parallel_for(p0.size(), [=](auto i)
        {
          // For each element of the input tables
          // call our lambda parameter with the input accessors
          kumi::apply([=](auto&&... m) { f(KWK_FWD(m)[i]...); }, accs);
        });
      });

      parent::wait();
    }

    // test const algo transform
    void miaou()
    {

    }

    // TODO: comprendre le bug avec le "const" dans transform.hpp -> transform.cpp
    // template< typename Func, concepts::container Out
    //           , concepts::container C0, concepts::container... Cs
    //           >
    // auto transform(Func f, Out& out, C0&& c0, Cs&&... cs) const
    // {
    //   // self().for_each([&](auto... is) { out(is...) = f(KWK_FWD(c0)(is...), KWK_FWD(cs)(is...)...); }, out.shape() );

    //   self().for_each (
    //                     [f](auto& o, auto const& i0, auto const&... in) { o = f(i0, in...); }
    //                   , this->out(out)
    //                   , this->in(c0)
    //                   , this->in(cs)...
    //                   );
    // }

    // template<typename Func, concepts::container In>
    // auto reduce(In const& in, Func f, auto init) const
    // {
    //   // self().for_each([&](auto... is) { init = f(init, in(is...)); }, in.shape() );

    //   self().for_each (
    //                     [&](auto const& i) { init = f(init, i); }
    //                   , this->in(in)
    //                   );
    //   return init;
    // }
  };

  // TODO: adapter les for_each au for_each de sycl

  // exemple
  // template< typename Func, concepts::container Out
  //         , concepts::container C0, concepts::container... Cs
  //         >
  // constexpr auto transform(Func f, Out& out, C0&& c0, Cs&&... cs) const
  // {
  //   // self().for_each([&](auto... is) { out(is...) = f(KWK_FWD(c0)(is...), KWK_FWD(cs)(is...)...); }, out.shape() );

  //   self().for_each (
  //                     [f](auto& o, auto const& i0, auto const&... in) { o = f(i0, in...); }
  //                   , Context::out(out)
  //                   , Context::in(c0)
  //                   , Context::in(cs)...
  //                   );
  // }

  // TODO: ça je sais pas comment faire pour itérer sur les dimensions de la shape
  // (enfin, j'ai pas encore regardé en détail)
  // template<typename Func, auto... S>
  // constexpr auto for_each(Func f, shape<S...> const& shp) const
  // {
  //   return [&]<std::size_t... N>(std::index_sequence<N...> const&)
  //   {
  //     return __::for_each(f, shp );
  //   }( std::make_index_sequence<shape<S...>::static_order>{} );
  // }
  
  // TODO
  // Celui là n'existe plus.
  // Il va donc falloir que j'adapte tous les algos avec le proxy

  // template<typename Func, concepts::container C0, concepts::container... Cs>
  // constexpr auto for_each(Func f, C0&& c0, Cs&&... cs) const
  // {
  //   this->for_each([&](auto... is) { return f(KWK_FWD(c0)(is...), KWK_FWD(cs)(is...)...); }, c0.shape() );
  //   return f;
  // }

  // TODO: voir comment adapter ça aussi
  // template<typename Func, concepts::container Container>
  // constexpr auto for_each_index(Func f, Container&& c) const
  // {
  //   this->for_each ( [&](auto... is) { return f(KWK_FWD(c)(is...), is...); }
  //                 , c.shape()
  //                 );
  //   return f;
  // }


inline kwk::sycl::context sycl_context  = {};
//  error: constexpr variable cannot have non-literal type 'const kwk::sycl::context'
// inline constexpr kwk::sycl::context sycl_context  = {};

// note: 'context' is not literal because it is not an aggregate and has no constexpr constructors other than copy or move constructors
//   struct context : private ::sycl::queue, public kwk::base_context<context>
}
}