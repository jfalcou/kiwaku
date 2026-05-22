//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/container.hpp>
#include <algorithm>
#include <array>
#include <vector>

using kwk::_, kwk::end;
using kwk::at, kwk::between, kwk::every, kwk::slice;
using kwk::from, kwk::to, kwk::by;

TTS_CASE( "Slice a 1D table")
{
  float ref[] = {1,2,3,4,5,6,7,8,9,10,11,12};
  auto v = kwk::table{ kwk::source = ref };

  auto v_all      = v(_);
  auto v_at3      = v(at(3));
  auto v_end      = v(end - 1);
  auto v_between  = v(between(3,6));
  auto v_slice    = v(slice(to = 5, from = 2));
  auto v_slicen   = v(slice(by = 2, from = 2, to = 5));
  auto v_every    = v(every(2));

  TTS_CONSTEXPR_EXPECT    (v_all.preserve_reachability    );
  TTS_CONSTEXPR_EXPECT    (v_at3.preserve_reachability    );
  TTS_CONSTEXPR_EXPECT    (v_end.preserve_reachability    );
  TTS_CONSTEXPR_EXPECT    (v_between.preserve_reachability);
  TTS_CONSTEXPR_EXPECT    (v_slice.preserve_reachability  );
  TTS_CONSTEXPR_EXPECT_NOT(v_slicen.preserve_reachability );
  TTS_CONSTEXPR_EXPECT_NOT(v_every.preserve_reachability  );

  TTS_EQUAL(v_at3(0), v(3));
  TTS_EQUAL(v_end(0), v(11));
  for(int i = 0; i < v.size();++i)          TTS_EQUAL(v_all(i), v(i));
  for(int i = 0; i < v_between.size();++i)  TTS_EQUAL(v_between(i), v(i+3));
  for(int i = 0; i < v_slice.size();++i)    TTS_EQUAL(v_slice(i), v(i+2));
  for(int i = 0; i < v_slicen.size();++i)   TTS_EQUAL(v_slicen(i), v(i*2+2));
  for(int i = 0; i < v_every.size();  ++i)  TTS_EQUAL(v_every(i), v(i*2));
};
