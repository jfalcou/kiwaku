//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/detail/kumi.hpp>
#include <kwk/utility/container/shape.hpp>

using namespace kwk::literals;
using namespace kwk;

/// List of inputs
auto in1  = kumi::tuple{411, 291_c};
auto in2  = kumi::tuple{179, 373_c};
auto in3  = kumi::tuple{597, 611_c};
auto in4  = kumi::tuple{739, 809_c};

// List of slicers
int f = 77, t = 23;
auto sl   = kumi::tuple{_, 13, from(f), to(t)};

// List of expected outputs
template<std::size_t I> auto out(auto const& tp)
{
  auto v = get<I>(tp);
  return kumi::tuple{v, 1, v - f, t};
}

void test_slice(auto const& in, auto const& out, auto const& sls)
{
  kumi::for_each_index([&]<typename N>(N , auto i)
  {
    auto sh = of_size(i);
    kumi::for_each_index([&]<typename M>(M , auto s)
    {
      auto result = kumi::apply([&](auto... m)
                    {
                      return sh(m...);
                    }, s);

      auto expected = of_size(kumi::get<M::value>(kumi::get<N::value>(out)));
      TTS_EQUAL(result, expected);
    }, sls);
  }, in);
}

template<std::size_t I>
auto compress_out(auto const& in)
{
  auto o1 = kumi::map([](auto p) { return compress<I>(of_size(p)); }, in);

  auto outs = kumi::map ([](auto p)
                          {
                            return [&]<std::size_t... N>(std::index_sequence<N...>)
                            {
                              return kumi::cartesian_product( out<N>(p)...);
                            }(std::make_index_sequence<I>{});

                          }, o1
                        );
  return outs;
}
