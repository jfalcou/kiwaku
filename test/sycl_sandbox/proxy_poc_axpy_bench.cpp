#include <CL/sycl.hpp>
#include <array>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <kwk/kwk.hpp>

// #include "../../sycl_common.hpp"
#include "utils/utils.hpp"

#define ENABLE_OMP true

#if ENABLE_OMP
#include <omp.h>
#endif

// dpcpp kwk_axpy_bench.cpp -o e.exe -O3 -std=c++20 -I/home/data_sync/academique/These/kiwaku_2023/kiwaku/include/ && ./e.exe

// Version 2023-08-18

/*
TODO 17 08 2023

Comprendre le principe, être capable de l'expliquer à David et Hadrien, 
et être capable de modifier. Faire un document pour résumer la manière dont 
fonctionne ce code.

Écrire les commentaires.

Faire un bench en float, comparer l'overhead induit par notre code par rapport
au code de base sans Kiwaku. (bench en -O3 avec une version récente de icpx
et les drivers CUDA à jour, sur Legend)

On a fait le choix d'utiliser les USM buffeurs/accesseurs et de ne pas utiliser
les USM. A terme, on pourra décider d'implémenter les deux, mais pour l'instant
le plus simple et ergonomique nous a paru être les buffers.

Une fois que j'ai fini ça :
- Discuter avec Joël de l'article proceedings de CHEP lundi 21 et mardi 22.
- Regarder mon emploi du temps de cours pour la rentrée
- Faire les slides de mon rapport de mi-parcours


*/

namespace kwk_poc::bench
{
  struct result_single
  {
    // Elapsed time in milliseconds
    std::size_t host_alloc, copies_and_kernel, checksum;
  };

  struct result_vector
  {
    // Elapsed time in milliseconds
    std::vector<std::size_t> host_alloc, copies_and_kernel, checksum;

    void clear()
    {
      host_alloc.clear();
      copies_and_kernel.clear();
      checksum.clear();
    }

    void write_to_gfile()
    {
      for (std::size_t i = 0; i < host_alloc.size(); ++i) {
        write_f << host_alloc[i] << " ";
      }
      write_f << "\n";

      for (std::size_t i = 0; i < host_alloc.size(); ++i) {
        write_f << copies_and_kernel[i] << " ";
      }
      write_f << "\n";

      for (std::size_t i = 0; i < host_alloc.size(); ++i) {
        write_f << checksum[i] << " ";
      }
      write_f << "\n";
    }
  };
}

namespace kwk_poc::concepts
{
  namespace sycl
  {
    // Le proxy est notre manière de passer d'une structure de données Kiwaku
    // à un accesseur.
    // Un proxy doit avoir une méthode .size() pour en donner le nombre d'éléments
    // et une méthode pour retourner créer un accesseur vers ses données.
    template<typename T>
    concept proxy = requires(T& t, ::sycl::handler& h)
    {
      { t.size()    };
      { t.access(h) };
    };

    template<typename T>
    concept proxy_accessor = requires(T& t, ::sycl::handler& h)
    {
      { t.access(h) };
    };
  }

  // Un contexte kiwaku doit avoir une propriété "is_context"
  // (ça nous sert de détrompeur, simplement)
  template<typename T>
  concept context = requires(T const&)
  {
    typename T::is_context;
  };
}

namespace kwk_poc
{
  // Contexte de base qui ne fait rien. Tous les autres contextes vont hériter
  // publiquement de ce base_context. Les méthodes de base_context ne sont pas
  // virtuelles, elles seront écrasées par les méthodes des autres types enfants
  // sans appel virtuel (programmation à base de templates, sans appels virtuels,
  // donc en concervant les performances)
  template<typename Context> // kwk::concepts::context enlevé parce que sinan erreur
  struct base_context
  {
    using is_context = void;
    // Les algos vont ici, for_each compris
    static auto const&  in   (kwk::concepts::container auto const& c) { return c; }
    static auto&        out  (kwk::concepts::container auto& c)       { return c; }
    static auto&        inout(kwk::concepts::container auto& c)       { return c; }
  };
}


namespace kwk_poc::sycl
{
  //PROXY
  namespace proxy
  {
    // base proxy
    template<typename T>
    class base
    {
    public:
      base(kwk::concepts::container auto const& c)
      : data(c.get_data(), ::sycl::range<1>(c.numel())) {}
      
      auto size() const { return data.size(); }

    protected:
      ::sycl::buffer<T> data;
    };

    // in proxy
    template<typename T>
    class in : public base<T>
    {
    public:
      auto access(::sycl::handler& h)
      {
        return ::sycl::accessor(base<T>::data, h, ::sycl::read_only);
      }
    };
    template<kwk::concepts::container T>
    in(T const&) -> in<typename T::value_type>;

    // out proxy
    template<typename T>
    class out : public base<T>
    {
    public:
      auto access(::sycl::handler& h)
      {
        return ::sycl::accessor(base<T>::data, h, ::sycl::write_only, ::sycl::no_init);
      }
    };
    template<kwk::concepts::container T>
    out(T const&) -> out<typename T::value_type>;

    // inout proxy
    template<typename T>
    class inout : public base<T>
    {
    public:
      auto access(::sycl::handler& h)
      {
        return ::sycl::accessor(base<T>::data, h, ::sycl::read_write);
      }
    };
    template<kwk::concepts::container T>
    inout(T const&) -> inout<typename T::value_type>;
  }


  //CONTEXT SYCL
  struct context : private ::sycl::queue, public kwk_poc::base_context<context>
  {
    using parent = ::sycl::queue;
    using parent::parent; // forwards every constructor of queue to context.
    // "using parent::parent" permet d'utiliser tous les constructeurs de ::sycl::queue
    // sans avoir à les redéfinir
    
    static auto in   (kwk::concepts::container auto const& c) { return proxy::in{c};    }
    static auto out  (kwk::concepts::container auto& c)       { return proxy::out{c};   }
    static auto inout(kwk::concepts::container auto& c)       { return proxy::inout{c}; }

    template<typename Func>
    void for_each(Func f, concepts::sycl::proxy auto&& p0, concepts::sycl::proxy auto&&... ps)
    {
      std::cout 
      << "Running on device: "
      << parent::get_device().get_info<::sycl::info::device::name>()
      << "\n";

      parent::submit([&](::sycl::handler &h) 
      {
        // Maps each sycl proxy to an accessor
        auto accs = kumi::map (
                                [&](auto& b) { return b.access(h); }
                              , kumi::tuple{p0, ps...}
                              );

        h.parallel_for(p0.size(), [=](auto i)
        {
          // For each element of the input tables
          // call our lambda parameter with the input accessors
          kumi::apply([=](auto&&... m) { f(KWK_FWD(m)[i]...); }, accs);
        });
      });

      parent::wait();
    }
  };
}


namespace kwk_poc::cpu
{
  struct context : public kwk_poc::base_context<context>
  {
    
    // static auto in(kwk::concepts::container auto const& c) { return in_proxy{c}; }
    // static auto out(kwk::concepts::container auto& c)      { return out_proxy{c}; }
    // static auto inout(kwk::concepts::container auto& c)    { return inout_proxy{c}; }

    template<typename Func>
    void for_each (
                    Func f
                  , kwk::concepts::container auto&& p0
                  , kwk::concepts::container auto&&... ps
                  )
    {
      std::cout << "Running on cpu.\n";

      // QUESTION : pourquoi ça marche pas avec kumi ??
      // Parce que copie du conteneur et pas réf ?
      // Ça marche avec les pointeurs en tout cas
      // auto accs = kumi::tuple{&p0, &ps...};

      for (std::size_t i = 0; i < p0.size(); ++i)
      {
        f(p0(i), ps(i)...);
        // kumi::apply([=](auto&&... m) { f(KWK_FWD(*m)(i)...); }, accs);
        // std::cout << "p0[" << i << "](" << p0(i) << ") ";
      }
      // std::cout << "\n";
    }
  };
}

#if ENABLE_OMP
namespace kwk_poc::omp
{
  struct context : public kwk_poc::base_context<context>
  {
    template<typename Func>
    void for_each (
                    Func f
                  , kwk::concepts::container auto&& p0
                  , kwk::concepts::container auto&&... ps
                  )
    {
      std::cout << "Running on OpemMP.\n";

      // #pragma omp parallel for
      #pragma omp target parallel for map(to:ps...) map(from:p0)
      for (std::size_t i = 0; i < p0.size(); ++i)
      {
        f(p0(i), ps(i)...);
      }
    }
  };
}
#endif


// Notre fonction transform prend en paramètre un contexte, une lambda, 
// un conteneur en sortie et 0 ou n conteneur.s en entrée
template<kwk_poc::concepts::context Context, typename Func>
static auto transform ( Context&& context
                      , Func f
                      , kwk::concepts::container auto& out
                      , kwk::concepts::container auto const&... ins
                      )
{
  // le KWK_FWD est nécessaire pour éviter une erreur de compilation
  // Appel de la fonction for_each du contexte passé en paramètre template
  // La fonction f passée en paramètre est appliquée à tous les éléments des 
  // conteneurs ins, sa sortie est sauvegardée dans le conteneur out.
  // Le conteneur out est transformé en 
  KWK_FWD(context).for_each
  (
    [f](auto& o, auto const&... i) { o = f(i...); }
  , Context::out(out)
  , Context::in(ins)...
  );
}

template<typename Func>
void check_results(
                    std::size_t array_size
                  , Func func
                  , kwk::concepts::container auto&& out
                  , kwk::concepts::container auto&&... ins
                        )
{
  const float tolerance_ratio = 0.001;
  // Vérification de la validité
  for (std::size_t i = 0; i < array_size; ++i)
  {
    auto expected_val = func(ins(i)...);
    //in_a(i) * in_cst + in_b(i);
    float delta = std::abs(out(i) - expected_val);
    if ( (delta > abs(tolerance_ratio * expected_val)) || (delta > abs(tolerance_ratio * out(i))) )
    // if (out(i) != expected_val) 
    {
      std::cout << ">>>>> FAILURE @check_results: at index " << i 
                << " out=" << out(i)
                << " should be " << (expected_val)
                << "\n";
      std::exit(-1);
    }
  }
}

template<kwk_poc::concepts::context Context>
kwk_poc::bench::result_single main_bench(
                                      std::string context_name
                                    , const std::size_t array_size
                                    , kwk_poc::bench::result_vector& rvect
                                    )
{
  // std::vector<> bres_vect;
  kwk_poc::bench::result_single bres;
  bench::chrono_t chrono;

  float in_cst     = 9; 
  auto f = [k = 0](auto& e) mutable { e = k++; };

  chrono.Init();
  auto in_a = kwk::table{kwk::of_size(array_size), kwk::real32}; // input
  auto in_b = kwk::table{kwk::of_size(array_size), kwk::real32}; // input
  auto out  = kwk::table{kwk::of_size(array_size), kwk::real32}; // output
  kwk::for_each(f, in_a);
  kwk::for_each(f, in_b);

  bres.host_alloc = chrono.ElapsedTimeMS(); chrono.Init();

  transform (
              Context{}
            , [=](auto x, auto y) { return y + x * in_cst; }
            , out
            , in_a
            , in_b
            );
  
  bres.copies_and_kernel = chrono.ElapsedTimeMS(); chrono.Init();

  float chksum = 0;
  for (std::size_t i = 0; i < array_size; ++i)
  {
    chksum += out(i);
  }
  std::cout << "chksum = " << chksum << "\n";
  bres.checksum = chrono.ElapsedTimeMS(); chrono.Init();

  auto func = [in_cst](auto&& in1, auto&& in2) {
    return in1 * in_cst + in2;
  };
  check_results(array_size, func, out, in_a, in_b);

  // write_f << bres.host_alloc << " "
  //         << bres.copies_and_kernel << " "
  //         << bres.checksum << "\n";
  
  rvect.host_alloc        .push_back(bres.host_alloc);
  rvect.copies_and_kernel .push_back(bres.copies_and_kernel);
  rvect.checksum          .push_back(bres.checksum);

  return bres;
}



int main(int argc, char* argv[])
{
  std::size_t file_version = 2;
  std::size_t array_size = 50000000;
  array_printer_t ap;
  kwk_poc::bench::result_single r;
  kwk_poc::bench::result_vector rvect;
  std::size_t repeat = 10;
  ap.add({"device", "alloc_host", "cpy+ker", "chksum"});

  write_f.open("bench/kwk_buffers_cpu.txt");
  write_f << file_version << "\n";
  rvect.clear();
  for (std::size_t i = 0; i < repeat; ++i)
  {
    r = main_bench<kwk_poc::cpu::context>("cpu", array_size, rvect);
    ap.add({"cpu", std::to_string(r.host_alloc), std::to_string(r.copies_and_kernel), std::to_string(r.checksum)});
  }
  rvect.write_to_gfile();
  write_f.close();

  write_f.open("bench/kwk_buffers_omp.txt");
  write_f << file_version << "\n";
  rvect.clear();
  for (std::size_t i = 0; i < repeat; ++i)
  {
    r = main_bench<kwk_poc::omp::context>("omp", array_size, rvect);
    ap.add({"omp", std::to_string(r.host_alloc), std::to_string(r.copies_and_kernel), std::to_string(r.checksum)});
  }
  rvect.write_to_gfile();
  write_f.close();


  write_f.open("bench/kwk_buffers_sycl.txt");
  write_f << file_version << "\n";
  rvect.clear();
  for (std::size_t i = 0; i < repeat; ++i)
  {
    r = main_bench<kwk_poc::sycl::context>("sycl", array_size, rvect);
    ap.add({"sycl", std::to_string(r.host_alloc), std::to_string(r.copies_and_kernel), std::to_string(r.checksum)});
  }
  rvect.write_to_gfile();
  write_f.close();

  ap.print();
}

