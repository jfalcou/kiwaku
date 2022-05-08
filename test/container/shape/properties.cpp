//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include "test.hpp"
#include <kwk/container/shape.hpp>

TTS_CASE( "nbdims behavior on nD shape" )
{
  TTS_EXPR_IS( kwk::of_size<std::uint8_t>(4,2,6,9).nbdims()   , int );
  TTS_EXPR_IS( kwk::of_size<short>(4,2,6,9).nbdims()          , int );
  TTS_EXPR_IS( kwk::of_size(4,2,6,9).nbdims()                 , int );
  TTS_EXPR_IS( kwk::of_size<std::ptrdiff_t>(4,2,6,9).nbdims() , int );

  TTS_EQUAL( (kwk::of_size(4,2,6,9).nbdims()), 4);
  TTS_EQUAL( (kwk::of_size(4,2,6,1).nbdims()), 3);
  TTS_EQUAL( (kwk::of_size(4,2,1,1).nbdims()), 2);
  TTS_EQUAL( (kwk::of_size(4,1,1,1).nbdims()), 1);
  TTS_EQUAL( (kwk::of_size(1,1,1,1).nbdims()), 0);
};

TTS_CASE( "numel behavior on nD shape" )
{
  TTS_EXPR_IS( kwk::of_size<std::uint8_t>(4,2,6,9).numel()  , int           );
  TTS_EXPR_IS( kwk::of_size<short>(4,2,6,9).numel()         , int           );
  TTS_EXPR_IS( kwk::of_size(4,2,6,9).numel()                , int           );
  TTS_EXPR_IS( kwk::of_size<std::uint32_t>(4,2,6,9).numel() , std::uint32_t );
  TTS_EXPR_IS( kwk::of_size<std::ptrdiff_t>(4,2,6,9).numel(), std::ptrdiff_t);
  TTS_EXPR_IS( kwk::of_size<std::size_t>(4,2,6,9).numel()   , std::size_t   );

  TTS_EQUAL( (kwk::of_size(4,2,6,9).numel()), 432 );
  TTS_EQUAL( (kwk::of_size(4,2,6,1).numel()), 48  );
  TTS_EQUAL( (kwk::of_size(4,2,1,1).numel()), 8   );
  TTS_EQUAL( (kwk::of_size(4,1,1,1).numel()), 4   );
  TTS_EQUAL( (kwk::of_size(1,1,1,1).numel()), 1   );
};

struct some_base  { double  y; };
struct other_base { int     x; };

struct ebo_carrier  : kwk::shape<kwk::extent[42][69]>, some_base {};
struct complex_ebo  : kwk::shape<kwk::extent[42][69]>, some_base, other_base {};
struct mixed_ebo    : some_base, kwk::shape<kwk::extent[42][69]>, other_base {};

TTS_CASE( "kwk::shape and Empty Base Optimisation" )
{
  TTS_CONSTEXPR_EQUAL( sizeof(ebo_carrier), sizeof(some_base) );
  TTS_CONSTEXPR_EQUAL( sizeof(complex_ebo), sizeof(some_base)*2 );
  TTS_CONSTEXPR_EQUAL( sizeof(mixed_ebo)  , sizeof(some_base)*2 );
};
