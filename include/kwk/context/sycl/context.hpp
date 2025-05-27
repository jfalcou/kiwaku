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

namespace kwk::utils::tools
{
  template<concepts::container Container>
  std::optional<kwk::position<Container::static_order>>
  linear_to_pos(int index, Container const& container)
  {
    auto shp = container.shape();

    std::vector<int> pos;
    pos.resize(shp.nbdims());
    std::fill(pos.begin(), pos.end(), -1);
  
    if (index != -1)
    {
      // Now convert back linear index to Kiwaku position :
      // dim 0 is the outer dimension, and dim 1 the innermost
      for (int dim = 0; dim < shp.nbdims(); ++dim)
      {
        int divide_by = 1;
        for (int dim2 = dim + 1; dim2 < shp.nbdims(); ++dim2)
        {
          int d = shp[dim2];
          divide_by *= d;
        }
        pos[dim] = index / divide_by;
        index -= pos[dim] * divide_by;
      }
      // index should now be zero
      if (index != 0) std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ERROR: index != 0  (equals " << index << "\n"; 
    }

    // kwk::position<shp::static_order> kwk_pos;
    kwk::position<Container::static_order> kwk_pos;
    for (int dim = 0; dim < shp.nbdims(); ++dim)
    {
      kwk_pos[dim] = pos[dim];
    }
    
    return kwk_pos;
  }
  
  int pos_to_linear(std::vector<int> const& pos, auto const& shp)
  {
    int res = 0;
  
    for (std::size_t dim = 0; dim < shp.nbdims(); ++dim)
    {
      int multiply_by = 1;
      for (std::size_t dim2 = dim + 1; dim2 < shp.nbdims(); ++dim2)
      {
        multiply_by *= shp[dim2];
      }
      res += pos[dim] * multiply_by;
    }
    return res;
  }
}

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


    template<typename Func, concepts::container In>
    auto reduce_internal(In const& in, [[maybe_unused]] Func f, [[maybe_unused]] auto init)
    {
      // std::cout 
      // << "Running on device: "
      // << parent::get_device().get_info<::sycl::info::device::name>()
      // << "   workitem_count(" << workitem_count << ")"
      // << "\n";
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
        std::cout << "An exception is caught for SYCL reduce.\n";
        std::terminate();
      }
    }



    template<concepts::container Container, typename Check>
    std::optional<kwk::position<Container::static_order>>
    find_if(Container const& container, Check f, bool ascending)
    {
      std::size_t numel = container.numel();

      std::vector<int> found_at_index{-1};
      ::sycl::buffer b_found(found_at_index);

      // std::cout << "  CONTEXT SYCL - find_if - numel = " << numel << "\n";

      // Always call in/out/inout outside of the submit lambda.
      auto p_container = in(container);


      parent::submit([&](::sycl::handler &h) 
      {
        auto a_container = p_container.access(h);
        ::sycl::accessor a_found(b_found, h, ::sycl::read_write);
    
        // find first
        if (ascending)
        {
          h.parallel_for(numel, [=](auto index)
          {
            int glob_id       = index[0];
            int current_value = a_container[index];

            if (f(current_value))
            {
              auto atomic_accessor = 
                ::sycl::atomic_ref< int
                                  , ::sycl::memory_order::relaxed
                                  , ::sycl::memory_scope::device
                                  , ::sycl::access::address_space::global_space
                                  >(a_found[0]);

              // TODO: find an atomic way to do the comparison and update
              int val = atomic_accessor.load();
              if ((val == -1) || (val > glob_id))
              {
                atomic_accessor.store(glob_id);
              }
            }
          });
        }
        else // find last
        {
          h.parallel_for(numel, [=](auto index)
          {
            int glob_id       = index[0];
            int current_value = a_container[index];

            if (f(current_value))
            {
              auto atomic_accessor =
                ::sycl::atomic_ref< int
                                  , ::sycl::memory_order::relaxed
                                  , ::sycl::memory_scope::device
                                  , ::sycl::access::address_space::global_space
                                  >(a_found[0]);

              // TODO: find an atomic way to do the comparison and update
              int val = atomic_accessor.load();
              if ((val == -1) || (val < glob_id))
              {
                atomic_accessor.store(glob_id);
              }
            }
          });
        }
      });

      parent::wait();

      ::sycl::host_accessor hostAcc(b_found, ::sycl::read_only);
      int index = hostAcc[0];

      if (index != -1)
      {
        // kwk::coordinates(it, out.shape());
        return std::optional<kwk::position<Container::static_order>>{kwk::utils::tools::linear_to_pos(index, container)};
      } else {
        return std::nullopt;
      }

    }

    template<concepts::container Container, typename Check>
    std::optional<kwk::position<Container::static_order>>
    find_if(Container const& container, Check f)
    {
      return this->find_if(container, f, true);
    }

    template <typename Func, concepts::container Out>
    constexpr std::optional<kwk::position<Out::static_order>>
    find_last_if(Out const& out, Func f)
    {
      auto res = this->find_if(out, f, false);
      return res;
    }



  };

  // inline kwk::sycl::context default_context{::sycl::gpu_selector_v};
  inline kwk::sycl::context default_context{};
}

namespace kwk
{

  template<typename Func, concepts::container C0, concepts::container... Cs>
  constexpr auto for_each(kwk::sycl::context& ctx, Func f, C0&& c0, Cs&&... cs)
  {
    // std::cout << "---------------------------------- for_each SYCL used !\n";
    return ctx.map(f, KWK_FWD(c0), KWK_FWD(cs)...);
  }

  // TODO reduce: soit on fait ctx.reduce, soit dans le contexte sycl on réécrit la fonction reduce avec sycl_context.
  template<typename Func, concepts::container In>
  constexpr auto reduce(kwk::sycl::context& ctx, In const& in, Func f, auto init)
  {
    // std::cout << "---------------------------------- reduce SYCL used !\n";
    // std::cout << "sycl reduce\n";
    return ctx.reduce(in, f, init);
  }

  template<concepts::container Container, typename Check>
  std::optional<kwk::position<Container::static_order>>
  find_if(kwk::sycl::context& ctx, Container const& c, Check f)
  {
    auto res = ctx.find_if(c, f, true);
    return res;
  }

  template <typename Func, concepts::container Out>
  constexpr std::optional<kwk::position<Out::static_order>>
  find_last_if(kwk::sycl::context& ctx, Out const& out, Func f)
  {
    return ctx.find_last_if(out, f);
  }

}