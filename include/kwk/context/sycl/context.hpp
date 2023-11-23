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
#include <kwk/context/cpu/context.hpp>

namespace kwk::sycl
{
  struct context : private ::sycl::queue, public kwk::base_context<context>
  {
  private:
    std::size_t workitem_count = 1000;

  public:
    using parent = ::sycl::queue;
    using parent::parent; // forwards every constructor of queue to context.

    static auto in   (kwk::concepts::container auto const& c) { return kwk::sycl::in{c};    }
    static auto out  (kwk::concepts::container auto& c)       { return kwk::sycl::out{c};   }
    static auto inout(kwk::concepts::container auto& c)       { return kwk::sycl::inout{c}; }

    void set_workitem_count(std::size_t workitem_count_) { workitem_count = workitem_count_; }

    template<typename Func>
    void map(Func f, concepts::sycl::proxy auto&& p0, concepts::sycl::proxy auto&&... ps)
    {
      // std::cout 
      // << "Running on device: "
      // << parent::get_device().get_info<::sycl::info::device::name>()
      // << "\n";

      parent::submit([&](::sycl::handler &h) 
      {
        // Maps each sycl proxy to an accessor
        auto accs = kumi::map([&](auto& b) { return b.access(h); }, kumi::tuple{p0, ps...});

        // For each element of the input tables, call our lambda parameter with the input accessors
        h.parallel_for(p0.size(), [=](auto i) { kumi::apply([=](auto&&... m) { f(KWK_FWD(m)[i]...); }, accs); });
        // nd_range makes my GPU crash... :'( (2023-11-10)
        // h.parallel_for(::sycl::nd_range<1>(p0.size(), 1024), [=](::sycl::item<1> i) { kumi::apply([=](auto&&... m) { f(KWK_FWD(m)[i]...); }, accs); });
      });

      parent::wait();
    }

    // template<typename T, typename Func>
    // void for_all(std::vector<T>& data, Func f)
    // {
    //   sycl::queue q;
    //   sycl::buffer<T> in(data);

    //   q.submit([&](sycl::handler &h)
    //   {
    //     sycl::accessor  pr{in, h};
    //     auto            sz = data.size();
    //     h.parallel_for(sycl::nd_range<1>(64,8), [=](sycl::item<1> i)
    //     {
    //       if (i < sz) f(pr[i]);
    //     });
    //   });
    // }

    template<typename Func, concepts::container In>
    auto reduce_internal(In const& in, [[maybe_unused]] Func f, [[maybe_unused]] auto init)
    {
      std::cout 
      << "Running on device: "
      << parent::get_device().get_info<::sycl::info::device::name>()
      << "   workitem_count(" << workitem_count << ")"
      << "\n";
      // std::cout << "kwk::sycl::context::reduce_internal\n";

      using data_type = typename In::value_type;

      std::size_t numel = in.numel();
      // std::cout << "in.numel = " << numel << "\n";

      std::size_t witems = workitem_count;
      if (witems > numel) witems = numel; // avoid having more workitems than elements to process
      ::sycl::range<1> num_items{witems};

      // Simple copy from in_vec to out_vec
      std::vector<data_type> out_vec;
      out_vec.resize(numel);

      std::fill(out_vec.begin(), out_vec.end(), 0);

      // Cas simple où num_out.size = num_in.size

      ::sycl::buffer out_buf(out_vec);
      auto in_proxy = this->in(in);

      parent::submit([&](::sycl::handler &h) {
        ::sycl::accessor in_acc = in_proxy.access(h);
        // ::sycl::accessor in_acc(in_buf, h, ::sycl::read_only);
        ::sycl::accessor out_acc(out_buf, h, ::sycl::write_only, ::sycl::no_init);
        h.parallel_for(num_items, [=](auto index) {
          std::size_t gid = index[0];

          // out_acc[i] = in_acc[i];

          //  in case (numel < num_items)
          if (gid < numel)
          {
            data_type ini = in_acc[gid];

            for (std::size_t i = gid + witems; i < numel; i += witems)
            {
              // sum += acc_in[i];
              // ini = f(ini, acc_in[i]);
              ini = f(ini, in_acc[i]);
            }
            out_acc[gid] = ini;
          }
        });
      });
      parent::wait();

      data_type res = init;

      ::sycl::host_accessor h_acc(out_buf);
      for (std::size_t i = 0; i < witems; ++i)
      {
        res = f(res, h_acc[i]);
      }

      return res;
    }


    template<typename Func, concepts::container In>
    auto reduce([[maybe_unused]] In const& in, [[maybe_unused]] Func f, [[maybe_unused]] auto init)
    {
      // std::cout << "kwk::sycl::context::reduce\n";
      // ::sycl::test_sycl();
      try
      {
        return reduce_internal(in, f, init);
      }
      catch (::sycl::exception const &e)
      {
        std::cout << "An exception is caught for vector add.\n";
        std::terminate();
      }
    }

    // TODO: regarder la doc de SYCL pour savoir comment c'est censé fonctionner les réductions en SYCL (y a des variables de réduction)
    // Regarder les exemples (spec Kronos à préférer au code de Intel)
  };

  // inline kwk::sycl::context default_context{::sycl::gpu_selector_v};
  inline kwk::sycl::context default_context{};
}

namespace kwk
{
  // TODO reduce: soit on fait ctx.reduce, soit dans le contexte sycl on réécrit la fonction reduce avec sycl_context.
  template<typename Func, concepts::container In>
  constexpr auto reduce(kwk::sycl::context& ctx, In const& in, Func f, auto init)
  {
    // std::cout << "sycl reduce\n";
    return ctx.reduce(in, f, init);
  }
}