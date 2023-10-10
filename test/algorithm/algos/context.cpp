//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <cstdlib>
#include <kwk/algorithm/algos/copy.hpp>
#include <kwk/algorithm/algos/predicates.hpp>
#include <kwk/container.hpp>
#include <kwk/concepts.hpp>
#include "test.hpp"
#include <string>
#include <iostream>
#include <memory>

namespace test
{
  struct messaging_context
  {
  public:
    static inline const std::string base_message{"Uninitialized string."};
    // messaging_context(std::string& msg) : message(&msg) {}
    messaging_context() { message = std::make_unique<std::string>(messaging_context::base_message); }
    std::string get_message() const { return *message; }
    std::string set_message(auto const& msg) const { *message = std::string{msg}; return *message; }
  protected:
    std::unique_ptr<std::string> message;
  };

  struct context_for_each : kwk::base_context<context_for_each>, messaging_context
  {
    using messaging_context::messaging_context;
    template<typename Func, auto... S>
    constexpr auto for_each(Func, kwk::shape<S...> const&) const { return set_message("for_each used!"); }

    template<typename Func, kwk::concepts::container C0, kwk::concepts::container... Cs>
    constexpr auto for_each(Func, C0&&, Cs&&...) { return set_message("for_each used!"); } // return f;
  };

  struct context_count : kwk::base_context<context_count>, messaging_context
  {
    using messaging_context::messaging_context;
    template<kwk::concepts::container In>
    constexpr auto count(In const&, auto) const { return set_message("count used!"); }
  };

  struct context_reduce : kwk::base_context<context_reduce>, messaging_context
  {
    using messaging_context::messaging_context;
    template<typename Func, kwk::concepts::container In>
    constexpr auto reduce(In const&, Func, auto) const
    {
      set_message("reduce used!");
      return static_cast<int>(84);
    }
  };

  struct context_any_of : kwk::base_context<context_any_of>, messaging_context
  {
    using messaging_context::messaging_context;
    template<typename Func, kwk::concepts::container In>
    constexpr auto any_of(In const&, Func) const { set_message("any_of used!"); return false; }
  };

  struct context_none_of : kwk::base_context<context_none_of>, messaging_context
  {
    using messaging_context::messaging_context;
    template<typename Func, kwk::concepts::container In>
    constexpr auto none_of(In const&, Func) const { set_message("none_of used!"); return false; }
  };

  struct context_count_if : kwk::base_context<context_count_if>, messaging_context
  {
    using messaging_context::messaging_context;
    template<typename Func, kwk::concepts::container In>
    constexpr auto count_if(In const&, Func) const { set_message("count_if used!"); return 0; }
  };

  struct context_copy : kwk::base_context<context_copy>, messaging_context
  {
    using messaging_context::messaging_context;
    template<kwk::concepts::container Out, kwk::concepts::container In>
    constexpr auto copy(Out&, In&&) const { set_message("copy used!"); return 0; }
  };

  struct context_copy_if : kwk::base_context<context_copy_if>, messaging_context
  {
    using messaging_context::messaging_context;
    template<typename Func, kwk::concepts::container Out, kwk::concepts::container In>
    constexpr auto copy_if(Func, Out&, In&&) const { set_message("copy_if used!"); return 0; }
  };

  struct context_transform : kwk::base_context<context_transform>, messaging_context
  {
    using messaging_context::messaging_context;
    template< typename Func, kwk::concepts::container Out
            , kwk::concepts::container C0, kwk::concepts::container... Cs>
    constexpr auto transform(Func, Out&, C0&&, Cs&&...) const { set_message("transform used!"); return 0; }
  };
}

// TODO: continuer à utiliser les chaines de caractères
// tester copy, reduce et vérifier que les dépendances avec (copy et reduce) y passent bien
// for_each est nécessaire pour tous les algos
// vérifier que toutes les variantes des algos ont bien un équivalent avec les contextes, et tester ces variantes dans
// les tests, et bien vérifier qu'on passe bien par les bonnes versions surchargées

TTS_CASE("Check for function overload")
{

  const std::size_t d0 = 2;
  const std::size_t d1 = 3;
  int data[d0 * d1];
  for (std::size_t i = 0; i < d0 * d1; ++i) { data[i] = 0; }
  data[1] = 6;
  data[3] = 6;
  int data2[d0 * d1];

  auto v  = kwk::view{kwk::source = data, kwk::of_size(d0, d1)};
  auto v2 = kwk::view{kwk::source = data2, kwk::of_size(d0, d1)};

  // FOR_EACH (for_each)
  {
    test::context_for_each c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    c1.for_each( [](auto e) {}, v);
    TTS_EQUAL(c1.get_message(),   std::string{"for_each used!"});
  }

  // TRANSFORM (transform -> for_each)
  {
    test::context_transform c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    c1.transform( [](auto e) { return 1.0/(1.0+e); }, v, v2);
    TTS_EQUAL(c1.get_message(),   std::string{"transform used!"});

    test::context_for_each c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    c2.transform( [](auto e) { return 1.0/(1.0+e); }, v, v2);
    TTS_EQUAL(c2.get_message(),   std::string{"for_each used!"});
  }

  // COUNT (count -> reduce -> for_each)
  {
    test::context_count c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    TTS_EQUAL(c1.count(v, 6)  ,   std::string{"count used!"});
    TTS_EQUAL(c1.get_message(),   std::string{"count used!"});

    test::context_reduce c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    TTS_EQUAL(c2.count(v, 6)  ,   static_cast<int>(84));
    TTS_EQUAL(c2.get_message(),   std::string{"reduce used!"});

    test::context_for_each c3;
    TTS_EQUAL(c3.get_message(),   test::messaging_context::base_message);
    c3.count(v, 6);
    TTS_EQUAL(c3.get_message(),   std::string{"for_each used!"});
  }


  // REDUCE (reduce -> for_each)
  {
    test::context_reduce c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    c1.reduce(v, 6, std::size_t{});
    TTS_EQUAL(c1.get_message(),   std::string{"reduce used!"});

    test::context_for_each c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    c2.reduce(v, 6, std::size_t{});
    TTS_EQUAL(c2.get_message(),   std::string{"for_each used!"});
  }


  // ANY_OF (any_of -> reduce -> for_each)
  {
    test::context_any_of c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    c1.any_of(v, 6);
    TTS_EQUAL(c1.get_message(),   std::string{"any_of used!"});

    test::context_reduce c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    c2.any_of(v, 6);
    TTS_EQUAL(c2.get_message(),   std::string{"reduce used!"});

    test::context_for_each c3;
    TTS_EQUAL(c3.get_message(),   test::messaging_context::base_message);
    c3.any_of(v, 6);
    TTS_EQUAL(c3.get_message(),   std::string{"for_each used!"});
  }


  // NONE_OF (none_of -> !any_of -> reduce -> for_each)
  {
    test::context_none_of c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    c1.none_of(v, 6);
    TTS_EQUAL(c1.get_message(),   std::string{"none_of used!"});

    test::context_any_of c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    c2.none_of(v, 6);
    TTS_EQUAL(c2.get_message(),   std::string{"any_of used!"});
    
    test::context_reduce c3;
    TTS_EQUAL(c3.get_message(),   test::messaging_context::base_message);
    c3.none_of(v, 6);
    TTS_EQUAL(c3.get_message(),   std::string{"reduce used!"});

    test::context_for_each c4;
    TTS_EQUAL(c4.get_message(),   test::messaging_context::base_message);
    c4.none_of(v, 6);
    TTS_EQUAL(c4.get_message(),   std::string{"for_each used!"});
  }

  // COUNT_IF (count_if -> reduce -> for_each)
  {
    test::context_count_if c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    c1.count_if(v, v);
    TTS_EQUAL(c1.get_message(),   std::string{"count_if used!"});

    test::context_reduce c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    c2.count_if(v, v);
    TTS_EQUAL(c2.get_message(),   std::string{"reduce used!"});

    test::context_for_each c3;
    TTS_EQUAL(c3.get_message(),   test::messaging_context::base_message);
    c3.count_if(v, v);
    TTS_EQUAL(c3.get_message(),   std::string{"for_each used!"});
  }

  // COPY (copy -> transform -> for_each)
  {
    test::context_copy c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    c1.copy(v, v2);
    TTS_EQUAL(c1.get_message(),   std::string{"copy used!"});

    test::context_transform c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    c2.copy(v, v2);
    TTS_EQUAL(c2.get_message(),   std::string{"transform used!"});

    test::context_for_each c3;
    TTS_EQUAL(c3.get_message(),   test::messaging_context::base_message);
    c3.copy(v, v2);
    TTS_EQUAL(c3.get_message(),   std::string{"for_each used!"});
  }

  // COPY_IF (copy_if -> for_each)
  {
    test::context_copy_if c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    c1.copy_if([](auto) {return true;}, v, v2);
    TTS_EQUAL(c1.get_message(),   std::string{"copy_if used!"});

    test::context_for_each c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    c2.copy_if([](auto) {return true;}, v, v2);
    TTS_EQUAL(c2.get_message(),   std::string{"for_each used!"});
  }



};

