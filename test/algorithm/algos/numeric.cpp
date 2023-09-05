//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <cstdlib>
#include <kwk/algorithm/algos/numeric.hpp>
#include <kwk/container.hpp>
#include "test.hpp"


TTS_CASE("Check for kwk::transform_reduce(In1, In2, Reduce, Transform, init) 1D")
{
  int data1[2];
  int data2[2];

  fill_data(data1, kwk::of_size(2), true);
  fill_data(data2, kwk::of_size(2), true);

  auto d1 = kwk::view{kwk::source = data1, kwk::of_size(2)};
  auto d2 = kwk::view{kwk::source = data2, kwk::of_size(2)};

  auto res = kwk::transform_reduce(d1, d2, 10,
            [](auto r, auto d){ return (r+d); },
            [](auto i1, auto i2){ return (i1*i2); }
            );

  TTS_EQUAL(res, 11);
};

TTS_CASE("Check for kwk::transform_reduce(In1, In2, Transform) 2D")
{
  int data1[2*3];
  int data2[2*3];

  fill_data(data1, kwk::of_size(2,3), true);
  fill_data(data2, kwk::of_size(2,3), true);

  auto d1 = kwk::view{kwk::source = data1, kwk::of_size(2,3)};
  auto d2 = kwk::view{kwk::source = data2, kwk::of_size(2,3)};

  auto res = kwk::transform_reduce(d1, d2, 0,
            [](auto i1, auto i2){ return (i1*i2); }
            );

  TTS_EQUAL(res, 370);
};

TTS_CASE("Check for kwk::transform_reduce(In1, In2, Reduce, Transform, init) 3D")
{
  int data1[2*3*4];
  int data2[2*3*4];

  fill_data(data1, kwk::of_size(2,3,4), true);
  fill_data(data2, kwk::of_size(2,3,4), true);

  auto d1 = kwk::view{kwk::source = data1, kwk::of_size(2,3,4)};
  auto d2 = kwk::view{kwk::source = data2, kwk::of_size(2,3,4)};

  auto res = kwk::transform_reduce(d1, d2, 0, 
            [](auto r, auto d){ return (r+d); },
            [](auto i1, auto i2){ return (i1*i2); }
            );

  TTS_EQUAL(res, 152404);
};

TTS_CASE("Check for kwk::transform_reduce(In1, In2, Reduce, Transform, init) 4D")
{
  int data1[2*3*4*5];
  int data2[2*3*4*5];

  fill_data(data1, kwk::of_size(2,3,4,5), true);
  fill_data(data2, kwk::of_size(2,3,4,5), true);

  auto d1 = kwk::view{kwk::source = data1, kwk::of_size(2,3,4,5)};
  auto d2 = kwk::view{kwk::source = data2, kwk::of_size(2,3,4,5)};

  auto res = kwk::transform_reduce(d1, d2, 0, 
            [](auto r, auto d){ return (r+d); },
            [](auto i1, auto i2){ return (i1*i2); }
            );

  TTS_EQUAL(res, 76497920);
};

TTS_CASE("Check for kwk::inner_product(In1, In2, value, sum, product) 1D")
{
  int data1[2];
  int data2[2];

  fill_data(data1, kwk::of_size(2), true);
  fill_data(data2, kwk::of_size(2), true);

  auto d1 = kwk::view{kwk::source = data1, kwk::of_size(2)};
  auto d2 = kwk::view{kwk::source = data2, kwk::of_size(2)};

  auto res = kwk::inner_product(d1, d2, 0,
            [](auto r, auto d){ return (r+d); },
            [](auto i1, auto i2){ return (i1==i2); }
            );

  TTS_EQUAL(res, 2);
};


TTS_CASE("Check for kwk::inner_product(In1, In2, value, sum, product) 2D")
{
  int data1[2*3];
  int data2[2*3];

  fill_data(data1, kwk::of_size(2,3), true);
  fill_data(data2, kwk::of_size(2,3), true);

  auto d1 = kwk::view{kwk::source = data1, kwk::of_size(2,3)};
  auto d2 = kwk::view{kwk::source = data2, kwk::of_size(2,3)};

  auto res = kwk::inner_product(d1, d2, 0,
            [](auto r, auto d){ return (r+d); },
            [](auto i1, auto i2){ return (i1==i2); }
            );

  TTS_EQUAL(res, 6);
};

TTS_CASE("Check for kwk::inner_product(In1, In2) 3D")
{
  int data1[2*3*4];
  int data2[2*3*4];

  fill_data(data1, kwk::of_size(2,3,4), true);
  fill_data(data2, kwk::of_size(2,3,4), true);

  auto d1 = kwk::view{kwk::source = data1, kwk::of_size(2,3,4)};
  auto d2 = kwk::view{kwk::source = data2, kwk::of_size(2,3,4)};

  auto res = kwk::inner_product(d1, d2);

  TTS_EQUAL(res, 152404);
};

TTS_CASE("Check for kwk::inner_product(In1, In2) 4D")
{
  int data1[2*3*4*5];
  int data2[2*3*4*5];

  fill_data(data1, kwk::of_size(2,3,4,5), true);
  fill_data(data2, kwk::of_size(2,3,4,5), true);

  auto d1 = kwk::view{kwk::source = data1, kwk::of_size(2,3,4,5)};
  auto d2 = kwk::view{kwk::source = data2, kwk::of_size(2,3,4,5)};

  auto res = kwk::inner_product(d1, d2);

  TTS_EQUAL(res, 76497920);
};

TTS_CASE("Check for kwk::transform_exclusive_scan(In, Out, init, f1, f2) 1D")
{
  int data1[2];
  int data2[2];
  int data3[2];

  fill_data(data1, kwk::of_size(2), true);
  fill_data(data2, kwk::of_size(2), true);

  auto d1 = kwk::view{kwk::source = data1, kwk::of_size(2)};
  auto d2 = kwk::view{kwk::source = data2, kwk::of_size(2)};

  kwk::transform_exclusive_scan(d1, d2, 0, [](auto r, auto d){ return (r+d); },
          [](auto i1){ return (i1); });

  std::transform_exclusive_scan(data1, &data1[2],
				data3,
				0, std::plus<int>{}, [](auto i1){ return (i1); });

  TTS_ALL_EQUAL(data2, data3);
};

TTS_CASE("Check for kwk::transform_exclusive_scan(In, Out, init, f1, f2) 2D")
{
  int data1[2*3];
  int data2[2*3];
  int data3[2*3];

  fill_data(data1, kwk::of_size(2,3), true);
  fill_data(data2, kwk::of_size(2,3), true);

  auto d1 = kwk::view{kwk::source = data1, kwk::of_size(2,3)};
  auto d2 = kwk::view{kwk::source = data2, kwk::of_size(2,3)};

  kwk::transform_exclusive_scan(d1, d2, 0, [](auto r, auto d){ return (r+d); },
          [](auto i1){ return (i1); });

  std::transform_exclusive_scan(data1, &data1[2*3],
				data3,
				0, std::plus<int>{}, [](auto i1){ return (i1); });

  TTS_ALL_EQUAL(data2, data3);
};

TTS_CASE("Check for kwk::transform_exclusive_scan(In, Out, init, f1, f2) 3D")
{
  int data1[2*3*4];
  int data2[2*3*4];
  int data3[2*3*4];

  fill_data(data1, kwk::of_size(2,3,4), true);
  fill_data(data2, kwk::of_size(2,3,4), true);

  auto d1 = kwk::view{kwk::source = data1, kwk::of_size(2,3,4)};
  auto d2 = kwk::view{kwk::source = data2, kwk::of_size(2,3,4)};

  kwk::transform_exclusive_scan(d1, d2, 0, [](auto r, auto d){ return (r+d); },
          [](auto i1){ return (i1); });

  std::transform_exclusive_scan(data1, &data1[2*3*4],
				data3,
				0, std::plus<int>{}, [](auto i1){ return (i1); });

  TTS_ALL_EQUAL(data2, data3);
};

TTS_CASE("Check for kwk::transform_exclusive_scan(In, Out, init, f1, f2) 4D")
{
  int data1[2*3*4*5];
  int data2[2*3*4*5];
  int data3[2*3*4*5];

  fill_data(data1, kwk::of_size(2,3,4,5), true);
  fill_data(data2, kwk::of_size(2,3,4,5), true);

  auto d1 = kwk::view{kwk::source = data1, kwk::of_size(2,3,4,5)};
  auto d2 = kwk::view{kwk::source = data2, kwk::of_size(2,3,4,5)};

  kwk::transform_exclusive_scan(d1, d2, 0, [](auto r, auto d){ return (r+d); },
          [](auto i1){ return (i1); });

  std::transform_exclusive_scan(data1, &data1[2*3*4*5],
				data3,
				0, std::plus<int>{}, [](auto i1){ return (i1); });

  TTS_ALL_EQUAL(data2, data3);
};

TTS_CASE("Check for kwk::transform_inclusive_scan(In, Out, init, f1, f2) 1D")
{
  int data1[2];
  int data2[2];
  int data3[2];

  fill_data(data1, kwk::of_size(2), true);
  fill_data(data2, kwk::of_size(2), true);

  auto d1 = kwk::view{kwk::source = data1, kwk::of_size(2)};
  auto d2 = kwk::view{kwk::source = data2, kwk::of_size(2)};

  kwk::transform_inclusive_scan(d1, d2, 0, [](auto r, auto d){ return (r+d); },
          [](auto i1){ return (i1); });

  std::transform_inclusive_scan(data1, &data1[2],
				data3,
				std::plus<int>{}, [](auto i1){ return (i1); });

  TTS_ALL_EQUAL(data2, data3);
};

TTS_CASE("Check for kwk::transform_inclusive_scan(In, Out, init, f1, f2) 2D")
{
  int data1[2*3];
  int data2[2*3];
  int data3[2*3];

  fill_data(data1, kwk::of_size(2,3), true);
  fill_data(data2, kwk::of_size(2,3), true);

  auto d1 = kwk::view{kwk::source = data1, kwk::of_size(2,3)};
  auto d2 = kwk::view{kwk::source = data2, kwk::of_size(2,3)};

  kwk::transform_inclusive_scan(d1, d2, 0, [](auto r, auto d){ return (r+d); },
          [](auto i1){ return (i1); });

  std::transform_inclusive_scan(data1, &data1[2*3],
				data3,
				std::plus<int>{}, [](auto i1){ return (i1); });

  TTS_ALL_EQUAL(data2, data3);
};

TTS_CASE("Check for kwk::transform_inclusive_scan(In, Out, init, f1, f2) 3D")
{
  int data1[2*3*4];
  int data2[2*3*4];
  int data3[2*3*4];

  fill_data(data1, kwk::of_size(2,3,4), true);
  fill_data(data2, kwk::of_size(2,3,4), true);

  auto d1 = kwk::view{kwk::source = data1, kwk::of_size(2,3,4)};
  auto d2 = kwk::view{kwk::source = data2, kwk::of_size(2,3,4)};

  kwk::transform_inclusive_scan(d1, d2, 0, [](auto r, auto d){ return (r+d); },
          [](auto i1){ return (i1); });

  std::transform_inclusive_scan(data1, &data1[2*3*4],
				data3,
				std::plus<int>{}, [](auto i1){ return (i1); });

  TTS_ALL_EQUAL(data2, data3);
};

TTS_CASE("Check for kwk::transform_inclusive_scan(In, Out, init, f1, f2) 4D")
{
  int data1[2*3*4*5];
  int data2[2*3*4*5];
  int data3[2*3*4*5];

  fill_data(data1, kwk::of_size(2,3,4,5), true);
  fill_data(data2, kwk::of_size(2,3,4,5), true);

  auto d1 = kwk::view{kwk::source = data1, kwk::of_size(2,3,4,5)};
  auto d2 = kwk::view{kwk::source = data2, kwk::of_size(2,3,4,5)};

  kwk::transform_inclusive_scan(d1, d2, 0, [](auto r, auto d){ return (r+d); },
          [](auto i1){ return (i1); });

  std::transform_inclusive_scan(data1, &data1[2*3*4*5],
				data3,
				std::plus<int>{}, [](auto i1){ return (i1); });

  TTS_ALL_EQUAL(data2, data3);
};

TTS_CASE("Check for kwk::exclusive_scan(In, Out, init, func) 1D")
{
  int data1[2];
  int data2[2];
  int data3[2];

  fill_data(data1, kwk::of_size(2), true);
  data1[0] = 1;

  auto d1 = kwk::view{kwk::source = data1, kwk::of_size(2)};
  auto d2 = kwk::view{kwk::source = data2, kwk::of_size(2)};

  kwk::exclusive_scan(d1, d2, 1, [](auto r, auto d){ return (r*d); });

  std::exclusive_scan(data1, &data1[2],
                      data3, 1,
                      std::multiplies<int>{});

  TTS_ALL_EQUAL(data2, data3);
};

TTS_CASE("Check for kwk::exclusive_scan(In, Out, init, func) 2D")
{
  int data1[2*3];
  int data2[2*3];
  int data3[2*3];

  fill_data(data1, kwk::of_size(2,3), true);
  data1[0] = 1;

  auto d1 = kwk::view{kwk::source = data1, kwk::of_size(2,3)};
  auto d2 = kwk::view{kwk::source = data2, kwk::of_size(2,3)};

  kwk::exclusive_scan(d1, d2, 1, [](auto r, auto d){ return (r*d); });

  std::exclusive_scan(data1, &data1[2*3],
                      data3, 1,
                      std::multiplies<int>{});

  TTS_ALL_EQUAL(data2, data3);
};

TTS_CASE("Check for kwk::exclusive_scan(In, Out, init, func) 3D")
{
  int data1[2*3*4];
  int data2[2*3*4];
  int data3[2*3*4];

  fill_data(data1, kwk::of_size(2,3,4), true);
  data1[0] = 1;

  auto d1 = kwk::view{kwk::source = data1, kwk::of_size(2,3,4)};
  auto d2 = kwk::view{kwk::source = data2, kwk::of_size(2,3,4)};

  kwk::exclusive_scan(d1, d2, 1, [](auto r, auto d){ return (r*d); });

  std::exclusive_scan(data1, &data1[2*3*4],
                      data3, 1,
                      std::multiplies<int>{});

  TTS_ALL_EQUAL(data2, data3);
};

TTS_CASE("Check for kwk::exclusive_scan(In, Out, init, func) 4D")
{
  int data1[2*3*4*5];
  int data2[2*3*4*5];
  int data3[2*3*4*5];

  fill_data(data1, kwk::of_size(2,3,4,5), true);
  data1[0] = 1;

  auto d1 = kwk::view{kwk::source = data1, kwk::of_size(2,3,4,5)};
  auto d2 = kwk::view{kwk::source = data2, kwk::of_size(2,3,4,5)};

  kwk::exclusive_scan(d1, d2, 1, [](auto r, auto d){ return (r*d); });

  std::exclusive_scan(data1, &data1[2*3*4*5],
                      data3, 1,
                      std::multiplies<int>{});

  TTS_ALL_EQUAL(data2, data3);
};

TTS_CASE("Check for kwk::exclusive_scan(In, Out, init) 1D")
{
  int data1[2];
  int data2[2];
  int data3[2];

  fill_data(data1, kwk::of_size(2), true);
  data1[0] = 1;

  auto d1 = kwk::view{kwk::source = data1, kwk::of_size(2)};
  auto d2 = kwk::view{kwk::source = data2, kwk::of_size(2)};

  kwk::exclusive_scan(d1, d2, 0);

  std::exclusive_scan(data1, &data1[2],
                      data3, 0);

  TTS_ALL_EQUAL(data2, data3);
};

TTS_CASE("Check for kwk::exclusive_scan(In, Out, init) 2D")
{
  int data1[2*3];
  int data2[2*3];
  int data3[2*3];

  fill_data(data1, kwk::of_size(2,3), true);
  data1[0] = 1;

  auto d1 = kwk::view{kwk::source = data1, kwk::of_size(2,3)};
  auto d2 = kwk::view{kwk::source = data2, kwk::of_size(2,3)};

  kwk::exclusive_scan(d1, d2, 0);

  std::exclusive_scan(data1, &data1[2*3],
                      data3, 0);

  TTS_ALL_EQUAL(data2, data3);
};

TTS_CASE("Check for kwk::exclusive_scan(In, Out, init) 3D")
{
  int data1[2*3*4];
  int data2[2*3*4];
  int data3[2*3*4];

  fill_data(data1, kwk::of_size(2,3,4), true);
  data1[0] = 1;

  auto d1 = kwk::view{kwk::source = data1, kwk::of_size(2,3,4)};
  auto d2 = kwk::view{kwk::source = data2, kwk::of_size(2,3,4)};

  kwk::exclusive_scan(d1, d2, 0);

  std::exclusive_scan(data1, &data1[2*3*4],
                      data3, 0);

  TTS_ALL_EQUAL(data2, data3);
};

TTS_CASE("Check for kwk::exclusive_scan(In, Out, init) 4D")
{
  int data1[2*3*4*5];
  int data2[2*3*4*5];
  int data3[2*3*4*5];

  fill_data(data1, kwk::of_size(2,3,4,5), true);
  data1[0] = 1;

  auto d1 = kwk::view{kwk::source = data1, kwk::of_size(2,3,4,5)};
  auto d2 = kwk::view{kwk::source = data2, kwk::of_size(2,3,4,5)};

  kwk::exclusive_scan(d1, d2, 0);

  std::exclusive_scan(data1, &data1[2*3*4*5],
                      data3, 0);

  TTS_ALL_EQUAL(data2, data3);
};

TTS_CASE("Check for kwk::inclusive_scan(In, Out, init, func) 1D")
{
  int data1[2];
  int data2[2];
  int data3[2];

  fill_data(data1, kwk::of_size(2), true);
  data1[0] = 1;

  auto d1 = kwk::view{kwk::source = data1, kwk::of_size(2)};
  auto d2 = kwk::view{kwk::source = data2, kwk::of_size(2)};

  kwk::inclusive_scan(d1, d2, 1, [](auto r, auto d){ return (r*d); });

  std::inclusive_scan(data1, &data1[2],
                      data3, 
                      std::multiplies<int>{});

  TTS_ALL_EQUAL(data2, data3);
};

TTS_CASE("Check for kwk::inclusive_scan(In, Out, init, func) 2D")
{
  int data1[2*3];
  int data2[2*3];
  int data3[2*3];

  fill_data(data1, kwk::of_size(2,3), true);
  data1[0] = 1;

  auto d1 = kwk::view{kwk::source = data1, kwk::of_size(2,3)};
  auto d2 = kwk::view{kwk::source = data2, kwk::of_size(2,3)};

  kwk::inclusive_scan(d1, d2, 1, [](auto r, auto d){ return (r*d); });

  std::inclusive_scan(data1, &data1[2*3],
                      data3,
                      std::multiplies<int>{});

  TTS_ALL_EQUAL(data2, data3);
};

TTS_CASE("Check for kwk::inclusive_scan(In, Out, init, func) 3D")
{
  int data1[2*3*4];
  int data2[2*3*4];
  int data3[2*3*4];

  fill_data(data1, kwk::of_size(2,3,4), true);
  data1[0] = 1;

  auto d1 = kwk::view{kwk::source = data1, kwk::of_size(2,3,4)};
  auto d2 = kwk::view{kwk::source = data2, kwk::of_size(2,3,4)};

  kwk::inclusive_scan(d1, d2, 0, [](auto r, auto d){ return (r+d); });

  std::inclusive_scan(data1, &data1[2*3*4],
                      data3,
                      std::plus<int>{});

  TTS_ALL_EQUAL(data2, data3);
};

TTS_CASE("Check for kwk::inclusive_scan(In, Out, init, func) 4D")
{
  int data1[2*3*4*5];
  int data2[2*3*4*5];
  int data3[2*3*4*5];

  fill_data(data1, kwk::of_size(2,3,4,5), true);
  data1[0] = 1;

  auto d1 = kwk::view{kwk::source = data1, kwk::of_size(2,3,4,5)};
  auto d2 = kwk::view{kwk::source = data2, kwk::of_size(2,3,4,5)};

  kwk::inclusive_scan(d1, d2, 0, [](auto r, auto d){ return (r+d); });

  std::inclusive_scan(data1, &data1[2*3*4*5],
                      data3,
                      std::plus<int>{});

  TTS_ALL_EQUAL(data2, data3);
};

TTS_CASE("Check for kwk::inclusive_scan(In, Out, init) 1D")
{
  int data1[2];
  int data2[2];
  int data3[2];

  fill_data(data1, kwk::of_size(2), true);
  data1[0] = 1;

  auto d1 = kwk::view{kwk::source = data1, kwk::of_size(2)};
  auto d2 = kwk::view{kwk::source = data2, kwk::of_size(2)};

  kwk::inclusive_scan(d1, d2, 0);

  std::inclusive_scan(data1, &data1[2],
                      data3);

  TTS_ALL_EQUAL(data2, data3);
};

TTS_CASE("Check for kwk::inclusive_scan(In, Out, init) 2D")
{
  int data1[2*3];
  int data2[2*3];
  int data3[2*3];

  fill_data(data1, kwk::of_size(2,3), true);
  data1[0] = 1;

  auto d1 = kwk::view{kwk::source = data1, kwk::of_size(2,3)};
  auto d2 = kwk::view{kwk::source = data2, kwk::of_size(2,3)};

  kwk::inclusive_scan(d1, d2, 0);

  std::inclusive_scan(data1, &data1[2*3],
                      data3);

  TTS_ALL_EQUAL(data2, data3);
};

TTS_CASE("Check for kwk::inclusive_scan(In, Out, init) 3D")
{
  int data1[2*3*4];
  int data2[2*3*4];
  int data3[2*3*4];

  fill_data(data1, kwk::of_size(2,3,4), true);
  data1[0] = 1;

  auto d1 = kwk::view{kwk::source = data1, kwk::of_size(2,3,4)};
  auto d2 = kwk::view{kwk::source = data2, kwk::of_size(2,3,4)};

  kwk::inclusive_scan(d1, d2, 0);

  std::inclusive_scan(data1, &data1[2*3*4],
                      data3);

  TTS_ALL_EQUAL(data2, data3);
};

TTS_CASE("Check for kwk::inclusive_scan(In, Out, init) 4D")
{
  int data1[2*3*4*5];
  int data2[2*3*4*5];
  int data3[2*3*4*5];

  fill_data(data1, kwk::of_size(2,3,4,5), true);
  data1[0] = 1;

  auto d1 = kwk::view{kwk::source = data1, kwk::of_size(2,3,4,5)};
  auto d2 = kwk::view{kwk::source = data2, kwk::of_size(2,3,4,5)};

  kwk::inclusive_scan(d1, d2, 0);

  std::inclusive_scan(data1, &data1[2*3*4*5],
                      data3);

  TTS_ALL_EQUAL(data2, data3);
};