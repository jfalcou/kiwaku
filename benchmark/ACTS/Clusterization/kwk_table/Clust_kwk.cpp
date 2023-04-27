#include <cstdint>
#include <kwk/kwk.hpp>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <random>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>

// Bench
#define ANKERL_NANOBENCH_IMPLEMENT
#include "../../../nanobench.h"

#define N 4
// #define GUI

// CSV file
std::ofstream res_nano;

namespace kwk
{
  void transform(auto func, auto& out, auto const&... in)
  {
    kwk::for_each( [&](auto i) { out(i) = func(in(i)...); }, out.shape() );
  }
}

struct cell
{
    int x = -1 ,y = -1, label = -1;
    int connections[2] = {-1,-1};

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
      return (a.x == b.x) ? a.y < b.y : a.x < b.x;
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
void find_connections(auto& cells)
{
  auto b = cells.get_data();
  auto e = cells.get_data() + cells.numel();

  kwk::transform
  ( [&](auto curr)
    {
      auto xm1 = curr.x-1;
      auto ym1 = curr.y-1;

      if(xm1 >= 0 || ym1 >= 0)
      {
        // Trouver astuce pour find (b, position actuelle car trié)
        auto it = std::find(b, e, cell{xm1,curr.y,0});
        if(it != e && it->x == xm1 && it->y == curr.y) curr.connections[0] = std::distance(b,it);
      
        it = std::find(b, e, cell{curr.x,ym1,0});
        if(it != e && it->x == curr.x && it->y == ym1) curr.connections[1] = std::distance(b,it);
      }

      return curr;
    }
  , cells, cells
  );
}

void run(int size){
  using namespace kwk;
  ankerl::nanobench::Bench bench;
  // Retrieving nanobench results
  std::vector<ankerl::nanobench::Result> vres;

  {
    auto cells        = table{ of_size(size*size), as<cell> };
    auto equivalences = table{ of_size(size*size), as<int> };
    size_t startSeed = 71902647;
    std::mt19937_64 rnd(startSeed);

    for (int density = 10; density <= 1000; density += 10) {
      // Percolation 
      std::vector<std::vector<int>> arr(size, std::vector<int>(size, 0));
      fill_array(arr, size, density, startSeed);

      std::string ss;
      ss = "CCL " + std::to_string(size) + " : " + std::to_string(density);

      // Initialise kwk cell table
      kwk::transform( [&](auto c) 
      { 
        c.x = -1; 
        c.y = -1; 
        c.label = -1; 
        c.connections[0] = -1; 
        c.connections[1] = -1; 
        return c;
      }, cells, cells
      );

      uint32_t nb_cells = 0;
      // std::cout << "Kiwaku : \n";
      // std::cout << cells << "\n";
      // Fill kwk table with percolation array
      for (int i = 0; i < size; ++i) {
          for (int j = 0; j < size; ++j) {
              if(arr[i][j] == 1){
                cells(i*size+j) = {i,j,0};
                nb_cells++;
              }
          }
      }
      std::sort(cells.get_data(), cells.get_data() + cells.numel());
      // std::cout << "Sorted :  \n";
      // std::cout << cells << "\n";
      auto v = view{source = (cells.get_data() + cells.numel() - nb_cells), of_size(nb_cells) };
      // std::cout << "Sorted :  \n";
      // std::cout << cells << "\n";
bench = ankerl::nanobench::Bench().minEpochIterations(1).epochs(1).run(ss, [&]
{

      std::shuffle(cells.get_data(), cells.get_data() + cells.numel(), rnd);


      // std::cout << "Origin : \n";
      // std::cout << cells << "\n";
      // Sort
      std::sort(cells.get_data(), cells.get_data() + cells.numel());
      // std::cout << "Sorted :  \n";
      // std::cout << cells << "\n";
      
      // Find connections
      find_connections(cells);
      // std::cout << "Connected :  \n";
      // std::cout << cells << "\n";

#ifdef GUI
      // Add graphical view
      std::cout << "Density of percolation : " << density << "/1000" << std::endl; 
      std::vector<std::vector<char>> screen(size,std::vector<char>(size));
      for(auto& row : screen)
        for(auto& p : row)
            p = 0;
      
      kwk::for_each( [&](auto e) { if(e.x > -1 && e.y > -1)screen[e.x][e.y] = 'X'; }, cells);

      for(auto row : screen)
      {
        for(auto p : row)
          if (int(p) > 0)std::cout << " " << p << " ";
          else std::cout << "   ";
        std::cout << "\n";
      }
      std::cout << "\n";
#endif
/*
      int label = 1;

      // Clustering
      kwk::transform
      ( [&](auto curr) mutable
        {
          auto prevx = curr.connections[0];
          auto prevy = curr.connections[1];

          // No neighbor cell
          if(prevx == -1 && prevy == -1 && curr.x > -1 && curr.y > -1) 
          { 
            curr.label = label++;
            equivalences(curr.label) = curr.label;
          }
          // Neighbor cell
          else if(curr.x > -1 && curr.y > -1){
            if(prevx != -1) 
            {
              if(curr.label > 0)
              {
                // equivalences(cells(prevx).label) = std::min(equivalences(curr.label), equivalences(cells(prevx).label));
                // Intersection with 2 clusters + simultaneous resolution matching
                auto eqrec = cells(prevx).label;
                // cells(prevx).label = std::min(equivalences(curr.label), equivalences(eqrec));
                equivalences(curr.label) = std::min(curr.label, equivalences(eqrec));

                equivalences(std::max(curr.label, equivalences(eqrec))) = std::min(equivalences(curr.label), equivalences(eqrec));
                while(eqrec != std::min(equivalences(curr.label), equivalences(eqrec))){
                equivalences(eqrec) = std::min(equivalences(curr.label), equivalences(eqrec));
                  eqrec = equivalences(eqrec);
                } 
                equivalences(curr.label) = std::min(equivalences(curr.label), equivalences(eqrec));
                equivalences(cells(prevx).label) = std::min(equivalences(curr.label), equivalences(eqrec));
                curr.label = std::min(equivalences(curr.label), equivalences(eqrec));
              }
              else {
                curr.label = equivalences(cells(prevx).label);
              }
            }

            if(prevy != -1)                 
            {
              if(curr.label > 0)
              {
                // equivalences(cells(prevy).label) = std::min(equivalences(curr.label), equivalences(cells(prevy).label));
                // Intersection with 2 clusters + simultaneous resolution matching
                auto eqrec = cells(prevy).label;
                
                // curr = 4, eqrec = 5
                // cells(prevy).label = std::min(equivalences(curr.label), equivalences(eqrec));
                // curr.label = std::min(equivalences(curr.label), equivalences(eqrec));
                equivalences(curr.label) = std::min(curr.label, equivalences(eqrec));

                equivalences(std::max(curr.label, equivalences(eqrec))) = std::min(equivalences(curr.label), equivalences(eqrec));
                while(eqrec != std::min(equivalences(curr.label), equivalences(eqrec))){
                equivalences(eqrec) = std::min(equivalences(curr.label), equivalences(eqrec));
                  eqrec = equivalences(eqrec);
                }
                equivalences(curr.label) = std::min(equivalences(curr.label), equivalences(eqrec));
                equivalences(cells(prevy).label) = std::min(equivalences(curr.label), equivalences(eqrec));
                curr.label = std::min(equivalences(curr.label), equivalences(eqrec));
              } 
              else {
                curr.label = equivalences(cells(prevy).label);
              }
            }
          }
          return curr;
        }
      , cells, cells
      );  
      

      // for(auto row : screen)
      // {
      //   for(auto p : row)
      //     if (int(p) > 0)std::cout << std::right << std::setw(2) << int(p) << " ";
      //     else std::cout << "   ";
      //   std::cout << "\n";
      // }
      // std::cout << "\n";

      // std::cout << "avant compression \n";
      // std::cout << std::setw(2) << equivalences << "\n";

      //Compress label + last resolve
      int max_label = 2;
      for (int i = 2; i <= label; i++) {
          if (equivalences(i) == i) {
              equivalences(i) = max_label++;
          } else {
              auto eqv = equivalences(i);
              // while (eqv != equivalences(eqv)) {
              //     eqv = equivalences(eqv);
              // }
              equivalences(i) = equivalences(eqv);
          }
      }

#ifdef GUI

      // std::cout << cells << "\n";

      kwk::for_each( [&](auto e) { if(e.x > -1 && e.y > -1)screen[e.x][e.y] = e.label; } , cells);

      std::cout << "post compression \n";
      // std::cout << cells << "\n";

      std::cout << std::setw(2) << equivalences << "\n";
#endif
      // Resolution
      kwk::transform
      ( [&](auto c)
        {
          c.label = equivalences(c.label);
          return c;
        }
      , cells, cells
      );
      */
});
#ifdef GUI
      // Last graphical check
      kwk::for_each( [&](auto e) { if(e.x > -1 && e.y > -1)screen[e.x][e.y] = e.label; } , cells);

      std::cout << "Result: " << "\n\n";
      for(auto row : screen)
      {
        for(auto p : row)
          if (int(p) > 0)std::cout << std::right << std::setw(2) << int(p) << " ";
          else std::cout << "   ";
        std::cout << "\n";
      }
      std::cout << "\n";
#endif

      vres = bench.results();
      double cyc_op_mean          =   vres.begin()->average(ankerl::nanobench::Result::Measure::cpucycles);
      double cyc_op_med           =   vres.begin()->median(ankerl::nanobench::Result::Measure::cpucycles);
      double cyc_op_min           =   vres.begin()->minimum(ankerl::nanobench::Result::Measure::cpucycles);
      double cyc_op_max           =   vres.begin()->maximum(ankerl::nanobench::Result::Measure::cpucycles);
      double cyc_op_err           =   vres.begin()->medianAbsolutePercentError(ankerl::nanobench::Result::Measure::cpucycles) ;

      res_nano 
      << size << ";"
      << density << ";"
      << cyc_op_mean << ";"
      << cyc_op_med << ";"
      << cyc_op_min << ";"
      << cyc_op_max << ";"
      << cyc_op_err << "\n";

    }
  }
}

int main(int argc, char *argv[])
{

  int size = std::atoi(argv[1]);

  if(argc > 1) 
  {
    std::string fname = "./Benchmark_ccl_kwk_table_" + std::to_string(size) + ".csv";
    res_nano.open(fname);
    res_nano << "Size(N*N);Density(1/1000);Mean Nano(Cycles);Median Nano(Cycles);Min Nano(Cycles);Max Nano(Cycles);Err Nano(Cycles)\n";
  }
  else
  {
    std::cout << "Size input needed";
    exit(EXIT_FAILURE);
  }
    
  run(size);

  // CSV close
  res_nano.close();
}
