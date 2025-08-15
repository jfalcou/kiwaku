//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kwk/concepts/container.hpp>
#include <kwk/context/base.hpp>
#include <kwk/detail/algorithm/for_each.hpp>
#include <kwk/detail/algorithm/for_until.hpp>
#include <kwk/detail/abi.hpp>
#include <cstddef>
#include <utility>

namespace kwk::sycl
{
  // base proxy
  template<typename T>
  class base_proxy
  {
    public:
    base_proxy(kwk::concepts::container auto const& c): data(c.get_data(), ::sycl::range<1>(c.numel())) {}
    
    auto size() const { return data.size(); }

    auto& get_buffer() { return data; }

    // WIP
    /* If we don't want to copy data back and trash it. buffer::set_final_data() tells the
     * SYCL runtime where to put the data when the buffer is destroyed; nullptr
     * indicates not to copy back. */
    // void discard_final_data()
    // {
    //   data.set_final_data(nullptr);
    // }
    // DOSEN'T SEEM TO WORK :(

    protected:
    ::sycl::buffer<T> data;
  };

  // in proxy
  template<typename T>
  class in : public base_proxy<T>
  {
    public:
    auto access(::sycl::handler& h)
    {
      return ::sycl::accessor(base_proxy<T>::data, h, ::sycl::read_only);
    }
  };

  template<kwk::concepts::container T>
  in(T const&) -> in<typename T::value_type>;

  // out proxy
  template<typename T>
  class out : public base_proxy<T>
  {
    public:
    auto access(::sycl::handler& h)
    {
      return ::sycl::accessor(base_proxy<T>::data, h, ::sycl::write_only, ::sycl::no_init);
    }
  };

  template<kwk::concepts::container T>
  out(T const&) -> out<typename T::value_type>;

  // inout proxy
  template<typename T>
  class inout : public base_proxy<T>
  {
    public:
    auto access(::sycl::handler& h)
    {
      return ::sycl::accessor(base_proxy<T>::data, h, ::sycl::read_write);
    }
  };

  template<kwk::concepts::container T>
  inout(T const&) -> inout<typename T::value_type>;
}