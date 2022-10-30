//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/utility.hpp>
#include <kwk/concepts.hpp>

struct box
{
  box() noexcept : alloc(), data_(), size_() {}

  template<kwk::concepts::allocator A>
  box ( std::ptrdiff_t n, A a)
      : alloc(std::move(a))
      , data_( reinterpret_cast<float*>(allocate(alloc, n*sizeof(float))) )
      , size_(n)
  {}

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

  float *       data()       noexcept { return data_; }
  float const*  data() const noexcept { return data_; }

  float *       begin()       noexcept { return data(); }
  float const*  begin() const noexcept { return data(); }

  float *       end()       noexcept { return begin() + size_; }
  float const*  end() const noexcept { return begin() + size_; }

  ~box() { if(size_ != 0) deallocate( alloc, data_ ); }

  void swap(box& b) noexcept
  {
    alloc.swap(b.alloc);
    std::swap(data_,b.data_);
    std::swap(size_,b.size_);
  }

  kwk::any_allocator  alloc;
  float*              data_;
  std::ptrdiff_t      size_;
};

TTS_CASE( "Checks allocator is suitable for pseudo-container support" )
{
  box b( 5, kwk::heap );
  box c( 7, kwk::heap );

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
};
