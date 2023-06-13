#include "kwk/utility/container/shape.hpp"
#include <cstdint>
#include <kwk/kwk.hpp>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <random>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>

// Bench
#define ANKERL_NANOBENCH_IMPLEMENT
#include "../../../nanobench.h"

// Unit Test output
#define UNIT

// CSV file
std::ofstream res_nano;

#ifdef UNIT
std::ofstream res_test;
#endif

namespace kwk
{
  void transform(auto func, auto& out, auto const&... in)
  {
    kwk::for_each( [&](auto... i) { out(i...) = func(in(i...)...); }, out.shape() );
  }

   void transform_index(auto func, auto& out, auto const&... in)
  {
    kwk::for_each( [&](auto... i) { out(i...) = func(in(i...)..., i...); }, out.shape() );
  }
}

struct cell
{
    int x = -1 ,y = -1, label = -1;
    int connections[4] = {-1,-1, -1, -1};

    friend std::ostream& operator<<(std::ostream& os, cell c)
    {
        return  os << '('  << c.x << ", " << c.y << ", " << c.label << ')' 
        << '[' << c.connections[0] << ',' << c.connections[1] << ']'
        << '\n';
    }

    friend bool operator==(cell a, cell b)
    {
      return (a.x == b.x) && (a.y == b.y);
    }

    friend bool operator<(cell a, cell b)
    {
      return (a.y == b.y) ? a.x < b.x : a.y < b.y;
    }
};

struct equivalence
{
  int current, ancestor;
  friend std::ostream& operator<<(std::ostream& os, equivalence e)
  {
    return  os << '('  << e.current << " -> " << e.ancestor << ')';
  }

   friend bool operator<(equivalence a, equivalence b)
    {
      return b.current < a.current;
    }
};

void fill_array(std::vector<std::vector<int>>& arr, int n, int density, size_t startSeed) {
    std::mt19937 gen(startSeed);
    std::uniform_int_distribution<> dist(0, 1000);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int random_value = dist(gen);
            arr[i][j] = (random_value <= density) ? 1 : 0;
        }
    }
}


// Find connection cells with previous cells if exist
void find_connections(auto& cells, int size)
{
  auto b = cells.get_data();
  auto e = cells.get_data() + cells.numel();

  
  kwk::transform_index
  ( [&](auto curr, auto p)
    {
      auto xm1 = curr.x-1;
      auto xp1 = curr.x+1;
      auto ym1 = curr.y-1;

      if(xm1 >= 0 || ym1 >= 0)
      {
        auto it = std::lower_bound(b+std::max((int(p)-size-1),0), b+int(p), cell{xm1,ym1,0});
        if(it != e && it->x == xm1 && it->y == ym1) curr.connections[0] = std::distance(b,it);

        it = std::lower_bound(b+curr.connections[0], b+curr.connections[0]+10, cell{curr.x,ym1,0});
        if(it != e && it->x == curr.x && it->y == ym1) curr.connections[1] = std::distance(b,it);

        it = std::lower_bound(b+curr.connections[0], b+curr.connections[0]+15, cell{xp1,ym1,0});
        if(it != e && it->x == xp1 && it->y == ym1) curr.connections[2] = std::distance(b,it);

        it = std::lower_bound(b+int(p)-1, b+int(p), cell{xm1,curr.y,0});
        if(it != e && it->x == xm1 && it->y == curr.y) curr.connections[3] = std::distance(b,it);
      }      

      // if(xm1 >= 0 || ym1 >= 0)
      // {
      //   auto it = std::lower_bound(b+std::max((int(p)-size-1),0), b+int(p), cell{xp1,ym1,0});
      //   if(it != e && it->x == xp1 && it->y == ym1) curr.connections[2] = std::distance(b,it);

      //   it = std::lower_bound(b+int(p)-2, b+int(p)-1, cell{xm1,curr.y,0});
      //   if(it != e && it->x == xm1 && it->y == curr.y) curr.connections[3] = std::distance(b,it);

      //   int pos = curr.connections[3];

      //   while(pos >= (curr.connections[3]-3))
      //   {
      //     if(cells(pos).x == curr.x && cells(pos).y == ym1) curr.connections[1] = pos;
      //     if(cells(pos).x == xm1 && cells(pos).y == ym1){
      //       curr.connections[0] = pos;
      //       break;
      //     } 
      //     pos--;
      //   }
      // }
      return curr;
    }
  , cells, cells
  );
}

void union_find(auto& cell, auto& curr, auto prev, auto &equivalences)
{
  if(curr.label > 0)
  {
    // Intersection with 2 clusters + simultaneous resolution matching
    auto eqrec = cell(prev).label;
    equivalences(curr.label) = std::min(curr.label, equivalences(eqrec));

    equivalences(std::max(curr.label, equivalences(eqrec))) = std::min(equivalences(curr.label), equivalences(eqrec));
    while(eqrec != std::min(equivalences(curr.label), equivalences(eqrec))){
    equivalences(eqrec) = std::min(equivalences(curr.label), equivalences(eqrec));
      eqrec = equivalences(eqrec);
    } 
    equivalences(curr.label) = std::min(equivalences(curr.label), equivalences(eqrec));
    equivalences(cell(prev).label) = std::min(equivalences(curr.label), equivalences(eqrec));
    curr.label = std::min(equivalences(curr.label), equivalences(eqrec));
  }
  else {
    curr.label = equivalences(cell(prev).label);
  }
}

int main(int argc, char *argv[])
{
  using namespace kwk;

  int size = 2;

  // Input args for matrix size
  if(argc > 1) 
  {
    size = std::atoi(argv[1]);
    std::string fname;
    fname = "./Benchmark_ccl_kwk_table_" + std::to_string(size) + ".csv";
    res_nano.open(fname);
    res_nano << "Size(N*N);Density(1/1000);Cells(Nb);Cycles/cell;Mean Nano(Cycles);Median Nano(Cycles);Min Nano(Cycles);Max Nano(Cycles);Err Nano(Cycles)\n";
  }
  else
  {
    std::cout << "Size input needed";
    exit(EXIT_FAILURE);
  }

#ifdef UNIT
  std::string f_test = "./Test_verif_" + std::to_string(size) + ".csv";
  res_test.open(f_test);
#endif

  auto equivalences = table{ of_size(size*size/2), as<int> };
  std::vector<std::vector<int>> arr(size, std::vector<int>(size, 0));

  size_t startSeed = 71902647;
  std::mt19937_64 rnd(startSeed);

  // Setting matrix sparsity *1/1000
  for (int density = 1; density <= 10; density += 1) {
    fill_array(arr, size, density, startSeed);

    uint32_t nb_cells;

    auto cells        = table{of_size(size*size), as<cell> };
      // To implement in kwk (executing default constructor)
    kwk::transform( [&](auto c) 
    { 
      c.x = -1; 
      c.y = -1; 
      c.label = -1; 
      c.connections[0] = -1; 
      c.connections[1] = -1; 
      c.connections[2] = -1; 
      c.connections[3] = -1; 
      return c;
    }, cells, cells
    ); 

    // Fill kwk table from sparse array (Should not be needed in ACTS)
    nb_cells = 0;
    for (int i = 0; i < size; ++i) {
      for (int j = 0; j < size; ++j) {
        if(arr[i][j] == 1){
          cells(i*size+j) = {j,i,0};
          nb_cells++;
        }
      }
    }

    std::sort(cells.get_data(), cells.get_data() + cells.numel());

    // Nanobench - To update epochs with independent context
    std::string ss;
    ss = "CCL " + std::to_string(size) + " : " + std::to_string(density);

    auto bench = ankerl::nanobench::Bench().minEpochIterations(1).epochs(1).run(ss, [&]
    {
      // To implement in kwk (Fill table with default value)
      for (int i = 0; i < (size*size/2); ++i)equivalences(i) = 0;

      auto t_cell = table{source = cells.get_data(), of_size(size*size)};
      // Making a view on non-empty cells from kwk table (Maybe to implement ?)
      auto v_cell = view{source = (t_cell.get_data() + t_cell.numel() - nb_cells), of_size(nb_cells) };

      // Sort - Should add label and connections table reset
      std::shuffle(v_cell.get_data(), v_cell.get_data() + v_cell.numel(), rnd);
      std::sort(v_cell.get_data(), v_cell.get_data() + v_cell.numel());

      // Find connections
      find_connections(v_cell, size);
      
      int label = 1;
      kwk::transform
      ( [&](auto curr) mutable
        {
          auto prevxy = curr.connections[0];  // -1 -1
          auto prevy  = curr.connections[1];  // -1  0   
          auto prevyx = curr.connections[2];  // -1 +1
          auto prevx  = curr.connections[3];  //  0 -1

          // No neighbor cell = new label
          if(prevx == -1 && prevy == -1 && prevxy == -1 && prevyx == -1) 
          { 
            curr.label = label++;
            equivalences(curr.label) = curr.label;
          }
          else { // To rearange
            if(prevx != -1 && prevxy == -1) 
            {
              union_find(v_cell, curr, prevx, equivalences);
            }

            if(prevyx != -1 && prevy == -1)                 
            {
              union_find(v_cell, curr, prevyx, equivalences);
            }

            if(prevy != -1)                 
            {
              union_find(v_cell, curr, prevy, equivalences);
            }

            if(prevxy != -1)                 
            {
              union_find(v_cell, curr, prevxy, equivalences);
            }
          }
          return curr;
        }

      , v_cell, v_cell
      );

      //Compress label + last resolve
      int max_label = 2;
      for (int i = 2; i <= label; i++) {
        if (equivalences(i) == i) {
          equivalences(i) = max_label++;
        } else {
          auto eqv = equivalences(i);
          equivalences(i) = equivalences(eqv);
        }
      }

      // Resolution
      kwk::transform
      ( [&](auto c)
        {
          c.label = equivalences(c.label);
          return c;
        }
      , v_cell, v_cell
      );

    // end nanobench
    });

    std::vector<ankerl::nanobench::Result> vres;
    vres = bench.results();
    double cyc_op_mean          =   vres.begin()->average(ankerl::nanobench::Result::Measure::cpucycles);
    double cyc_op_med           =   vres.begin()->median(ankerl::nanobench::Result::Measure::cpucycles);
    double cyc_op_min           =   vres.begin()->minimum(ankerl::nanobench::Result::Measure::cpucycles);
    double cyc_op_max           =   vres.begin()->maximum(ankerl::nanobench::Result::Measure::cpucycles);
    double cyc_op_err           =   vres.begin()->medianAbsolutePercentError(ankerl::nanobench::Result::Measure::cpucycles) ;

#ifdef UNIT
    // std::sort(v_cell.get_data(), v_cell.get_data() + v_cell.numel());

    // kwk::transform
    //   ( [&](auto c)
    //     {
    //       res_test  << c.x << "," << c.y << "," << c.label << "\n";
    //       return c;
    //     }
    //   , v_cell, v_cell
    // );
    // res_test << "\n";
#endif
    res_nano 
    << size << ";"
    << density << ";"
    << nb_cells << ";"
    << cyc_op_mean/nb_cells << ";"
    << cyc_op_mean << ";"
    << cyc_op_med << ";"
    << cyc_op_min << ";"
    << cyc_op_max << ";"
    << cyc_op_err << "\n";

  }
  // CSV close
  res_nano.close();
#ifdef UNIT
  res_test.close();
#endif
}