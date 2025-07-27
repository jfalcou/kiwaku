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
#include <kwk/utility/position.hpp>
#include <kwk/context/sycl/internal/sycl_tools.hpp>
#include <cmath>
#include <functional>



namespace kwk::sycl
{
  struct context : private ::sycl::queue, public kwk::base_context<context>
  {
  private:
    bool PRINT_SYCL_HEADER = true;

  public:
    using parent = ::sycl::queue;
    using parent::parent; // forwards every constructor of queue to context.

    static auto in   (kwk::concepts::container auto const& c) { return kwk::sycl::in{c};    }
    static auto out  (kwk::concepts::container auto& c)       { return kwk::sycl::out{c};   }
    static auto inout(kwk::concepts::container auto& c)       { return kwk::sycl::inout{c}; }

    void print_sycl_header(bool force_print = false)
    {
      if (PRINT_SYCL_HEADER || force_print)
      {
        std::cout << "\n\n============ SYCL RUNTIME ============" << std::endl;
        auto device = parent::get_device();
        auto deviceName = device.get_info<::sycl::info::device::name>();
        std::cout << "   Device Name:    " << deviceName << std::endl;
        auto platformName =  device.get_platform().get_info<::sycl::info::platform::name>();
        std::cout << "   Platform Name:  " << platformName << "\n\n\n";
      }
    }

    context()
    {
      print_sycl_header(true);
    }

    // ::kwk::sycl::in
    // auto access(auto const& proxy)
    // {
    //   // return proxy.access(current_handler);
    // }

    // template<typename T>
    // auto access(::kwk::sycl::base_proxy<T>& proxy) { return proxy.access(current_handler); }

    // template<typename T>
    // auto access(::kwk::sycl::in<T>& proxy) { return proxy.access(current_handler); }

    // template<typename T>
    // auto access(::kwk::sycl::out<T>& proxy) { return proxy.access(current_handler); }

    // template<typename T>
    // auto access(::kwk::sycl::inout<T>& proxy) { return proxy.access(current_handler); }


    template<typename Func>
    void map(Func f, concepts::sycl::proxy auto&& p0, concepts::sycl::proxy auto&&... ps)
    {

      parent::submit([&](::sycl::handler &h) 
      {
        // Maps each sycl proxy to an accessor
        auto accs = kumi::map([&](auto& b) { return b.access(h); }, kumi::tuple{p0, ps...});

        // Function passed to SYCL parallel_for
        auto kernel = [=](auto i)
        {
          // Maps each accessor m to m[i]
          auto kapply = [=](auto&&... m)
          {
            f(KWK_FWD(m)[i]...);
          };

          kumi::apply(kapply, accs);
        };

        // For each element of the input tables, call our lambda parameter with the input accessors
        h.parallel_for(p0.size(), kernel);

        // Same code, but more compact
        // h.parallel_for(p0.size(), [=](auto i) { kumi::apply([=](auto&&... m) { f(KWK_FWD(m)[i]...); }, accs); });

        // nd_range makes my GPU crash... :'( (2023-11-10)
        // h.parallel_for(::sycl::nd_range<1>(p0.size(), 1024), [=](::sycl::item<1> i) { kumi::apply([=](auto&&... m) { f(KWK_FWD(m)[i]...); }, accs); });
      });

      parent::wait();
    }

    template<typename Func>
    void map_ext(Func f, concepts::sycl::proxy auto&& p0, concepts::sycl::proxy auto&&... ps)
    {

      parent::submit([&](::sycl::handler &h) 
      {
        // Maps each sycl proxy to an accessor
        auto accs = kumi::map([&](auto& b) { return b.access(h); }, kumi::tuple{p0, ps...});

        auto kernel = [=](auto i)
        {
          auto kapply = [=](auto&&... m)
          {
            f(i, KWK_FWD(m)...);
          };

          kumi::apply(kapply, accs);
        };

        // For each element of the input tables, call our lambda parameter with the input accessors
        h.parallel_for(p0.size(), kernel);

      });

      parent::wait();
      current_handler = nullptr;
      prepare_kernel = nullptr;
    }

    // template<typename Func>
    // void map2(Func f, concepts::sycl::proxy auto&& p0, concepts::sycl::proxy auto&& p1)
    // {
    //   parent::submit([&](::sycl::handler &h) 
    //   {
    //     auto acc0 = p0.access(h);
    //     auto acc1 = p1.access(h);

    //     auto kernel = [=](auto i)
    //     {
    //       f(acc0[i], acc1[i]);

    //       auto kapply = [=](auto&&... m)
    //       {
    //         f(KWK_FWD(m)[i]...);
    //       }

    //       kumi::apply(kapply, accs);
    //     }

    //     // For each element of the input tables, call our lambda parameter with the input accessors
    //     h.parallel_for(p0.size(), kernel);
    //     // nd_range makes my GPU crash... :'( (2023-11-10)
    //     // h.parallel_for(::sycl::nd_range<1>(p0.size(), 1024), [=](::sycl::item<1> i) { kumi::apply([=](auto&&... m) { f(KWK_FWD(m)[i]...); }, accs); });
    //   });
    //   parent::wait();
    // }


    template<typename Func>
    void map_index(Func f, concepts::sycl::proxy auto&& p0)
    {
      parent::submit([&](::sycl::handler &h) 
      {
        auto acc = p0.access(h);
        h.parallel_for(p0.size(), [=](auto i) { f(KWK_FWD(acc)[i], i.get_id(1)); });
      });
      parent::wait();
    }

    // Generic map that will work regardless of data transfer directionality
    // but not optimized when all initial or final transfers are not required.
    template<typename Func, concepts::container C0, concepts::container... Cs>
    void map(Func f, C0&& c0, Cs&&... cs)
    {
      map(f, inout(c0), inout(cs)...);
    }

    template<typename Func, concepts::container C0, concepts::container... Cs>
    constexpr auto for_each(Func f, C0&& c0, Cs&&... cs) // kwk::sycl::context& ctx, 
    {
      std::cout << "!!!!!!!!!!!!!!!!!!! for_each SYCL used !\n";
      // ctx.
      map([&](auto... is) { return f(KWK_FWD(c0)(is...), KWK_FWD(cs)(is...)...); }, c0.shape() );
      // return f;
    }



    // template<typename _Tp>
    // struct custom_plus : public ::std::binary_function<_Tp, _Tp, _Tp>
    // {
    //   /// Returns the sum
    //   _GLIBCXX14_CONSTEXPR
    //   _Tp
    //   operator()(const _Tp& __x, const _Tp& __y) const
    //   { return __x + __y; }
    // };

    // template<typename _Tp, typename Func>
    // struct custom_plus : public binary_function<_Tp, _Tp, _Tp>
    // {
    //   /// Returns the sum
    //   _GLIBCXX14_CONSTEXPR
    //   _Tp
    //   operator()(const _Tp& __x, const _Tp& __y) const
    //   { return f(__x, __y); }
    // private:
    //   Func f;
    // };

    template<typename Func, concepts::container In>
    auto reduce_internal_v2(In const& in, [[maybe_unused]] Func func, [[maybe_unused]] auto init)
    {
      using data_type = typename In::value_type;
      
      data_type result = init;
      {
        std::size_t const numel = in.numel();
        auto in_proxy = this->in(in);
        ::sycl::buffer<data_type> result_buf(&result, ::sycl::range<1>(1));

        parent::submit([&](::sycl::handler& h)
        {
          ::sycl::accessor data_acc = in_proxy.access(h);
          auto reduction_obj = ::sycl::reduction(result_buf, h, func);

          h.parallel_for( ::sycl::range<1>(numel)
                        , reduction_obj
                        , [=](::sycl::id<1> i, auto& sum)
                          {
                            sum.combine(data_acc[i]);
                          }
                        );
        });
        parent::wait();
      }

      return result;
    }




    template<typename Func, concepts::container In>
    auto reduce([[maybe_unused]] In const& in, [[maybe_unused]] Func f, [[maybe_unused]] auto init)
    {
      // std::cout << "kwk::sycl::context::reduce\n";
      // ::sycl::test_sycl();
      try
      {
        return reduce_internal_v2(in, f, init);
      }
      catch (::sycl::exception const &e)
      {
        std::cout << "An exception is caught for SYCL reduce.\n";
        std::terminate();
      }
    }


    template<typename Func_R, typename Func_T, concepts::container In1, concepts::container In2>
    constexpr auto transform_reduce(In1 const& in1, In2 const& in2, auto init, Func_R R, Func_T T)
    {
      // transform_reduce
      // init = R(init, T(i1, i2));

      // reduce
      // init = R(init, i);

      try
      {
        // using data_type = typename In::value_type;
        
        auto result = init;
        {
          std::size_t const numel = in1.numel();
          auto in1_proxy = this->in(in1);
          auto in2_proxy = this->in(in2);
          ::sycl::buffer<decltype(init)> result_buf(&result, ::sycl::range<1>(1));

          parent::submit([&](::sycl::handler& h)
          {
            ::sycl::accessor data1_acc = in1_proxy.access(h);
            ::sycl::accessor data2_acc = in2_proxy.access(h);

            auto reduction_obj = ::sycl::reduction(result_buf, h, R);

            h.parallel_for( ::sycl::range<1>(numel)
                          , reduction_obj
                          , [=](::sycl::id<1> i, auto& sum)
                            {
                              sum.combine(T(data1_acc[i], data2_acc[i]));
                            }
                          );
          });
          parent::wait();
        }

        return result;
      }
      catch (::sycl::exception const &e)
      {
        std::cout << "An exception is caught for SYCL transform_reduce.\n";
        std::terminate();
      }

      // return error for in1.shape() != in2.shape()
      // ctx.map([&](auto const& i1, auto const& i2) { init = R(init, T(i1, i2)); }, ctx.in(in1), ctx.in(in2));
      // return init;
    }

    template<concepts::container In, typename Func>
    std::optional<kwk::position<In::static_order>>
    find_if_v2(In const& in, Func func, [[maybe_unused]] bool ascending)
    {
      std::size_t const numel = in.numel();
      if (numel == 0) return std::nullopt;

      int result = -1;
      try
      {
        // TODO: will only work if values contained in the "in" container are large enough to accept 
        // Reduce function : take the smallest index

        auto rfunc_ascending = [](int a, int b) 
        {
          if (a == -1) return b;
          if (b == -1) return a;
          if (a > b) return b;
          else       return a;
        };

        auto rfunc_descending = [](int a, int b)
        {
          if (a < b) return b;
          else       return a;
        };

        {

          ::sycl::buffer<decltype(result)> result_buf(&result, ::sycl::range<1>(1));

          bool* filter = ::sycl::malloc_device<bool>(numel, *(this));

          auto in_proxy = this->in(in);
          // in_proxy.discard_final_data(); // Will not copy data back to the host

          parent::submit([&](::sycl::handler& h)
          {
            ::sycl::accessor data_acc = in_proxy.access(h);

            // First, test all values
            h.parallel_for( ::sycl::range<1>(numel)
                          , [=](::sycl::id<1> i)
                            {
                              if (func(data_acc[i[0]])) filter[i[0]] = true;
                              else                      filter[i[0]] = false;
                            }
                          );
          });
          parent::wait();

          // std::cout << "Filter: ";
          // for (std::size_t i = 0; i < numel; ++i)
          // {
          //   std::cout << "" << filter[i] << " ";
          // }
          // std::cout << "\n";

          if (ascending)
          {
            parent::submit([&](::sycl::handler& h)
            {
              auto reduction_obj = ::sycl::reduction(result_buf, h, rfunc_ascending);

              // Then reduce the elements
              h.parallel_for( ::sycl::range<1>(numel)
                            , reduction_obj
                            , [=](::sycl::id<1> i, auto& sum)
                              {
                                sum.combine(filter[i[0]] ? i[0] : -1);
                              }
                            );
            });
          }
          else
          {
            parent::submit([&](::sycl::handler& h)
            {
              auto reduction_obj = ::sycl::reduction(result_buf, h, rfunc_descending);

              // Then reduce the elements
              h.parallel_for( ::sycl::range<1>(numel)
                            , reduction_obj
                            , [=](::sycl::id<1> i, auto& sum)
                              {
                                sum.combine(filter[i[0]] ? i[0] : -1);
                              }
                            );
            });
          }
          parent::wait();
          ::sycl::free(filter, *this);
        }
      }
      catch (::sycl::exception const &e)
      {
        std::cout << "An exception is caught for SYCL transform_reduce.\n";
        std::terminate();
      }

      if (result != -1)
      {
        // kwk::coordinates(it, out.shape());
        return std::optional<kwk::position<In::static_order>>{kwk::utils::tools::linear_to_pos(result, in)};
      } else {
        return std::nullopt;
      }
    }

    std::string get_device_name()
    {
      return parent::get_device().get_info<::sycl::info::device::name>();
    }

    bool is_gpu()
    {
      return parent::get_device().is_gpu();
    }
  };

  // Checks whether GPU offloading is available in this system
  // Don't forget the compilation flag "-fsycl -fsycl-targets=nvptx64-nvidia-cuda,x86_64" x86_64 or spir64
  // To target both Nvidia GPUs and native CPUs.
  [[maybe_unused]] static bool has_gpu()
  {
    auto devices = ::sycl::device::get_devices();
    for (const auto& dev : devices)
    {
      if (dev.is_gpu())
      {
        // std::cout << "\n*** SYCL - found GPU: " 
        //           << dev.get_info<::sycl::info::device::name>() << " ***\n";
        return true;
      }
    }
    // std::cout << "\n*** SYCL - no GPU device available. ***\n\n";
    return false;
  }

  inline kwk::sycl::context default_context{};  
  // inline kwk::sycl::context gpu{::sycl::gpu_selector_v};
  // inline kwk::sycl::context cpu{::sycl::cpu_selector_v};
}

namespace kwk
{

  template<typename Func, concepts::container C0, concepts::container... Cs>
  constexpr auto for_each(kwk::sycl::context& ctx, Func f, C0&& c0, Cs&&... cs)
  {
    // std::cout << "---------------------------------- for_each SYCL used !\n";
    return ctx.map(f, KWK_FWD(c0), KWK_FWD(cs)...);
  }

  // template<typename Func, concepts::container C0, concepts::container... Cs>
  // constexpr auto for_each_ext(kwk::sycl::context& ctx, Func f, C0&& c0, Cs&&... cs)
  // {
  //   // std::cout << "---------------------------------- for_each SYCL used !\n";
  //   return ctx.map_ext(f, KWK_FWD(c0), KWK_FWD(cs)...);
  // }

  // TODO reduce: soit on fait ctx.reduce, soit dans le contexte sycl on réécrit la fonction reduce avec sycl_context.
  template<typename Func, concepts::container In>
  constexpr auto reduce(kwk::sycl::context& ctx, In const& in, Func f, auto init)
  {
    // std::cout << "---------------------------------- reduce SYCL used !\n";
    // std::cout << "sycl reduce\n";
    return ctx.reduce(in, f, init);
  }

  template<typename Func_R, typename Func_T, concepts::container In1, concepts::container In2>
  constexpr auto transform_reduce(kwk::sycl::context& ctx, In1 const& in1, In2 const& in2, auto init, Func_R R, Func_T T)
  {
    // std::cout << "---------------------------------- transform_reduce SYCL used !\n";
    // return error for in1.shape() != in2.shape()
    return ctx.transform_reduce(in1, in2, init, R, T);
  }

  template<concepts::container Container, typename Check>
  std::optional<kwk::position<Container::static_order>>
  find_if(kwk::sycl::context& ctx, Container const& c, Check f)
  {
    auto res = ctx.find_if_v2(c, f, true);
    return res;
  }

  template <typename Func, concepts::container Out>
  constexpr std::optional<kwk::position<Out::static_order>>
  find_last_if(kwk::sycl::context& ctx, Out const& out, Func f)
  {
    return ctx.find_if_v2(out, f, false);
  }

}