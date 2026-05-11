//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#include "test.hpp"
#include <kwk/kwk.hpp>

using kwk::_;

TTS_CASE("Shape behavior - Mixed 1D")
{
  using namespace kwk::literals;
  using kwk::shape;

  shape<_> shape_d;
  shape<7> shape_s;
  shape<_> shape_di = {27};
  shape<_> shape_d7 = {7};
  auto shape_si = shape{7_c}; 

  TTS_EQUAL(sizeof(shape_d), 1 * sizeof(kwk::config::default_size_type));
  TTS_EQUAL(sizeof(shape_s), 1UL);
  TTS_EQUAL(sizeof(shape_di), 1 * sizeof(kwk::config::default_size_type));
  TTS_EQUAL(sizeof(shape_si), 1UL);

  TTS_EQUAL(shape_d.ndim, 1UL);
  TTS_EQUAL(shape_s.ndim, 1UL);
  TTS_EQUAL(shape_di.ndim, 1UL);
  TTS_EQUAL(shape_si.ndim, 1UL);

  TTS_NOT_EQUAL(shape_d, shape_s);
  TTS_NOT_EQUAL(shape_d, shape_di);
  TTS_EQUAL(shape_d7, shape_s);
  TTS_EQUAL(shape_si, shape_s);

  TTS_EQUAL(shape_d.size(), 0L);
  TTS_EQUAL(shape_s.size(), 7L);
  TTS_EQUAL(shape_di.size(), 27L);
  TTS_EQUAL(shape_s.size(), shape_d7.size());
  TTS_EQUAL(shape_si.size(), 7L);
};

/*TTS_CASE("Shape behavior - Mixed 2D"){TTS_WHEN("Build shapes"){kwk::shape<_, _> shape_dd;
kwk::shape<_, 7> shape_ds;
kwk::shape<7, _> shape_sd;
kwk::shape<7, 5> shape_ss;

TTS_AND_THEN("Attributes of default constructed shapes")
{
  TTS_EQUAL(sizeof(shape_dd), 2 * sizeof(kwk::config::default_size_type));
  TTS_EQUAL(sizeof(shape_ds), 1 * sizeof(kwk::config::default_size_type));
  TTS_EQUAL(sizeof(shape_sd), 1 * sizeof(kwk::config::default_size_type));
  TTS_EQUAL(sizeof(shape_ss), 1UL);

  TTS_EQUAL(shape_dd.ndim, 2UL);
  TTS_EQUAL(shape_ds.ndim, 2UL);
  TTS_EQUAL(shape_sd.ndim, 2UL);
  TTS_EQUAL(shape_ss.ndim, 2UL);
}

TTS_AND_THEN("Attributes of filled shapes")
{
  shape_dd = {25, 10};
  shape_ds = {67, _};
  shape_sd = {_, 42};

  TTS_EQUAL(shape_dd.size(), 250L);
  TTS_EQUAL(shape_ds.size(), 469L);
  TTS_EQUAL(shape_sd.size(), 294L);
  TTS_EQUAL(shape_ss.size(), 35L);
}

TTS_AND_THEN("Tuple-like properties")
{
  shape_dd = {25, 10};
  shape_ds = {67, _};
  shape_sd = {_, 42};

  TTS_EQUAL(get<0>(shape_dd), 25);
  TTS_EQUAL(get<1>(shape_dd), 10);

  TTS_EQUAL(get<0>(shape_ds), 67);
  TTS_EQUAL(get<1>(shape_ds), 7);

  TTS_EQUAL(get<0>(shape_sd), 7);
  TTS_EQUAL(get<1>(shape_sd), 42);

  TTS_EQUAL(get<0>(shape_ss), 7);
  TTS_EQUAL(get<1>(shape_ss), 5);
}
}
}
;*/

TTS_CASE("Default constructed shape behavior - Mixed 3D")
{
  kwk::shape<_, _, _> shape_ddd;
  kwk::shape<_, _, 7> shape_dds;
  kwk::shape<_, 7, _> shape_dsd;
  kwk::shape<_, 7, 5> shape_dss;
  kwk::shape<7, _, _> shape_sdd;
  kwk::shape<7, _, 5> shape_sds;
  kwk::shape<7, 5, _> shape_ssd;
  kwk::shape<7, 5, 3> shape_sss;

  TTS_EQUAL(sizeof(shape_ddd), 3 * sizeof(kwk::config::default_size_type));
  TTS_EQUAL(sizeof(shape_dds), 2 * sizeof(kwk::config::default_size_type));
  TTS_EQUAL(sizeof(shape_dsd), 2 * sizeof(kwk::config::default_size_type));
  TTS_EQUAL(sizeof(shape_dss), 1 * sizeof(kwk::config::default_size_type));
  TTS_EQUAL(sizeof(shape_sdd), 2 * sizeof(kwk::config::default_size_type));
  TTS_EQUAL(sizeof(shape_sds), 1 * sizeof(kwk::config::default_size_type));
  TTS_EQUAL(sizeof(shape_ssd), 1 * sizeof(kwk::config::default_size_type));
  TTS_EQUAL(sizeof(shape_sss), 1UL);

  TTS_EQUAL(shape_ddd.ndim, 3UL);
  TTS_EQUAL(shape_dds.ndim, 3UL);
  TTS_EQUAL(shape_dsd.ndim, 3UL);
  TTS_EQUAL(shape_dss.ndim, 3UL);
  TTS_EQUAL(shape_sdd.ndim, 3UL);
  TTS_EQUAL(shape_sds.ndim, 3UL);
  TTS_EQUAL(shape_ssd.ndim, 3UL);
  TTS_EQUAL(shape_sss.ndim, 3UL);
};

TTS_CASE("Default constructed shape behavior - Mixed 4D")
{
  kwk::shape<_, _, _, _> shape_dddd;
  kwk::shape<_, _, _, 7> shape_ddds;
  kwk::shape<_, _, 7, _> shape_ddsd;
  kwk::shape<_, _, 7, 5> shape_ddss;
  kwk::shape<_, 7, _, _> shape_dsdd;
  kwk::shape<_, 7, _, 5> shape_dsds;
  kwk::shape<_, 7, 5, _> shape_dssd;
  kwk::shape<_, 7, 5, 3> shape_dsss;
  kwk::shape<9, _, _, _> shape_sddd;
  kwk::shape<9, _, _, 7> shape_sdds;
  kwk::shape<9, _, 7, _> shape_sdsd;
  kwk::shape<9, _, 7, 5> shape_sdss;
  kwk::shape<9, 7, _, _> shape_ssdd;
  kwk::shape<9, 7, _, 5> shape_ssds;
  kwk::shape<9, 7, 5, _> shape_sssd;
  kwk::shape<9, 7, 5, 3> shape_ssss;

  TTS_EQUAL(sizeof(shape_dddd), 4 * sizeof(kwk::config::default_size_type));
  TTS_EQUAL(sizeof(shape_ddds), 3 * sizeof(kwk::config::default_size_type));
  TTS_EQUAL(sizeof(shape_ddsd), 3 * sizeof(kwk::config::default_size_type));
  TTS_EQUAL(sizeof(shape_ddss), 2 * sizeof(kwk::config::default_size_type));
  TTS_EQUAL(sizeof(shape_dsdd), 3 * sizeof(kwk::config::default_size_type));
  TTS_EQUAL(sizeof(shape_dsds), 2 * sizeof(kwk::config::default_size_type));
  TTS_EQUAL(sizeof(shape_dssd), 2 * sizeof(kwk::config::default_size_type));
  TTS_EQUAL(sizeof(shape_dsss), 1 * sizeof(kwk::config::default_size_type));
  TTS_EQUAL(sizeof(shape_sddd), 3 * sizeof(kwk::config::default_size_type));
  TTS_EQUAL(sizeof(shape_sdds), 2 * sizeof(kwk::config::default_size_type));
  TTS_EQUAL(sizeof(shape_sdsd), 2 * sizeof(kwk::config::default_size_type));
  TTS_EQUAL(sizeof(shape_sdss), 1 * sizeof(kwk::config::default_size_type));
  TTS_EQUAL(sizeof(shape_ssdd), 2 * sizeof(kwk::config::default_size_type));
  TTS_EQUAL(sizeof(shape_ssds), 1 * sizeof(kwk::config::default_size_type));
  TTS_EQUAL(sizeof(shape_sssd), 1 * sizeof(kwk::config::default_size_type));
  TTS_EQUAL(sizeof(shape_ssss), 1UL);

  TTS_EQUAL(shape_dddd.ndim, 4UL);
  TTS_EQUAL(shape_ddds.ndim, 4UL);
  TTS_EQUAL(shape_ddsd.ndim, 4UL);
  TTS_EQUAL(shape_ddss.ndim, 4UL);
  TTS_EQUAL(shape_dsdd.ndim, 4UL);
  TTS_EQUAL(shape_dsds.ndim, 4UL);
  TTS_EQUAL(shape_dssd.ndim, 4UL);
  TTS_EQUAL(shape_dsss.ndim, 4UL);
  TTS_EQUAL(shape_sddd.ndim, 4UL);
  TTS_EQUAL(shape_sdds.ndim, 4UL);
  TTS_EQUAL(shape_sdsd.ndim, 4UL);
  TTS_EQUAL(shape_sdss.ndim, 4UL);
  TTS_EQUAL(shape_ssdd.ndim, 4UL);
  TTS_EQUAL(shape_ssds.ndim, 4UL);
  TTS_EQUAL(shape_sssd.ndim, 4UL);
  TTS_EQUAL(shape_ssss.ndim, 4UL);
};

template<int N, typename Indexes = std::make_index_sequence<N>> struct sizes;

template<int N, std::size_t... I> struct sizes<N, std::index_sequence<I...>>
{
  using types_list = tts::types<std::integral_constant<std::size_t, I + 1>...>;
};

TTS_CASE_TPL("Building a nD shape with kwk::kwk::shape{a1,...,an}", sizes<10>)<typename T>(::tts::type<T>)
{
  auto f = []<std::size_t... Idx>(std::index_sequence<Idx...> const&) {
    return kwk::shape{3 * (1 + Idx)...};
  };

  auto sh = f(std::make_index_sequence<T::value>{});
  TTS_EQUAL(sh.ndim, T::value);
};
