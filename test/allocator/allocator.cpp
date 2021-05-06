//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include "test.hpp"
#include <kiwaku/allocator/heap_allocator.hpp>
#include <kiwaku/allocator/shallow_allocator.hpp>
#include <kiwaku/allocator/any_allocator.hpp>

struct box
{
  box() noexcept : alloc(), data_(), size_() {}

  template<kwk::concepts::allocator A>
  box( std::ptrdiff_t n, A const& a) : alloc(a), data_(alloc.allocate(n)), size_(n) {}

  box( box const& that ) : box(that.size_, that.alloc)
  {
    std::copy(that.begin(), that.end(), data());
  }

  box( box&& that )  noexcept : box() { swap(that); }

  box& operator=(box const& other)
  {
    box that(other);
    swap(that);
    return *this;
  }

  box& operator=(box && other) noexcept
  {
    box that(std::move(other));
    swap(that);
    return *this;
  }

  float&  get(int i)        noexcept { return data()[i];  }
  float   get(int i) const  noexcept { return data()[i];  }

  float *       data()       noexcept { return static_cast<float*>(data_.data); }
  float const*  data() const noexcept { return static_cast<float const*>(data_.data); }

  float *       begin()       noexcept { return data(); }
  float const*  begin() const noexcept { return data(); }

  float *       end()       noexcept { return begin() + size_; }
  float const*  end() const noexcept { return begin() + size_; }

  ~box() { if(size_ != 0) alloc.deallocate( data_ ); }

  void swap(box& b) noexcept
  {
    alloc.swap(b.alloc);
    data_.swap(b.data_);
    std::swap(size_,b.size_);
  }

  kwk::any_allocator  alloc;
  kwk::block          data_;
  std::ptrdiff_t      size_;
};

TTS_CASE( "Checks allocator is suitable for pseudo-container support" )
{
  std::byte data[32];
  box b( 5, kwk::heap_allocator{} );
  box c( 7, kwk::make_shallow<32,16>(data) );

  for(int i=0;i<5;i++) b.get(i) = 1.f/(1+i);
  for(int i=0;i<7;i++) c.get(i) = 1.5f*(1+i);

  TTS_EQUAL ( b.get(0), 1.f   );
  TTS_EQUAL ( b.get(1), 1.f/2 );
  TTS_EQUAL ( b.get(2), 1.f/3 );
  TTS_EQUAL ( b.get(3), 1.f/4 );
  TTS_EQUAL ( b.get(4), 1.f/5 );

  TTS_EQUAL ( c.get(0), 1.5f    );
  TTS_EQUAL ( c.get(1), 1.5f*2  );
  TTS_EQUAL ( c.get(2), 1.5f*3  );
  TTS_EQUAL ( c.get(3), 1.5f*4  );
  TTS_EQUAL ( c.get(4), 1.5f*5  );
  TTS_EQUAL ( c.get(5), 1.5f*6  );
  TTS_EQUAL ( c.get(6), 1.5f*7  );

  box d = c;

  TTS_EQUAL ( d.get(0), 1.5f    );
  TTS_EQUAL ( d.get(1), 1.5f*2  );
  TTS_EQUAL ( d.get(2), 1.5f*3  );
  TTS_EQUAL ( d.get(3), 1.5f*4  );
  TTS_EQUAL ( d.get(4), 1.5f*5  );
  TTS_EQUAL ( d.get(5), 1.5f*6  );
  TTS_EQUAL ( d.get(6), 1.5f*7  );

  c = b;

  TTS_EQUAL ( c.get(0), 1.f   );
  TTS_EQUAL ( c.get(1), 1.f/2 );
  TTS_EQUAL ( c.get(2), 1.f/3 );
  TTS_EQUAL ( c.get(3), 1.f/4 );
  TTS_EQUAL ( c.get(4), 1.f/5 );

  d = std::move(b);

  TTS_EQUAL ( d.get(0), 1.f   );
  TTS_EQUAL ( d.get(1), 1.f/2 );
  TTS_EQUAL ( d.get(2), 1.f/3 );
  TTS_EQUAL ( d.get(3), 1.f/4 );
  TTS_EQUAL ( d.get(4), 1.f/5 );
}
