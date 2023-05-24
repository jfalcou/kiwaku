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

// #define DEBUG
// #define GUI

// CSV file
std::ofstream res_nano;
std::ofstream res_test;

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
      // int i = 1;
      // int pos = int(p);

      // if(xm1 >= 0 || ym1 >= 0)
      // {
      //   while((cells(pos-i).x >= xm1 || cells(pos-i).y >= ym1) && (pos-i) >= 0){
      //     if(cells(pos-i).y == curr.y && cells(pos-i).x == xm1){
      //       curr.connections[0] = (pos-i);
      //       break;
      //     }
      //     i++;
      //   }
      //   if(cells(pos-1).y == ym1 && cells(pos-1).x == curr.x) curr.connections[1] = (pos-1);

      // }
      if(xm1 >= 0 || ym1 >= 0)
      {
        // Trouver astuce pour find (b, position actuelle car trié)
        
      
        auto it = std::lower_bound(b+std::max((int(p)-size-1),0), b+int(p), cell{xm1,ym1,0});
        if(it != e && it->x == xm1 && it->y == ym1) curr.connections[0] = std::distance(b,it);

        it = std::lower_bound(b+std::max((int(p)-size), 0), b+int(p), cell{curr.x,ym1,0});
        if(it != e && it->x == curr.x && it->y == ym1) curr.connections[1] = std::distance(b,it);

        it = std::lower_bound(b+std::max((int(p)-size+1),0), b+int(p), cell{xp1,ym1,0});
        if(it != e && it->x == xp1 && it->y == ym1) curr.connections[2] = std::distance(b,it);

        it = std::lower_bound(b+int(p)-1, b+int(p), cell{xm1,curr.y,0});
        if(it != e && it->x == xm1 && it->y == curr.y) curr.connections[3] = std::distance(b,it);
      }

      return curr;
    }
  , cells, cells
  );
}

int main(int argc, char *argv[])
{
  using namespace kwk;

  int size = 2;

  if(argc > 1) 
  {
    size = std::atoi(argv[1]);
    std::string fname;
    fname = "./Benchmark_ccl_kwk_table_" + std::to_string(size) + ".csv";
    res_nano.open(fname);
    res_nano << "Size(N*N);Density(1/1000);Cells(Nb);Cycles/cell;Mean Nano(Cycles);Median Nano(Cycles);Min Nano(Cycles);Max Nano(Cycles);Err Nano(Cycles)\n";
  
    std::string f_test = "./Test_verif_" + std::to_string(size) + ".csv";
    res_test.open(f_test);
  }
  else
  {
    std::cout << "Size input needed";
    exit(EXIT_FAILURE);
  }

  auto cells        = table{of_size(size*size), as<cell> };
  auto equivalences = table{ of_size(size*size/2), as<int> };
  size_t startSeed = 71902647;
  std::mt19937_64 rnd(startSeed);


  std::vector<std::vector<int>> arr(size, std::vector<int>(size, 0));

  for (int density = 10; density <= 1000; density += 10) {

    fill_array(arr, size, density, startSeed);

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

#ifdef DEBUG
    std::cout << "Kiwaku : \n";
    std::cout << cells << "\n";
#endif
    uint32_t nb_cells = 0;
    for (int i = 0; i < size; ++i) {
      for (int j = 0; j < size; ++j) {
        if(arr[i][j] == 1){
          cells(i*size+j) = {j,i,0};
          nb_cells++;
        }
      }
    }
    for (int i = 0; i < (size*size/2); ++i)equivalences(i) = 0;
    std::sort(cells.get_data(), cells.get_data() + cells.numel());
          // Test 
      // auto cells        = table{ of_size(22), as<cell> };
      // auto equivalences = table{ of_size(22), as<int> };
      // auto nb_cells = 22;

      // cells(9) = {0,0,0};
      // cells(0) = {0,2,0};
      // cells(10) = {0,4,0};
      // cells(11) = {0,6,0};
      // cells(1) = {1,0,0};
      // cells(2) = {1,2,0};
      // cells(3) = {1,4,0};
      // cells(12) = {1,6,0};
      // cells(4) = {2,0,0};
      // cells(5) = {2,1,0};
      // cells(6) = {2,2,0};
      // cells(7) = {2,3,0};
      // cells(8) = {2,4,0};
      // cells(13) = {2,5,0};
      // cells(14) = {2,6,0};
      // cells(17) = {3,6,0};
      // cells(15) = {4,5,0};
      // cells(16) = {4,4,0};
      // cells(18) = {5,5,0};
      // cells(19) = {5,4,0};
      // cells(20) = {6,4,0};
      // cells(21) = {6,3,0};
    auto v_cell = view{source = (cells.get_data() + cells.numel() - nb_cells), of_size(nb_cells) };

#ifdef DEBUG
    std::cout << "Origin : \n";
    std::cout << cells << "\n";
#endif
    std::string ss;
    ss = "CCL " + std::to_string(size) + " : " + std::to_string(density);

    auto bench = ankerl::nanobench::Bench().minEpochIterations(1).epochs(1).run(ss, [&]
    {
    // Sort
      std::shuffle(v_cell.get_data(), v_cell.get_data() + v_cell.numel(), rnd);
      std::sort(v_cell.get_data(), v_cell.get_data() + v_cell.numel());
#ifdef DEBUG
    std::cout << "Sorted :  \n";
    std::cout << cells << "\n";
#endif
    // Find connections
      find_connections(v_cell, size);
#ifdef DEBUG
    std::cout << "Connected :  \n";
    std::cout << cells << "\n";
#endif
      
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
      
      int label = 1;

      kwk::transform
      ( [&](auto curr) mutable
        {
          auto prevxy = curr.connections[0];
          auto prevy = curr.connections[1];
          auto prevyx = curr.connections[2];
          auto prevx = curr.connections[3];

          // No neighbor cell
          if(prevx == -1 && prevy == -1 && prevxy == -1 && prevyx == -1) 
          { 
            curr.label = label++;
            equivalences(curr.label) = curr.label;
          }
          else {
            if(prevx != -1 && prevxy == -1) 
            {
              if(curr.label > 0)
              {
                // equivalences(cells(prevx).label) = std::min(equivalences(curr.label), equivalences(cells(prevx).label));
                // Intersection with 2 clusters + simultaneous resolution matching
                auto eqrec = v_cell(prevx).label;
                // cells(prevx).label = std::min(equivalences(curr.label), equivalences(eqrec));
                equivalences(curr.label) = std::min(curr.label, equivalences(eqrec));

                equivalences(std::max(curr.label, equivalences(eqrec))) = std::min(equivalences(curr.label), equivalences(eqrec));
                while(eqrec != std::min(equivalences(curr.label), equivalences(eqrec))){
                equivalences(eqrec) = std::min(equivalences(curr.label), equivalences(eqrec));
                  eqrec = equivalences(eqrec);
                } 
                equivalences(curr.label) = std::min(equivalences(curr.label), equivalences(eqrec));
                equivalences(v_cell(prevx).label) = std::min(equivalences(curr.label), equivalences(eqrec));
                curr.label = std::min(equivalences(curr.label), equivalences(eqrec));
              }
              else {
                curr.label = equivalences(v_cell(prevx).label);
              }
            }

            if(prevyx != -1 && prevy == -1)                 
            {
              if(curr.label > 0)
              {
                // equivalences(cells(prevy).label) = std::min(equivalences(curr.label), equivalences(cells(prevy).label));
                // Intersection with 2 clusters + simultaneous resolution matching
                auto eqrec = v_cell(prevyx).label;
                
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
                equivalences(v_cell(prevyx).label) = std::min(equivalences(curr.label), equivalences(eqrec));
                curr.label = std::min(equivalences(curr.label), equivalences(eqrec));
              } 
              else {
                curr.label = equivalences(v_cell(prevyx).label);
              }
            }

            if(prevy != -1)                 
            {
              if(curr.label > 0)
              {
                // Intersection with 2 clusters + simultaneous resolution matching
                auto eqrec = v_cell(prevy).label;

                equivalences(curr.label) = std::min(curr.label, equivalences(eqrec));

                equivalences(std::max(curr.label, equivalences(eqrec))) = std::min(equivalences(curr.label), equivalences(eqrec));
                while(eqrec != std::min(equivalences(curr.label), equivalences(eqrec))){
                equivalences(eqrec) = std::min(equivalences(curr.label), equivalences(eqrec));
                  eqrec = equivalences(eqrec);
                }
                equivalences(curr.label) = std::min(equivalences(curr.label), equivalences(eqrec));
                equivalences(v_cell(prevy).label) = std::min(equivalences(curr.label), equivalences(eqrec));
                curr.label = std::min(equivalences(curr.label), equivalences(eqrec));
              } 
              else {
                curr.label = equivalences(v_cell(prevy).label);
              }
            }

            if(prevxy != -1)                 
            {
              if(curr.label > 0)
              {
                // equivalences(cells(prevy).label) = std::min(equivalences(curr.label), equivalences(cells(prevy).label));
                // Intersection with 2 clusters + simultaneous resolution matching
                auto eqrec = v_cell(prevxy).label;
                
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
                equivalences(v_cell(prevxy).label) = std::min(equivalences(curr.label), equivalences(eqrec));
                curr.label = std::min(equivalences(curr.label), equivalences(eqrec));
              } 
              else {
                curr.label = equivalences(v_cell(prevxy).label);
              }
            }
          }
          return curr;
        }

      , v_cell, v_cell
      );  

#ifdef GUI
    kwk::for_each( [&](auto e) { if(e.x > -1 && e.y > -1)screen[e.x][e.y] = e.label; } , v_cell);

    for(auto row : screen)
    {
      for(auto p : row)
        if (int(p) > 0)std::cout << std::right << std::setw(2) << int(p) << " ";
        else std::cout << "   ";
      std::cout << "\n";
    }
    std::cout << "\n";
#endif

#ifdef DEBUG
    std::cout << v_cell << "\n";
    std::cout << "avant compression \n";
    std::cout << std::setw(2) << equivalences << "\n";
#endif

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
          equivalences(i) =equivalences(eqv);
        }
      }

#ifdef DEBUG
    std::cout << "post compression \n";
    std::cout << v_cell << "\n";
#endif

#ifdef DEBUG
     std::cout << std::setw(2) << equivalences << "\n";
#endif

      // Resolution
      kwk::transform
      ( [&](auto c)
        {
          c.label = equivalences(c.label);
          return c;
        }
      , v_cell, v_cell
      );
    
      // Last graphical check
#ifdef GUI
      kwk::for_each( [&](auto e) { if(e.x > -1 && e.y > -1)screen[e.x][e.y] = e.label; } , v_cell);


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
      
    });

    std::vector<ankerl::nanobench::Result> vres;
    vres = bench.results();
    double cyc_op_mean          =   vres.begin()->average(ankerl::nanobench::Result::Measure::cpucycles);
    double cyc_op_med           =   vres.begin()->median(ankerl::nanobench::Result::Measure::cpucycles);
    double cyc_op_min           =   vres.begin()->minimum(ankerl::nanobench::Result::Measure::cpucycles);
    double cyc_op_max           =   vres.begin()->maximum(ankerl::nanobench::Result::Measure::cpucycles);
    double cyc_op_err           =   vres.begin()->medianAbsolutePercentError(ankerl::nanobench::Result::Measure::cpucycles) ;

    std::sort(v_cell.get_data(), v_cell.get_data() + v_cell.numel());

    kwk::transform
      ( [&](auto c)
        {
          res_test  << c.x << "," << c.y << "," << c.label << "\n";
          return c;
        }
      , v_cell, v_cell
    );
    res_test << "\n";

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
  res_test.close();
}