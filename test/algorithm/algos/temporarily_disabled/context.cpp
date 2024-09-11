//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <cstdlib>
#include <kwk/algorithm/algos/copy.hpp>
#include <kwk/algorithm/algos/replace.hpp>
#include <kwk/algorithm/algos/predicates.hpp>
#include <kwk/algorithm/algos/binary_search.hpp>
#include <kwk/algorithm/algos/generator.hpp>
#include <kwk/container.hpp>
#include <kwk/concepts.hpp>
#include "test.hpp"
#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include <kwk/algorithm/algos/find.hpp>
#include <kwk/algorithm/algos/numeric.hpp>

#include <kwk/concepts/container.hpp>
#include <kwk/context/base.hpp>
#include <kwk/detail/algorithm/for_each.hpp>
#include <kwk/detail/algorithm/for_until.hpp>
#include <kwk/detail/abi.hpp>
#include <cstddef>
#include <utility>

namespace test
{
  const std::size_t d0 = 2;
  const std::size_t d1 = 3;
  int global_data1[d0 * d1];
  int global_data2[d0 * d1];

  auto make_view_1()
  {
    for (std::size_t i = 0; i < d0 * d1; ++i) { global_data1[i] = 0; }
    global_data1[1] = 6;
    global_data1[3] = 6;
    return kwk::view{kwk::source = global_data1, kwk::of_size(d0, d1)};
  }

  auto make_view_2()
  {
    return kwk::view{kwk::source = global_data2, kwk::of_size(d0, d1)};
  }

  struct messaging_context
  {
  public:
    static inline const std::string base_message{"Uninitialized string."};
    // messaging_context() { message = std::make_unique<std::string>(messaging_context::base_message); }
    // std::string get_message() const { return *message; }
    // std::string set_message(auto const& msg) const { *message = std::string{msg}; return *message; }

    messaging_context()
    {
      messages = std::make_unique<std::vector<std::string>>();
    }
    std::string get_message() const
    {
      if (messages->size() == 0) return base_message;
      return messages->at(messages->size()-1);
    }
    std::string get_message(std::size_t i) const
    {
      return messages->at(i);
    }

    std::string set_message(auto const& msg) const
    {
      messages->push_back(std::string{msg});
      return msg;
    }

    void print()
    {
      std::cout << "Messages: ";
      std::size_t i = 0;
      for (std::string const& s : *messages)
      {
        std::cout << s;
        ++i;
        if (i != messages->size()) std::cout << " -> ";
      }
      std::cout << std::endl;
    }

  protected:
    // std::unique_ptr<std::string> message;
    std::unique_ptr<std::vector<std::string>> messages;
  };
}

// Functions from context
// for_each : must be reimplemented by every context
// transform : in base_context
// reduce : in base_context



namespace test
{
  struct context_for_each : kwk::base_context<context_for_each>, messaging_context
  {
    using messaging_context::messaging_context;

    template<typename Func, auto... S>
    constexpr auto map(Func f, kwk::shape<S...> const& shp) const
    {
      set_message("map used!");
      return [&]<std::size_t... N>(std::index_sequence<N...> const&)
      {
        return kwk::__::for_each(f, shp );
      }( std::make_index_sequence<kwk::shape<S...>::static_order>{} );
    }

    template<typename Func, kwk::concepts::container C0, kwk::concepts::container... Cs>
    constexpr auto map(Func f, C0&& c0, Cs&&... cs) const
    {
      this->map([&](auto... is) { return f(KWK_FWD(c0)(is...), KWK_FWD(cs)(is...)...); }, c0.shape() );
      return f;
    }

    template<typename Func, kwk::concepts::container Container>
    constexpr auto map_index(Func f, Container&& c) const
    {
      this->map ( [&](auto... is) { return f(KWK_FWD(c)(is...), is...); }
                    , c.shape()
                    );
      return f;
    }
  };

  struct c_for_each1   : public context_for_each {};
  struct c_for_each2   : public context_for_each {};

  using c_base = context_for_each;
}
namespace kwk
{
  template<typename Func, auto... S>
  constexpr auto for_each(test::c_for_each2& ctx, Func&& f, shape<S...> const& shp)
  {
    ctx.set_message("for_each(ctx, f, shp) used!");
    kwk::for_each(cpu, KWK_FWD(f), shp);
  }

  template<typename Func, concepts::container C0, concepts::container... Cs>
  constexpr auto for_each(test::c_for_each1& ctx, Func f, C0&& c0, Cs&&... cs)
  {
    ctx.set_message("for_each(ctx, f, c0, cs...) used!");
    return kwk::for_each(cpu, f, KWK_FWD(c0), KWK_FWD(cs)...);
  }
}
TTS_CASE("Check for context overload - for_each")
{
  auto v  = test::make_view_1();

  // FOR_EACH (for_each)
  {
    test::context_for_each c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::for_each(c1, [](auto) {}, v);
    TTS_EQUAL(c1.get_message(),   std::string{"map used!"});
  }

  // kwk::for_each overloads
  {
    test::c_for_each1 c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::for_each(c1, [](auto) {}, v);
    TTS_EQUAL(c1.get_message(),   std::string{"for_each(ctx, f, c0, cs...) used!"});

    auto shp = kwk::of_size(4,kwk::fixed<6>,3ULL);
    test::c_for_each2 c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::for_each(c2, [&](auto...) {}, shp);
    TTS_EQUAL(c2.get_message(),   std::string{"for_each(ctx, f, shp) used!"});
  }
};





namespace test
{
  struct c_transform1   : public context_for_each {};

  // struct context_transform : kwk::base_context<context_transform>, messaging_context
  // {
  //   using messaging_context::messaging_context;

  //   template< typename Func, kwk::concepts::container Out
  //           , kwk::concepts::container C0, kwk::concepts::container... Cs
  //           >
  //   constexpr auto transform(Func f, Out& out, C0&& c0, Cs&&... cs) const
  //   {
  //     set_message("transform used!");
  //     kwk::for_each(kwk::cpu, [&](auto... is) { out(is...) = f(KWK_FWD(c0)(is...), KWK_FWD(cs)(is...)...); }, out.shape() );
  //   }
  // };
}
namespace kwk
{
  template< typename Func, concepts::container Out
          , concepts::container C0, concepts::container... Cs
          >
  constexpr auto transform(test::c_transform1& ctx, Func f, Out& out, C0&& c0, Cs&&... cs)
  {
    ctx.set_message("transform(ctx, f, out, c0, cs...) used!");
    transform(f, out, KWK_FWD(c0), KWK_FWD(cs)...);
  }
}
TTS_CASE("Check for context overload - transform")
{
  auto v  = test::make_view_1();
  auto v2 = test::make_view_2();

  // TRANSFORM (transform -> for_each)
  {
    // test::context_transform c1;
    // TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    // kwk::transform(c1, [](auto e) { return e+1; }, v, v2);
    // TTS_EQUAL(c1.get_message(),   std::string{"transform used!"});

    test::context_for_each c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::transform(c2, [](auto e) { return e+1; }, v, v2);
    TTS_EQUAL(c2.get_message(),   std::string{"map used!"});
  }

  // kwk::transform specialization
  {
    test::c_transform1 c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::transform(c1, [](auto e) { return e+1; }, v, v2);
    TTS_EQUAL(c1.get_message(),   std::string{"transform(ctx, f, out, c0, cs...) used!"});
  }
};






namespace test
{
  struct c_reduce1   : public context_for_each {};
  struct c_reduce2   : public context_for_each {};
  struct c_reduce3   : public context_for_each {};

  // struct context_reduce : kwk::base_context<context_reduce>, messaging_context
  // {
  //   using messaging_context::messaging_context;

  //   template<typename Func, kwk::concepts::container In>
  //   constexpr auto reduce(In const& in, Func f, auto init) const
  //   {
  //     set_message("reduce used!");
  //     return kwk::reduce(in, f, init);
  //   }
  // };
}
namespace kwk
{

  template<typename Func, concepts::container In>
  constexpr auto reduce(test::c_reduce3& ctx, In const& in, Func f, auto init)
  {
    ctx.set_message("reduce(ctx, in, f, init) used!");
    return reduce(in, f, init);
  }

  template<typename Func, concepts::container In>
  constexpr auto reduce(test::c_reduce2& ctx, In const& in, Func f)
  {
    ctx.set_message("reduce(ctx, in, f) used!");
    return reduce(in, f);
  }

  template<concepts::container In>
  constexpr auto reduce(test::c_reduce1& ctx, In const& in)
  {
    ctx.set_message("reduce(ctx, in) used!");
    return reduce(in);
  }
}
TTS_CASE("Check for context overload - reduce")
{
  auto v  = test::make_view_1();

  // Reduce context (reduce -> for_each)
  {
    // test::context_reduce c1;
    // TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    // kwk::reduce(c1, v);
    // TTS_EQUAL(c1.get_message(),   std::string{"reduce used!"});

    test::context_for_each c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::reduce(c2, v, [](auto a, auto e){ return (a+e); });
    TTS_EQUAL(c2.get_message(),   std::string{"map used!"});

    test::context_for_each c3;
    TTS_EQUAL(c3.get_message(),   test::messaging_context::base_message);
    kwk::reduce(c3, v, [](auto a, auto e){ return (a+e); }, 100);
    TTS_EQUAL(c3.get_message(),   std::string{"map used!"});
  }

  // Reduce overloads
  {
    test::c_reduce1 c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::reduce(c1, v);
    TTS_EQUAL(c1.get_message(),   std::string{"reduce(ctx, in) used!"});

    test::c_reduce2 c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::reduce(c2, v);
    TTS_EQUAL(c2.get_message(),   std::string{"reduce(ctx, in, f) used!"});

    test::c_reduce3 c3;
    TTS_EQUAL(c3.get_message(),   test::messaging_context::base_message);
    kwk::reduce(c3, v);
    TTS_EQUAL(c3.get_message(),   std::string{"reduce(ctx, in, f, init) used!"});

    test::context_for_each c4;
    TTS_EQUAL(c4.get_message(),   test::messaging_context::base_message);
    kwk::reduce(c4, v);
    TTS_EQUAL(c4.get_message(),   std::string{"map used!"});
  }

};







namespace test
{
  struct c_upper_bound1   : public context_for_each {};
  struct c_upper_bound2   : public context_for_each {};
  struct c_lower_bound1   : public context_for_each {};
  struct c_lower_bound2   : public context_for_each {};
  struct c_binary_search1 : public context_for_each {};
  struct c_binary_search2 : public context_for_each {};
}
namespace kwk
{
  template <typename Func, concepts::container Out>
  constexpr auto lower_bound(test::c_lower_bound2& c, Out const& out, auto value, Func func)
  {
    c.set_message("lower_bound(ctx, out, value, func) used!");
    return lower_bound(out, value, func);
  }
  template <concepts::container Out>
  constexpr auto lower_bound(test::c_lower_bound1& c, Out const& out, auto value)
  {
    c.set_message("lower_bound(ctx, out, value) used!");
    return lower_bound(out, value);
  }

  template <typename Func, concepts::container Out>
  constexpr auto upper_bound(test::c_upper_bound2& c, Out const& out, auto value, Func func)
  {
    c.set_message("upper_bound(ctx, out, value, func) used!");
    return upper_bound(out, value, func);
  }
  template <concepts::container Out>
  constexpr auto upper_bound(test::c_upper_bound1& c, Out const& out, auto value)
  {
    c.set_message("upper_bound(ctx, out, value) used!");
    return upper_bound(out, value);
  }

  template <typename Func, concepts::container Out>
  constexpr auto binary_search(test::c_binary_search2& c, Out const& out, auto value, Func func)
  {
    c.set_message("binary_search(ctx, out, value, func) used!");
    return binary_search(out, value, func);
  }
  template <concepts::container Out>
  constexpr auto binary_search(test::c_binary_search1& c, Out const& out, auto value)
  {
    c.set_message("binary_search(ctx, out, value) used!");
    return binary_search(out, value);
  }
}
TTS_CASE("Check for context overload - lower/upper_bound & binary_search")
{
  auto v  = test::make_view_1();

  // LOWER_BOUND (lower_bound -> kwk::coordinates)
  {
    test::c_lower_bound1 c1;
    TTS_EQUAL(c1.get_message(), test::c_base::base_message);
    kwk::lower_bound(c1, v, 21);
    TTS_EQUAL(c1.get_message(), std::string{"lower_bound(ctx, out, value) used!"});

    test::c_lower_bound2 c2;
    TTS_EQUAL(c2.get_message(), test::c_base::base_message);
    kwk::lower_bound(c2, v, 21, [](auto e, auto s) { return e < (s-1); });
    TTS_EQUAL(c2.get_message(), std::string{"lower_bound(ctx, out, value, func) used!"});

    test::c_lower_bound2 c3;
    TTS_EQUAL(c3.get_message(), test::c_base::base_message);
    kwk::lower_bound(c3, v, 21);
    TTS_EQUAL(c3.get_message(), std::string{"lower_bound(ctx, out, value, func) used!"});
  }

  // UPPER_BOUND (upper_bound -> kwk::coordinates)
  {
    test::c_upper_bound1 c2;
    TTS_EQUAL(c2.get_message(), test::c_base::base_message);
    kwk::upper_bound(c2, v, 21);
    TTS_EQUAL(c2.get_message(), std::string{"upper_bound(ctx, out, value) used!"});

    test::c_upper_bound2 c1;
    TTS_EQUAL(c1.get_message(), test::c_base::base_message);
    kwk::upper_bound(c1, v, 21, [](auto, auto) {return true;});
    TTS_EQUAL(c1.get_message(), std::string{"upper_bound(ctx, out, value, func) used!"});


    test::c_upper_bound2 c3;
    TTS_EQUAL(c3.get_message(), test::c_base::base_message);
    kwk::upper_bound(c3, v, 21);
    TTS_EQUAL(c3.get_message(), std::string{"upper_bound(ctx, out, value, func) used!"});
  }

  // BINARY_SEARCH (binary_search -> lower_bound -> kwk::coordinates)
  {
    test::c_binary_search1 c1;
    TTS_EQUAL(c1.get_message(),   test::c_base::base_message);
    kwk::binary_search(c1, v, 4);
    TTS_EQUAL(c1.get_message(),   std::string{"binary_search(ctx, out, value) used!"});

    test::c_binary_search2 c2;
    TTS_EQUAL(c2.get_message(),   test::c_base::base_message);
    kwk::binary_search(c2, v, 4, [](auto e, auto s){return e < s;});
    TTS_EQUAL(c2.get_message(),   std::string{"binary_search(ctx, out, value, func) used!"});

    test::c_lower_bound2 c3;
    TTS_EQUAL(c3.get_message(),   test::c_base::base_message);
    kwk::binary_search(c3, v, 21);
    TTS_EQUAL(c3.get_message(),   std::string{"lower_bound(ctx, out, value, func) used!"}); // TODO: ERREUR ALÉATOIRE ICI, COMPRENDRE POURQUOI
    // TODO: vérifier que l'erreur suivante n'arrive plus, même 1 fois sur 10:
    //     [context.cpp:341] - ** FAILURE ** : Expression: c3.get_message() == std::string{"lower_bound used!"} is false because: Uninitialized string. != lower_bound used!
  }
};








namespace test
{
  struct c_copy    : public context_for_each {};
  struct c_copy_if : public context_for_each {};
}
namespace kwk
{
  template<kwk::concepts::container Out, kwk::concepts::container In>
  constexpr auto copy(test::c_copy& ctx, Out& out, In&& in)
  {
    ctx.set_message("copy used!");
    return copy(out, in);
  }

  template<typename Func, concepts::container Out, concepts::container In>
  constexpr auto copy_if(test::c_copy_if& ctx, Func f, Out& out, In&& in)
  {
    ctx.set_message("copy_if used!");
    return copy_if(f, out, in);
  }

  // template< typename Func, concepts::container Out
  //         , concepts::container C0, concepts::container... Cs
  //         >
  // constexpr auto transform(test::c_transform1& ctx, Func f, Out& out, C0&& c0, Cs&&... cs)
  // {
  //   ctx.set_message("transform(ctx, f, out, c0, cs...) used!");
  //   transform(f, out, KWK_FWD(c0), KWK_FWD(cs)...);
  // }
}
TTS_CASE("Check for context overload - copy & copy_if")
{
  auto v  = test::make_view_1();
  auto v2 = test::make_view_2();

  // COPY (copy -> transform -> for_each)
  {
    test::c_copy c1;
    TTS_EQUAL(c1.get_message(), test::c_base::base_message);
    kwk::copy(c1, v, v2);
    TTS_EQUAL(c1.get_message(), std::string{"copy used!"});

    test::c_transform1 c2;
    TTS_EQUAL(c2.get_message(),   test::c_base::base_message);
    kwk::copy(c2, v, v2);
    TTS_EQUAL(c2.get_message(),   std::string{"transform(ctx, f, out, c0, cs...) used!"});

    test::context_for_each c3;
    TTS_EQUAL(c3.get_message(),   test::c_base::base_message);
    kwk::copy(c3, v, v2);
    TTS_EQUAL(c3.get_message(),   std::string{"map used!"});
  }

  // COPY_IF (copy_if -> for_each)
  {
    test::c_copy_if c1;
    TTS_EQUAL(c1.get_message(),   test::c_base::base_message);
    kwk::copy_if(c1, [](auto) {return true;}, v, v2);
    TTS_EQUAL(c1.get_message(),   std::string{"copy_if used!"});

    test::context_for_each c2;
    TTS_EQUAL(c2.get_message(),   test::c_base::base_message);
    kwk::copy_if(c2, [](auto) {return true;}, v, v2);
    TTS_EQUAL(c2.get_message(),   std::string{"map used!"});
  }
};








namespace test
{
  struct c_all_of   : public context_for_each {};
  struct c_any_of   : public context_for_each {};
  struct c_none_of  : public context_for_each {};
  struct c_count    : public context_for_each {};
  struct c_count_if : public context_for_each {};
}
namespace kwk
{
  template<typename Func, kwk::concepts::container In>
  constexpr auto all_of(test::c_all_of& ctx, In const& in, Func f)
  {
    ctx.set_message("all_of used!");
    return kwk::all_of(in, f);
  }

  template<typename Func, kwk::concepts::container In>
  constexpr auto any_of(test::c_any_of& ctx, In const& in, Func f)
  {
    ctx.set_message("any_of used!");
    return kwk::any_of(in, f);
  }

  template<typename Func, kwk::concepts::container In>
  constexpr auto none_of(test::c_none_of& ctx, In const& in, Func f)
  {
    ctx.set_message("none_of used!");
    return kwk::none_of(in, f);
  }

  template<kwk::concepts::container In>
  constexpr auto count(test::c_count& ctx, In const& in, auto value)
  {
    ctx.set_message("count used!");
    return kwk::count(in, value);
  }

  template<typename Func, kwk::concepts::container In>
  constexpr auto count_if(test::c_count_if& ctx, In const& in, Func f)
  {
    ctx.set_message("count_if used!");
    return kwk::count_if(in, f);
  }
};
TTS_CASE("Check for context overload - predicates: all_of, any_of, none_of, count, count_if")
{
  auto v  = test::make_view_1();

  // all_of -> reduce -> for_each
  {
    test::c_all_of c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::all_of(c1, v, [](auto e) { return (e == 0); });
    TTS_EQUAL(c1.get_message(),   std::string{"all_of used!"});

    test::c_reduce3 c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::all_of(c2, v, [](auto e) { return (e == 0); });
    TTS_EQUAL(c2.get_message(),   std::string{"reduce(ctx, in, f, init) used!"});

    test::context_for_each c3;
    TTS_EQUAL(c3.get_message(),   test::messaging_context::base_message);
    kwk::all_of(c3, v, [](auto e) { return (e == 0); });
    TTS_EQUAL(c3.get_message(),   std::string{"map used!"});
  }

  // COUNT (count -> reduce -> for_each)
  {
    test::c_count c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::count(c1, v, 6);
    TTS_EQUAL(c1.get_message(),   std::string{"count used!"});

    test::c_reduce3 c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::count(c2, v, 6);
    TTS_EQUAL(c2.get_message(),   std::string{"reduce(ctx, in, f, init) used!"});

    test::context_for_each c3;
    TTS_EQUAL(c3.get_message(),   test::messaging_context::base_message);
    kwk::count(c3, v, 6);
    TTS_EQUAL(c3.get_message(),   std::string{"map used!"});
  }

  // ANY_OF (any_of -> reduce -> for_each)
  {
    test::c_any_of c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::any_of(c1, v, [](auto e) { return (e == 0); });
    TTS_EQUAL(c1.get_message(),   std::string{"any_of used!"});

    test::c_reduce3 c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::any_of(c2, v, [](auto e) { return (e == 0); });
    TTS_EQUAL(c2.get_message(),   std::string{"reduce(ctx, in, f, init) used!"});

    test::context_for_each c3;
    TTS_EQUAL(c3.get_message(),   test::messaging_context::base_message);
    kwk::any_of(c3, v, [](auto e) { return (e == 0); });
    TTS_EQUAL(c3.get_message(),   std::string{"map used!"});
  }

  // NONE_OF (none_of -> !any_of -> reduce -> for_each)
  {
    test::c_none_of c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::none_of(c1, v, [](auto e) { return (e == 0); });
    TTS_EQUAL(c1.get_message(),   std::string{"none_of used!"});

    test::c_any_of c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::none_of(c2, v, [](auto e) { return (e == 0); });
    TTS_EQUAL(c2.get_message(),   std::string{"any_of used!"});

    test::c_reduce3 c3;
    TTS_EQUAL(c3.get_message(),   test::messaging_context::base_message);
    kwk::none_of(c3, v, [](auto e) { return (e == 0); });
    TTS_EQUAL(c3.get_message(),   std::string{"reduce(ctx, in, f, init) used!"});

    test::context_for_each c4;
    TTS_EQUAL(c4.get_message(),   test::messaging_context::base_message);
    kwk::none_of(c4, v, [](auto e) { return (e == 0); });
    TTS_EQUAL(c4.get_message(),   std::string{"map used!"});
  }

  // COUNT_IF (count_if -> reduce -> for_each)
  {
    test::c_count_if c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::count_if(c1, v, [](auto e) { return (e == 0); });
    TTS_EQUAL(c1.get_message(),   std::string{"count_if used!"});

    test::c_reduce3 c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::count_if(c2, v, [](auto e) { return (e == 0); });
    TTS_EQUAL(c2.get_message(),   std::string{"reduce(ctx, in, f, init) used!"});

    test::context_for_each c3;
    TTS_EQUAL(c3.get_message(),   test::messaging_context::base_message);
    kwk::count_if(c3, v, [](auto e) { return (e == 0); });
    TTS_EQUAL(c3.get_message(),   std::string{"map used!"});
  }
};





// TODO: uncomment "find" tests when kwk::reverse will work again
// TODO: uncomment "find" tests when kwk::reverse will work again

namespace test
{
  struct c_find             : public context_for_each {};
  struct c_find_if          : public context_for_each {};
  struct c_find_if_not      : public context_for_each {};
  struct c_find_first_of    : public context_for_each {};
  struct c_find_last        : public context_for_each {};
  struct c_find_last_if     : public context_for_each {};
  struct c_find_last_if_not : public context_for_each {};
}
namespace kwk
{
  template<typename Container, typename Check>
  constexpr auto find_if(test::c_find_if& ctx, Container const& c, Check cc)
  {
    ctx.set_message("find_if used!");
    return find_if(cpu, c, cc);
  }

  template <concepts::container Out>
  constexpr auto find(test::c_find& ctx, Out const& o, auto v)
  {
    ctx.set_message("find used!");
    return find(cpu, o, v);
  }

  template <typename Func, concepts::container Out>
  constexpr auto find_if_not(test::c_find_if_not& ctx, Out const& o, Func f)
  {
    ctx.set_message("find_if_not used!");
    return find_if_not(cpu, o, f);
  }

  template <concepts::container Out, concepts::container Values>
  constexpr auto find_first_of(test::c_find_first_of& ctx, Out const& o, Values const& v)
  {
    ctx.set_message("find_first_of used!");
    return find_first_of(cpu, o, v);
  }

  template <typename Func, concepts::container Out>
  constexpr auto find_last_if(test::c_find_last_if& ctx, Out const& o, Func f)
  {
    ctx.set_message("find_last_if used!");
    return find_last_if(cpu, o, f);
  }

  template <concepts::container Out>
  constexpr auto find_last(test::c_find_last& ctx, Out const& o, auto v)
  {
    ctx.set_message("find_last used!");
    return find_last(cpu, o, v);
  }

  template <typename Func, concepts::container Out>
  constexpr auto find_last_if_not(test::c_find_last_if_not& ctx, Out const& o, Func f)
  {
    ctx.set_message("find_last_if_not used!");
    return find_last_if_not(cpu, o, f);
  }

  // find_if -> kwk::__::for_until
  // find_if_not -> find_if
  // find_first_of -> find_if
  // find_first_of -> any_of -> reduce -> for_each
  // find -> find_if
  // find_last_if -> find_if
  // find_last -> find_last_if -> find_if
  // find_last_if_not -> find_last_if -> find_if
};

TTS_CASE("Check for context overload - initially from find.hpp")
{
  auto v  = test::make_view_1();

  // find_if -> kwk::__::for_until
  {
    test::c_find_if c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::find_if(c1, v, [&](auto){ return true; });
    TTS_EQUAL(c1.get_message(),   std::string{"find_if used!"});

    // kwk::__::for_until
    // test::context_for_until c2;
    // TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    // kwk::find_if(c2, v, [&](auto e){ return true; });
    // TTS_EQUAL(c2.get_message(),   std::string{"for_until used!"});
  }

  // find -> find_if -> kwk::__::for_until
  {
    test::c_find c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::find(c1, v, 12);
    TTS_EQUAL(c1.get_message(),   std::string{"find used!"});

    test::c_find_if c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::find(c2, v, 12);
    TTS_EQUAL(c2.get_message(),   std::string{"find_if used!"});
  }

  // find_if_not -> find_if -> kwk::__::for_until
  {
    test::c_find_if_not c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::find_if_not(c1, v, [&](auto){ return true; });
    TTS_EQUAL(c1.get_message(),   std::string{"find_if_not used!"});

    test::c_find_if c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::find_if_not(c2, v, [&](auto){ return true; });
    TTS_EQUAL(c2.get_message(),   std::string{"find_if used!"});
  }

  auto v2 = test::make_view_2();

  // find_first_of -> find_if
  // find_first_of -> any_of -> reduce -> for_each
  {
    // vvv  find_first_of -> find_if -> kwk::__::for_until  vvv
    test::c_find_first_of c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::find_first_of(c1, v, v2);
    TTS_EQUAL(c1.get_message(),   std::string{"find_first_of used!"});

    test::c_find_if c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::find_first_of(c2, v, v2);
    TTS_EQUAL(c2.get_message(0),   std::string{"find_if used!"});
    TTS_EQUAL(c2.get_message(1),   std::string{"map used!"});

    // vvv  find_first_of -> any_of -> reduce -> for_each  vvv
    test::c_any_of c3;
    TTS_EQUAL(c3.get_message(),   test::messaging_context::base_message);
    kwk::find_first_of(c3, v, v2);
    TTS_EQUAL(c3.get_message(),   std::string{"any_of used!"});

    test::c_reduce3 c4;
    TTS_EQUAL(c4.get_message(),   test::messaging_context::base_message);
    kwk::find_first_of(c4, v, v2);
    TTS_EQUAL(c4.get_message(),   std::string{"reduce(ctx, in, f, init) used!"});

    test::context_for_each c5;
    TTS_EQUAL(c5.get_message(),   test::messaging_context::base_message);
    kwk::find_first_of(c5, v, v2);
    TTS_EQUAL(c5.get_message(),   std::string{"map used!"});
  }

  // TODO: finir cette partie lorsque les predicates seront finis eux aussi

  // find_last_if -> find_if -> kwk::__::for_until
  {
    test::c_find_last_if c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::find_last_if(c1, v, [&](auto){ return true; });
    TTS_EQUAL(c1.get_message(),   std::string{"find_last_if used!"});

    test::c_find_if c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::find_last_if(c2, v, [&](auto){ return true; });
    TTS_EQUAL(c2.get_message(),   std::string{"find_if used!"});
  }

  // find_last -> find_last_if -> find_if -> kwk::__::for_until
  {
    test::c_find_last c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::find_last(c1, v, 4);
    TTS_EQUAL(c1.get_message(),   std::string{"find_last used!"});

    test::c_find_last_if c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::find_last(c2, v, 4);
    TTS_EQUAL(c2.get_message(),   std::string{"find_last_if used!"});

    test::c_find_if c3;
    TTS_EQUAL(c3.get_message(),   test::messaging_context::base_message);
    kwk::find_last(c3, v, 4);
    TTS_EQUAL(c3.get_message(),   std::string{"find_if used!"});
  }

  // find_last_if_not -> find_last_if -> find_if -> kwk::__::for_until
  {
    test::c_find_last_if_not c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::find_last_if_not(c1, v, [](auto e) { return (e == 0); });
    TTS_EQUAL(c1.get_message(),   std::string{"find_last_if_not used!"});

    test::c_find_last_if c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::find_last_if_not(c2, v, [](auto e) { return (e == 0); });
    TTS_EQUAL(c2.get_message(),   std::string{"find_last_if used!"});

    test::c_find_if c3;
    TTS_EQUAL(c3.get_message(),   test::messaging_context::base_message);
    kwk::find_last_if_not(c3, v, [](auto e) { return (e == 0); });
    TTS_EQUAL(c3.get_message(),   std::string{"find_if used!"});
  }
};






namespace test
{
  struct c_fill      : public context_for_each {};
  struct c_generate  : public context_for_each {};
  struct c_iota1     : public context_for_each {};
  struct c_iota2     : public context_for_each {};
  struct c_iota3     : public context_for_each {};
}
namespace kwk
{
  template<kwk::concepts::container Out>
  constexpr auto fill(test::c_fill& ctx, Out& out, auto value)
  {
    ctx.set_message("fill used!");
    kwk::fill(cpu, out, value);
  }

  template<typename Func, kwk::concepts::container Out>
  constexpr auto generate(test::c_generate& ctx, Func f, Out& out)
  {
    ctx.set_message("generate used!");
    kwk::generate(cpu, f, out);
  }

  template<kwk::concepts::container Out>
  constexpr auto iota(test::c_iota2& ctx, Out& out, auto value)
  {
    ctx.set_message("iota(ctx, out, value) used!");
    kwk::iota(cpu, out, value);
  }

  template<kwk::concepts::container Out>
  constexpr auto iota(test::c_iota1& ctx, Out& out)
  {
    ctx.set_message("iota(ctx, out) used!");
    kwk::iota(cpu, out);
  }

  template<kwk::concepts::container Out>
  constexpr auto iota(test::c_iota3& ctx, Out& out, auto value, auto step)
  {
    ctx.set_message("iota(ctx, out, value, step) used!");
    kwk::iota(cpu, out, value, step);
  }
};

TTS_CASE("Check for context overload - generator: fill, generate, iota")
{
  auto v  = test::make_view_1();

  // fill -> for_each
  {
    test::c_fill c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::fill(c1, v, 2);
    TTS_EQUAL(c1.get_message(),   std::string{"fill used!"});

    test::context_for_each c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::fill(c2, v, 2);
    TTS_EQUAL(c2.get_message(),   std::string{"map used!"});
  }

  // generate -> for_each
  {
    test::c_generate c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::generate(c1, [](auto p1, auto p2) { return p1*10+p2; }, v);
    TTS_EQUAL(c1.get_message(),   std::string{"generate used!"});

    test::context_for_each c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::generate(c2, [](auto p1, auto p2) { return p1*10+p2; }, v);
    TTS_EQUAL(c2.get_message(),   std::string{"map used!"});
  }

  // iota -> for_each
  {
    test::c_iota1 c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::iota(c1, v);
    TTS_EQUAL(c1.get_message(),   std::string{"iota(ctx, out) used!"});

    test::c_iota2 c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::iota(c2, v);
    TTS_EQUAL(c2.get_message(),   std::string{"iota(ctx, out, value) used!"});

    test::c_iota3 c3;
    TTS_EQUAL(c3.get_message(),   test::messaging_context::base_message);
    kwk::iota(c3, v, 0, 20);
    TTS_EQUAL(c3.get_message(),   std::string{"iota(ctx, out, value, step) used!"});

    test::context_for_each c4;
    TTS_EQUAL(c4.get_message(),   test::messaging_context::base_message);
    kwk::iota(c4, v);
    TTS_EQUAL(c4.get_message(),   std::string{"map used!"});
  }

};






namespace test
{
  struct c_transform_reduce1        : public context_for_each {};
  struct c_transform_reduce2        : public context_for_each {};
  struct c_transform_reduce3        : public context_for_each {};
  struct c_transform_reduce4        : public context_for_each {};
  struct c_inner_product1           : public context_for_each {};
  struct c_inner_product2           : public context_for_each {};
  struct c_inner_product3           : public context_for_each {};
  struct c_transform_exclusive_scan : public context_for_each {};
  struct c_exclusive_scan1          : public context_for_each {};
  struct c_exclusive_scan2          : public context_for_each {};
  struct c_transform_inclusive_scan : public context_for_each {};
  struct c_inclusive_scan1          : public context_for_each {};
  struct c_inclusive_scan2          : public context_for_each {};
}
namespace kwk
{
  template<typename Func_R, typename Func_T, concepts::container In>
  constexpr auto transform_reduce(test::c_transform_reduce4& ctx, In const& in1, In const& in2, auto init, Func_R R, Func_T T)
  {
    ctx.set_message("transform_reduce(ctx, in1, in2, init, R, T) used!");
    return transform_reduce(cpu, in1, in2, init, R, T);
  }

  template<typename Func_T, concepts::container In>
  constexpr auto transform_reduce(test::c_transform_reduce3& ctx, In const& in1, In const& in2, auto init, Func_T T)
  {
    ctx.set_message("transform_reduce(ctx, in1, in2, init, T) used!");
    return transform_reduce(cpu, in1, in2, init, T);
  }

  template<concepts::container In>
  constexpr auto transform_reduce(test::c_transform_reduce2& ctx, In const& in1, In const& in2, auto init)
  {
    ctx.set_message("transform_reduce(ctx, in1, in2, init) used!");
    return transform_reduce(cpu, in1, in2, init);
  }

  template<concepts::container In>
  constexpr auto transform_reduce(test::c_transform_reduce1& ctx, In const& in1, In const& in2)
  {
    ctx.set_message("transform_reduce(ctx, in1, in2) used!");
    return transform_reduce(cpu, in1, in2);
  }

  template<typename Func_1, typename Func_2, concepts::container In>
  constexpr auto inner_product(test::c_inner_product3& ctx, In const& in1, In const& in2, auto init, Func_1 f1, Func_2 f2)
  {
    ctx.set_message("inner_product(ctx, in1, in2, init, f1, f2) used!");
    return inner_product(cpu, in1, in2, init, f1, f2);
  }

  template<concepts::container In>
  constexpr auto inner_product(test::c_inner_product2& ctx, In const& in1, In const& in2, auto init)
  {
    ctx.set_message("inner_product(ctx, in1, in2, init) used!");
    return inner_product(cpu, in1, in2, init);
  }

  template<concepts::container In>
  constexpr auto inner_product(test::c_inner_product1& ctx, In const& in1, In const& in2)
  {
    ctx.set_message("inner_product(ctx, in1, in2) used!");
    return inner_product(cpu, in1, in2);
  }

  template<typename Func1, typename Func2, concepts::container In, concepts::container Out >
  constexpr auto transform_exclusive_scan(test::c_transform_exclusive_scan& ctx, const In in, Out&  out, auto init, Func1 f1, Func2 f2)
  {
    ctx.set_message("transform_exclusive_scan(ctx, in, out, init, f1, f2) used!");
    transform_exclusive_scan(cpu, in, out, init, f1, f2);
  }

  template<typename Func, concepts::container In, concepts::container Out >
  constexpr auto exclusive_scan(test::c_exclusive_scan2& ctx, const In in, Out&  out, auto init, Func f)
  {
    ctx.set_message("exclusive_scan(ctx, in, out, init, f) used!");
    exclusive_scan(cpu, in, out, init, f);
  }

  template<concepts::container In, concepts::container Out >
  constexpr auto exclusive_scan(test::c_exclusive_scan1& ctx, const In in, Out&  out, auto init)
  {
    ctx.set_message("exclusive_scan(ctx, in, out, init) used!");
    exclusive_scan(cpu, in, out, init);
  }

  // (Sasa?) a corriger
  template<typename Func1, typename Func2, concepts::container In, concepts::container Out >
  constexpr auto transform_inclusive_scan(test::c_transform_inclusive_scan& ctx, const In in, Out&  out, auto init, Func1 f1, Func2 f2)
  {
    ctx.set_message("transform_inclusive_scan(ctx, in, out, init, f1, f2) used!");
    transform_inclusive_scan(cpu, in, out, init, f1, f2);
  }

  template<typename Func, concepts::container In, concepts::container Out >
  constexpr auto inclusive_scan(test::c_inclusive_scan2& ctx, const In in, Out&  out, auto init, Func f)
  {
    ctx.set_message("inclusive_scan(ctx, in, out, init, f) used!");
    inclusive_scan(cpu, in, out, init, f);
  }

  template<concepts::container In, concepts::container Out >
  constexpr auto inclusive_scan(test::c_inclusive_scan1& ctx, const In in, Out&  out, auto init)
  {
    ctx.set_message("inclusive_scan(ctx, in, out, init) used!");
    inclusive_scan(cpu, in, out, init);
  }

};

TTS_CASE("Check for context overload - numeric: transform_reduce, inner_product"
         ", transform_exclusive_scan, exclusive_scan, transform_inclusive_scan, inclusive_scan")
{
  auto v   = test::make_view_1();
  auto v2  = test::make_view_2();

  // transform_reduce overloads
  {
    test::c_transform_reduce1 c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::transform_reduce(c1, v, v2);
    TTS_EQUAL(c1.get_message(),   std::string{"transform_reduce(ctx, in1, in2) used!"});

    test::c_transform_reduce2 c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    transform_reduce(c2, v, v2);
    TTS_EQUAL(c2.get_message(),   std::string{"transform_reduce(ctx, in1, in2, init) used!"});

    test::c_transform_reduce3 c3;
    TTS_EQUAL(c3.get_message(),   test::messaging_context::base_message);
    kwk::transform_reduce(c3, v, v2);
    TTS_EQUAL(c3.get_message(),   std::string{"transform_reduce(ctx, in1, in2, init, T) used!"});

    test::c_transform_reduce4 c4;
    TTS_EQUAL(c4.get_message(),   test::messaging_context::base_message);
    kwk::transform_reduce(c4, v, v2);
    TTS_EQUAL(c4.get_message(),   std::string{"transform_reduce(ctx, in1, in2, init, R, T) used!"});

    test::context_for_each c5;
    TTS_EQUAL(c5.get_message(),   test::messaging_context::base_message);
    kwk::transform_reduce(c5, v, v2);
    TTS_EQUAL(c5.get_message(),   std::string{"map used!"});
  }

  // inner_product overloads
  {
    test::c_inner_product1 c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::inner_product(c1, v, v2);
    TTS_EQUAL(c1.get_message(),   std::string{"inner_product(ctx, in1, in2) used!"});

    test::c_inner_product2 c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    inner_product(c2, v, v2);
    TTS_EQUAL(c2.get_message(),   std::string{"inner_product(ctx, in1, in2, init) used!"});

    test::c_inner_product3 c3;
    TTS_EQUAL(c3.get_message(),   test::messaging_context::base_message);
    kwk::inner_product(c3, v, v2);
    TTS_EQUAL(c3.get_message(),   std::string{"inner_product(ctx, in1, in2, init, f1, f2) used!"});

    test::context_for_each c4;
    TTS_EQUAL(c4.get_message(),   test::messaging_context::base_message);
    kwk::inner_product(c4, v, v2);
    TTS_EQUAL(c4.get_message(),   std::string{"map used!"});
  }

  // transform_exclusive_scan -> for_each
  {
    test::c_transform_exclusive_scan c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::transform_exclusive_scan(c1, v, v2, 0, [](auto r, auto d){ return (r+d); },
                                  [](auto i1){ return (i1); });
    TTS_EQUAL(c1.get_message(),   std::string{"transform_exclusive_scan(ctx, in, out, init, f1, f2) used!"});

    test::context_for_each c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::transform_exclusive_scan(c2, v, v2, 0, [](auto r, auto d){ return (r+d); },
                                  [](auto i1){ return (i1); });
    TTS_EQUAL(c2.get_message(),   std::string{"map used!"});
  }

  // exclusive_scan overloads -> for_each
  {
    test::c_exclusive_scan1 c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::exclusive_scan(c1, v, v2, 1);
    // kwk::exclusive_scan(c1, v, v2, 1, [](auto r, auto d){ return (r*d); });
    TTS_EQUAL(c1.get_message(),   std::string{"exclusive_scan(ctx, in, out, init) used!"});

    test::c_exclusive_scan2 c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::exclusive_scan(c2, v, v2, 1);
    TTS_EQUAL(c2.get_message(),   std::string{"exclusive_scan(ctx, in, out, init, f) used!"});

    test::context_for_each c3;
    TTS_EQUAL(c3.get_message(),   test::messaging_context::base_message);
    kwk::exclusive_scan(c3, v, v2, 1);
    TTS_EQUAL(c3.get_message(),   std::string{"map used!"});
  }

  // transform_inclusive_scan -> for_each
  {
    test::c_transform_inclusive_scan c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::transform_inclusive_scan(c1, v, v2, 0, [](auto r, auto d){ return (r+d); }, [](auto i1){ return (i1); });
    TTS_EQUAL(c1.get_message(),   std::string{"transform_inclusive_scan(ctx, in, out, init, f1, f2) used!"});

    test::context_for_each c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::transform_inclusive_scan(c2, v, v2, 0, [](auto r, auto d){ return (r+d); }, [](auto i1){ return (i1); });
    TTS_EQUAL(c2.get_message(),   std::string{"map used!"});
  }

  // inclusive_scan(ctx, in, out, init)
  //   -> inclusive_scan(ctx, in, out, init, f)
  //   -> transform_inclusive_scan
  //   -> for_each
  {
    test::c_inclusive_scan1 c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::inclusive_scan(c1, v, v2, 1);
    TTS_EQUAL(c1.get_message(),   std::string{"inclusive_scan(ctx, in, out, init) used!"});

    test::c_inclusive_scan2 c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::inclusive_scan(c2, v, v2, 1);
    TTS_EQUAL(c2.get_message(),   std::string{"inclusive_scan(ctx, in, out, init, f) used!"});

    test::c_transform_inclusive_scan c3;
    TTS_EQUAL(c3.get_message(),   test::messaging_context::base_message);
    kwk::inclusive_scan(c3, v, v2, 1);
    TTS_EQUAL(c3.get_message(),   std::string{"transform_inclusive_scan(ctx, in, out, init, f1, f2) used!"});

    test::context_for_each c4;
    TTS_EQUAL(c4.get_message(),   test::messaging_context::base_message);
    kwk::inclusive_scan(c4, v, v2, 1);
    TTS_EQUAL(c4.get_message(),   std::string{"map used!"});
  }
};





namespace test
{
  struct c_replace    : public context_for_each {};
  struct c_replace_if : public context_for_each {};
}
namespace kwk
{

  template<concepts::container Out>
  constexpr auto replace(test::c_replace& ctx, Out& out, auto old_value, auto new_value)
  {
    ctx.set_message("replace(ctx, out, old_value, new_value) used!");
    return replace(cpu, out, old_value, new_value);
  }

  template<typename Func, concepts::container Out>
  constexpr auto replace_if(test::c_replace_if& ctx, Out& out, Func f, auto new_value)
  {
    ctx.set_message("replace_if(ctx, out, f, new_value) used!");
    return replace_if(cpu, out, f, new_value);
  }
};

TTS_CASE("Check for context overload - replace: replace, replace_if")
{
  auto v   = test::make_view_1();

  {
    test::c_replace c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::replace(c1, v, 2, 3);
    TTS_EQUAL(c1.get_message(),   std::string{"replace(ctx, out, old_value, new_value) used!"});

    test::context_for_each c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::replace(c2, v, 2, 3);
    TTS_EQUAL(c2.get_message(),   std::string{"map used!"});
  }

  {
    test::c_replace_if c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::replace_if(c1, v, [&](auto e) { return (e>=0); }, 0);
    TTS_EQUAL(c1.get_message(),   std::string{"replace_if(ctx, out, f, new_value) used!"});

    test::context_for_each c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::replace_if(c2, v, [&](auto e) { return (e>=0); }, 0);
    TTS_EQUAL(c2.get_message(),   std::string{"map used!"});
  }
};


// TODO: Vérifier qu'on passe bien par les bonnes surcharges à chaque fois

