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
#include <kwk/algorithm/algos/binary_search.hpp>
#include <kwk/container.hpp>
#include <kwk/concepts.hpp>
#include "test.hpp"
#include <string>
#include <iostream>
#include <memory>
#include <kwk/algorithm/algos/find.hpp>

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

  auto make_view_1()
  {
    const std::size_t d0 = 2;
    const std::size_t d1 = 3;
    int data[d0 * d1];
    for (std::size_t i = 0; i < d0 * d1; ++i) { data[i] = 0; }
    data[1] = 6;
    data[3] = 6;
    return kwk::view{kwk::source = data, kwk::of_size(d0, d1)};
  }

  auto make_view_2()
  {
    const std::size_t d0 = 2;
    const std::size_t d1 = 3;
    int data2[d0 * d1];
    return kwk::view{kwk::source = data2, kwk::of_size(d0, d1)};
  }
}


namespace test
{
  struct context_for_each : kwk::base_context<context_for_each>, messaging_context
  {
    using messaging_context::messaging_context;
    template<typename Func, auto... S>
    constexpr auto for_each(Func, kwk::shape<S...> const&) const { return set_message("for_each used!"); }

    template<typename Func, kwk::concepts::container C0, kwk::concepts::container... Cs>
    constexpr auto for_each(Func, C0&&, Cs&&...) { return set_message("for_each used!"); } // return f;
  };
}
TTS_CASE("Check for context overload - for_each")
{
  auto v  = test::make_view_1();

  // FOR_EACH (for_each)
  {
    test::context_for_each c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::for_each(c1, [](auto e) {}, v);
    TTS_EQUAL(c1.get_message(),   std::string{"for_each used!"});
  }
};


namespace test
{
  struct context_transform : kwk::base_context<context_transform>, messaging_context
  {
    using messaging_context::messaging_context;
    template< typename Func, kwk::concepts::container Out
            , kwk::concepts::container C0, kwk::concepts::container... Cs>
    constexpr auto transform(Func, Out&, C0&&, Cs&&...) const { set_message("transform used!"); return 0; }
  };
}
TTS_CASE("Check for context overload - transform")
{
  auto v  = test::make_view_1();
  auto v2 = test::make_view_2();

  // TRANSFORM (transform -> for_each)
  {
    test::context_transform c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::transform(c1, [](auto e) { return 1.0/(1.0+e); }, v, v2);
    TTS_EQUAL(c1.get_message(),   std::string{"transform used!"});

    test::context_for_each c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::transform(c2, [](auto e) { return 1.0/(1.0+e); }, v, v2);
    TTS_EQUAL(c2.get_message(),   std::string{"for_each used!"});
  }
};


namespace test
{
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
}
TTS_CASE("Check for context overload - copy & copy_if")
{

  auto v  = test::make_view_1();
  auto v2 = test::make_view_2();

  // COPY (copy -> transform -> for_each)
  {
    test::context_copy c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::copy(c1, v, v2);
    TTS_EQUAL(c1.get_message(),   std::string{"copy used!"});

    test::context_transform c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::copy(c2, v, v2);
    TTS_EQUAL(c2.get_message(),   std::string{"transform used!"});

    test::context_for_each c3;
    TTS_EQUAL(c3.get_message(),   test::messaging_context::base_message);
    kwk::copy(c3, v, v2);
    TTS_EQUAL(c3.get_message(),   std::string{"for_each used!"});
  }

  // COPY_IF (copy_if -> for_each)
  {
    test::context_copy_if c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::copy_if(c1, [](auto) {return true;}, v, v2);
    TTS_EQUAL(c1.get_message(),   std::string{"copy_if used!"});

    test::context_for_each c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::copy_if(c2, [](auto) {return true;}, v, v2);
    TTS_EQUAL(c2.get_message(),   std::string{"for_each used!"});
  }

};


namespace test
{
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
}
TTS_CASE("Check for context overload - reduce")
{

  auto v  = test::make_view_1();
  // auto v2 = test::make_view_2();

  // REDUCE (reduce -> for_each)
  {
    test::context_reduce c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::reduce(c1, v, 6, std::size_t{});
    TTS_EQUAL(c1.get_message(),   std::string{"reduce used!"});

    test::context_for_each c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::reduce(c2, v, 6, std::size_t{});
    TTS_EQUAL(c2.get_message(),   std::string{"for_each used!"});
  }
};



// all_of any_of, none_of, count, count_if

namespace test
{
  struct context_all_of : kwk::base_context<context_all_of>, messaging_context
  {
    using messaging_context::messaging_context;
    template<typename Func, kwk::concepts::container In>
    constexpr auto all_of(In const&, Func) const { set_message("all_of used!"); return true; }
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

  struct context_count : kwk::base_context<context_count>, messaging_context
  {
    using messaging_context::messaging_context;
    template<kwk::concepts::container In>
    constexpr auto count(In const&, auto) const { return set_message("count used!"); }
  };

  struct context_count_if : kwk::base_context<context_count_if>, messaging_context
  {
    using messaging_context::messaging_context;
    template<typename Func, kwk::concepts::container In>
    constexpr auto count_if(In const&, Func) const { set_message("count_if used!"); return 0; }
  };
}

// TODO: continuer à utiliser les chaines de caractères
// tester copy, reduce et vérifier que les dépendances avec (copy et reduce) y passent bien
// for_each est nécessaire pour tous les algos
// vérifier que toutes les variantes des algos ont bien un équivalent avec les contextes, et tester ces variantes dans
// les tests, et bien vérifier qu'on passe bien par les bonnes versions surchargées



TTS_CASE("Check for context overload")
{

  auto v  = test::make_view_1();
  // auto v2 = test::make_view_2();

  // all_of -> reduce -> for_each
  {
    test::context_all_of c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::all_of(c1, v, [](auto e) {return true;});
    TTS_EQUAL(c1.get_message(),   std::string{"all_of used!"});

    test::context_reduce c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::all_of(c2, v, [](auto e) {return true;});
    TTS_EQUAL(c2.get_message(),   std::string{"reduce used!"});

    test::context_for_each c3;
    TTS_EQUAL(c3.get_message(),   test::messaging_context::base_message);
    kwk::all_of(c3, v, [](auto e) {return true;});
    TTS_EQUAL(c3.get_message(),   std::string{"for_each used!"});
  }

  // COUNT (count -> reduce -> for_each)
  {
    test::context_count c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    TTS_EQUAL(kwk::count(c1, v, 6)  ,   std::string{"count used!"});
    TTS_EQUAL(c1.get_message(),   std::string{"count used!"});

    test::context_reduce c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    TTS_EQUAL(kwk::count(c2, v, 6)  ,   static_cast<int>(84));
    TTS_EQUAL(c2.get_message(),   std::string{"reduce used!"});

    test::context_for_each c3;
    TTS_EQUAL(c3.get_message(),   test::messaging_context::base_message);
    kwk::count(c3, v, 6);
    TTS_EQUAL(c3.get_message(),   std::string{"for_each used!"});
  }


  // REDUCE (reduce -> for_each)
  {
    test::context_reduce c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::reduce(c1, v, 6, std::size_t{});
    TTS_EQUAL(c1.get_message(),   std::string{"reduce used!"});

    test::context_for_each c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::reduce(c2, v, 6, std::size_t{});
    TTS_EQUAL(c2.get_message(),   std::string{"for_each used!"});
  }


  // ANY_OF (any_of -> reduce -> for_each)
  {
    test::context_any_of c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::any_of(c1, v, 6);
    TTS_EQUAL(c1.get_message(),   std::string{"any_of used!"});

    test::context_reduce c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::any_of(c2, v, 6);
    TTS_EQUAL(c2.get_message(),   std::string{"reduce used!"});

    test::context_for_each c3;
    TTS_EQUAL(c3.get_message(),   test::messaging_context::base_message);
    kwk::any_of(c3, v, 6);
    TTS_EQUAL(c3.get_message(),   std::string{"for_each used!"});
  }


  // NONE_OF (none_of -> !any_of -> reduce -> for_each)
  {
    test::context_none_of c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::none_of(c1, v, 6);
    TTS_EQUAL(c1.get_message(),   std::string{"none_of used!"});

    test::context_any_of c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::none_of(c2, v, 6);
    TTS_EQUAL(c2.get_message(),   std::string{"any_of used!"});
    
    test::context_reduce c3;
    TTS_EQUAL(c3.get_message(),   test::messaging_context::base_message);
    kwk::none_of(c3, v, 6);
    TTS_EQUAL(c3.get_message(),   std::string{"reduce used!"});

    test::context_for_each c4;
    TTS_EQUAL(c4.get_message(),   test::messaging_context::base_message);
    kwk::none_of(c4, v, 6);
    TTS_EQUAL(c4.get_message(),   std::string{"for_each used!"});
  }

  // COUNT_IF (count_if -> reduce -> for_each)
  {
    test::context_count_if c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::count_if(c1, v, v);
    TTS_EQUAL(c1.get_message(),   std::string{"count_if used!"});

    test::context_reduce c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::count_if(c2, v, v);
    TTS_EQUAL(c2.get_message(),   std::string{"reduce used!"});

    test::context_for_each c3;
    TTS_EQUAL(c3.get_message(),   test::messaging_context::base_message);
    kwk::count_if(c3, v, v);
    TTS_EQUAL(c3.get_message(),   std::string{"for_each used!"});
  }

};



namespace test
{
  struct context_lower_bound : kwk::base_context<context_lower_bound>, messaging_context
  {
    using messaging_context::messaging_context;
    template <typename Func, kwk::concepts::container Out>
    constexpr auto lower_bound(Out const&, auto, Func) const { set_message("lower_bound used!"); return 0; }
  };

  struct context_upper_bound : kwk::base_context<context_upper_bound>, messaging_context
  {
    using messaging_context::messaging_context;
    template <typename Func, kwk::concepts::container Out>
    constexpr auto upper_bound(Out const&, auto, Func) const { set_message("upper_bound used!"); return 0; }
  };

  struct context_binary_search : kwk::base_context<context_binary_search>, messaging_context
  {
    using messaging_context::messaging_context;
    template <typename Func, kwk::concepts::container Out>
    constexpr auto binary_search(Out const&, auto, Func) const { set_message("binary_search used!"); return 0; }
  };
}

TTS_CASE("Check for context overload - lower/upper_bound & binary_search")
{
  auto v  = test::make_view_1();
  // auto v2 = test::make_view_2();

  // LOWER_BOUND (lower_bound -> kwk::coordinates)
  {
    test::context_lower_bound c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::lower_bound(c1, v, 21, [](auto, auto) {return true;});
    TTS_EQUAL(c1.get_message(),   std::string{"lower_bound used!"});

    test::context_lower_bound c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::lower_bound(c2, v, 21);
    TTS_EQUAL(c2.get_message(),   std::string{"lower_bound used!"});
  }

  // UPPER_BOUND (upper_bound -> kwk::coordinates)
  {
    test::context_upper_bound c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::upper_bound(c1, v, 21, [](auto, auto) {return true;});
    TTS_EQUAL(c1.get_message(),   std::string{"upper_bound used!"});

    test::context_upper_bound c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::upper_bound(c2, v, 21);
    TTS_EQUAL(c2.get_message(),   std::string{"upper_bound used!"});
  }

  // BINARY_SEARCH (binary_search -> lower_bound -> kwk::coordinates)
  {
    test::context_binary_search c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::binary_search(c1, v, 21, [](auto, auto) {return true;});
    TTS_EQUAL(c1.get_message(),   std::string{"binary_search used!"});

    test::context_binary_search c1b;
    TTS_EQUAL(c1b.get_message(),   test::messaging_context::base_message);
    kwk::binary_search(c1b, v, 21);
    TTS_EQUAL(c1b.get_message(),   std::string{"binary_search used!"});

    test::context_binary_search c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::binary_search(c2, v, 21);
    TTS_EQUAL(c2.get_message(),   std::string{"binary_search used!"});

    test::context_lower_bound c3;
    TTS_EQUAL(c3.get_message(),   test::messaging_context::base_message);
    kwk::binary_search(c3, v, 21);
    TTS_EQUAL(c3.get_message(),   std::string{"lower_bound used!"});

    test::context_lower_bound c4;
    TTS_EQUAL(c4.get_message(),   test::messaging_context::base_message);
    kwk::binary_search(c4, v, 21, [](auto, auto) {return false;});
    TTS_EQUAL(c4.get_message(),   std::string{"lower_bound used!"});
  }
};



namespace test
{
  struct context_find_if : kwk::base_context<context_find_if>, messaging_context
  {
    using messaging_context::messaging_context;
    template<typename Container, typename Check>
    auto find_if(Container const&, Check) const { set_message("find_if used!"); return 0; }
  };

  struct context_find : kwk::base_context<context_find>, messaging_context
  {
    using messaging_context::messaging_context;
    template <kwk::concepts::container Out>
    constexpr auto find(Out const&, auto) const { set_message("find used!"); return 0; }
  };

  struct context_find_if_not : kwk::base_context<context_find_if_not>, messaging_context
  {
    using messaging_context::messaging_context;
    template <typename Func, kwk::concepts::container Out>
    constexpr auto find_if_not(Out const&, Func) const { set_message("find_if_not used!"); return 0; }
  };

  struct context_find_first_of : kwk::base_context<context_find_first_of>, messaging_context
  {
    using messaging_context::messaging_context;
    template <kwk::concepts::container Out, kwk::concepts::container Values>
    constexpr auto find_first_of(Out const&, Values const&) const { set_message("find_first_of used!"); return 0; }
  };

  // TODO : quand ça sera possible (include kwk::reverse) tester le reste (...last...)

  // struct context_find_last_if : kwk::base_context<context_find_last_if>, messaging_context
  // {
  //   using messaging_context::messaging_context;
  //   template <typename Func, kwk::concepts::container Out>
  //   constexpr auto find_last_if(Out const&, Func) const { set_message("find_last_if used!"); return 0; }
  // };

  // struct context_find_last : kwk::base_context<context_find_last>, messaging_context
  // {
  //   using messaging_context::messaging_context;
  //   template <kwk::concepts::container Out>
  //   constexpr auto find_last(Out const&, auto) const { set_message("find_last used!"); return 0; }
  // };

  // struct context_find_last_if_not : kwk::base_context<context_find_last_if_not>, messaging_context
  // {
  //   using messaging_context::messaging_context;
  //   template <typename Func, kwk::concepts::container Out>
  //   constexpr auto find_last_if_not(Out const&, Func) const { set_message("find_last_if_not used!"); return 0; }
  // };

  // reste à faire :
  // find_last_if -> find_if
  // find_last -> find_last_if -> find_if
  // find_last_if_not -> find_last_if -> find_if

};

TTS_CASE("Check for context overload - initially from find.hpp")
{
  auto v  = test::make_view_1();
  auto v2 = test::make_view_2();

  // const std::size_t d0 = 2;
  // const std::size_t d1 = 3;
  // int data[d0 * d1];
  // for (std::size_t i = 0; i < d0 * d1; ++i) { data[i] = 0; }
  // data[1] = 6;
  // data[3] = 6;
  // int data2[d0 * d1];

  // auto v  = kwk::view{kwk::source = data, kwk::of_size(d0, d1)};
  // auto v2 = kwk::view{kwk::source = data2, kwk::of_size(d0, d1)};

  // find_if -> kwk::__::for_until
  {
    test::context_find_if c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::find_if(c1, v, [&](auto e){ return true; });
    TTS_EQUAL(c1.get_message(),   std::string{"find_if used!"});

    // kwk::__::for_until
    // test::context_for_until c2;
    // TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    // kwk::find_if(c2, v, [&](auto e){ return true; });
    // TTS_EQUAL(c2.get_message(),   std::string{"for_until used!"});
  }

  // find -> find_if
  {
    test::context_find c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::find(c1, v, 12);
    TTS_EQUAL(c1.get_message(),   std::string{"find used!"});

    test::context_find_if c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::find(c2, v, 12);
    TTS_EQUAL(c2.get_message(),   std::string{"find_if used!"});
  }

  // find_if_not -> find_if
  {
    test::context_find_if_not c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::find_if_not(c1, v, [&](auto e){ return true; });
    TTS_EQUAL(c1.get_message(),   std::string{"find_if_not used!"});

    test::context_find_if c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::find(c2, v, [&](auto e){ return true; });
    TTS_EQUAL(c2.get_message(),   std::string{"find_if used!"});
  }

  // find_first_of -> find_if
  // find_first_of -> any_of -> reduce -> for_each
  {
    test::context_find_first_of c1;
    TTS_EQUAL(c1.get_message(),   test::messaging_context::base_message);
    kwk::find_first_of(c1, v, v2);
    TTS_EQUAL(c1.get_message(),   std::string{"find_first_of used!"});

    test::context_find_if c2;
    TTS_EQUAL(c2.get_message(),   test::messaging_context::base_message);
    kwk::find_first_of(c2, v, v2);
    TTS_EQUAL(c2.get_message(),   std::string{"find_if used!"});

    // vvv find_first_of -> any_of -> reduce -> for_each vvv
    test::context_any_of c3;
    TTS_EQUAL(c3.get_message(),   test::messaging_context::base_message);
    kwk::find_first_of(c3, v, v2);
    TTS_EQUAL(c3.get_message(),   std::string{"any_of used!"});

    test::context_reduce c4;
    TTS_EQUAL(c4.get_message(),   test::messaging_context::base_message);
    kwk::find_first_of(c4, v, v2);
    TTS_EQUAL(c4.get_message(),   std::string{"reduce used!"});

    test::context_for_each c5;
    TTS_EQUAL(c5.get_message(),   test::messaging_context::base_message);
    kwk::find_first_of(c5, v, v2);
    TTS_EQUAL(c5.get_message(),   std::string{"for_each used!"});
  }
};