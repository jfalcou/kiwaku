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

// auto out1 = kumi::tuple{411, 1, 411-77, 23};
// auto out2 = kumi::tuple{291, 1, 291-77, 23};
// auto out3 = kumi::tuple{179, 1, 179-77, 23};
// auto out4 = kumi::tuple{373, 1, 373-77, 23};
// auto out5 = kumi::tuple{597, 1, 597-77, 23};
// auto out6 = kumi::tuple{611, 1, 611-77, 23};
// auto out7 = kumi::tuple{739, 1, 739-77, 23};
// auto out8 = kumi::tuple{809, 1, 809-77, 23};

void test_slice(auto const& in, auto const& out, auto const& sls)
{
  kumi::for_each_index([&](auto n, auto i)
  {
    auto sh = of_size(i);
    kumi::for_each_index([&](auto m, auto s)
    {
      auto result = kumi::apply([&](auto... m)
                    {
                      return sh(m...);
                    }, s);

      auto expected = of_size(get<m>(get<n>(out)));
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