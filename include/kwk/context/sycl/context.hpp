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

    // Generic map that will work regardless of data transfer directionality
    // but not optimized when all initial or final transfers are not required.
    template<typename Func, concepts::container C0, concepts::container... Cs>
    void map(Func f, C0&& c0, Cs&&... cs)
    {
      map(f, inout(c0), inout(cs)...);

      // int a = 0;
      // auto fn = [&](auto e) { return ++a; };

      // auto containers = kumi::tuple{fn(c0), fn(cs)...};

      // auto proxies = kumi::tuple{inout(c0), inout(cs)...};

      // kumi::for_each_index(
      //                       [](auto i, auto const& m) { std::cout << "Container #" << i << " : " << m << "\n";}
      //                     , containers
      //                     );
    }

    template<typename Func, concepts::container C0, concepts::container... Cs>
    constexpr auto for_each(kwk::sycl::context& ctx, Func&& f, C0&& c0, Cs&&... cs)
    {
      std::cout << "!!!!!!!!!!!!!!!!!!! for_each SYCL used !\n";
      ctx.map([&](auto... is) { return KWK_FWD(f)(KWK_FWD(c0)(is...), KWK_FWD(cs)(is...)...); }, c0.shape() );
      // return f;
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

      // TODO 2024-02-27 : faire avec les reduction variables

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




  //   void VectorAdd(queue &q, const IntVector &a_vector, const IntVector &b_vector, IntVector &sum_parallel) {
  //   range<1> num_items{a_vector.size()};

  //   buffer a_buf(a_vector);
  //   buffer b_buf(b_vector);
  //   buffer sum_buf(sum_parallel.data(), num_items);

  //   for (size_t i = 0; i < num_repetitions; i++ ) {

  //     q.submit([&](handler &h) {
  //       accessor a(a_buf, h, read_only);
  //       accessor b(b_buf, h, read_only);
  //       accessor sum(sum_buf, h, write_only, no_init);
  //       h.parallel_for(num_items, [=](auto i) { sum[i] = a[i] + b[i]; });
  //     });
  //   };
  //   // Wait until compute tasks on GPU done
  //   q.wait();
  // }

    template<concepts::container Out, concepts::container In>
    auto copy_2(Out& out, In const& in)
    {
      std::cout << "sycl copy called\n";

      ::sycl::buffer buf_out{out.get_data(), ::sycl::range<1>(out.numel())};
      ::sycl::buffer buf_in {in.get_data() , ::sycl::range<1>(in.numel())};

      parent::submit([&](::sycl::handler &h) 
      {
        auto access_in  = ::sycl::accessor{buf_in , h, ::sycl::read_only};
        auto access_out = ::sycl::accessor{buf_out, h, ::sycl::write_only};

        std::cout << "sycl copy - before kernel\n";

        // For each element of the input tables, call our lambda parameter with the input accessors
        h.parallel_for(out.numel(), [=](auto i) { access_out[i] = access_in[i]; });

        std::cout << "sycl copy - after kernel\n";
      });
      parent::wait();
    }
    
    template<concepts::container Out, concepts::container In>
    auto copy_1(Out& out, In const& in)
    {
      std::cout << "sycl copy called\n";
      auto proxy_in  = kwk::sycl::context::in(in);
      auto proxy_out = kwk::sycl::context::out(out);

      parent::submit([&](::sycl::handler &h) 
      {
        // Maps each sycl proxy to an accessor
        auto access_in  = proxy_in.access(h);
        auto access_out = proxy_out.access(h);

        // For each element of the input tables, call our lambda parameter with the input accessors
        h.parallel_for(proxy_out.size(), [=](auto i) { access_out[i] = access_in[i]; });
      });
      parent::wait();
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

  template<concepts::container Out, concepts::container In>
  constexpr auto copy(kwk::sycl::context& ctx, Out& out, In const& in)
  {
    ctx.copy_1(out, in);
  }

  // template<typename Func, auto... S>
  // constexpr auto for_each(kwk::sycl::context& ctx, Func&& f, shape<S...> const& shp)
  // {
  //   std::cout << "for_each SYCL used !\n";
  //   //return ctx.map(KWK_FWD(f), shp);
  // }

  template<typename Func, concepts::container C0, concepts::container... Cs>
  constexpr auto for_each(kwk::sycl::context& ctx, Func&& f, C0&& c0, Cs&&... cs)
  {
    // std::cout << "---------------------------------- for_each SYCL used !\n";
    return ctx.map(f, KWK_FWD(c0), KWK_FWD(cs)...);
  }


  // Transform is not a required part of contexts anymore
  template< typename Func, concepts::container Out
  , concepts::container C0, concepts::container... Cs
  >
  constexpr void transform(kwk::sycl::context& ctx, Func f, Out& out, C0 const& c0, Cs const&... cs)
  {
    // TODO: I replaced "KWK_FWD(f)" by just "f" to make the SYCL compiler happy
    auto new_f = [f](auto& o, auto const& i0, auto const&... in) { o = f(i0, in...); };

    ctx.map( new_f // [&](auto& o, auto const& i0, auto const&... in) { o = KWK_FWD(f)(i0, in...); }
           , ctx.out(out)
           , ctx.in(c0)
           , ctx.in(cs)...
           );
  }


}