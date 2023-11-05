//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kwk/concepts/container.hpp>
#include <kwk/concepts/sycl_proxy.hpp>
#include <kwk/context/sycl/proxy.hpp>
#include <kwk/context/base.hpp>

namespace kwk::sycl
{
  struct context : private ::sycl::queue, public kwk::base_context<context>
  {
    using parent = ::sycl::queue;
    using parent::parent; // forwards every constructor of queue to context.

    static auto in   (kwk::concepts::container auto const& c) { return kwk::sycl::in{c};    }
    static auto out  (kwk::concepts::container auto& c)       { return kwk::sycl::out{c};   }
    static auto inout(kwk::concepts::container auto& c)       { return kwk::sycl::inout{c}; }

    template<typename Func>
    void map(Func f, concepts::sycl::proxy auto&& p0, concepts::sycl::proxy auto&&... ps)
    {
      std::cout 
      << "Running on device: "
      << parent::get_device().get_info<::sycl::info::device::name>()
      << "\n";

      parent::submit([&](::sycl::handler &h) 
      {
        // Maps each sycl proxy to an accessor
        auto accs = kumi::map([&](auto& b) { return b.access(h); }, kumi::tuple{p0, ps...});

        // For each element of the input tables, call our lambda parameter with the input accessors
        h.parallel_for(p0.size(), [=](auto i) { kumi::apply([=](auto&&... m) { f(KWK_FWD(m)[i]...); }, accs); });
      });

      parent::wait();
    }

  // inline kwk::sycl::context default_context{::sycl::gpu_selector_v};
  inline kwk::sycl::context default_context{};
}