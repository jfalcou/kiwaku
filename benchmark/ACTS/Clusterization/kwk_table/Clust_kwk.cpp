#include <kwk/kwk.hpp>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <random>
#include <vector>
#include <cmath>
#include <boost/functional/hash.hpp>

// Bench
#define ANKERL_NANOBENCH_IMPLEMENT
#include "../../../nanobench.h"

#define N 1

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

int main()
{
  using namespace kwk;

  for(int size = (2<<(N+N/2)); size < (2<<(N+N/2+1)); size*=2){
    auto cells        = table{of_size(size*size), as<cell> };
    auto equivalences = table{ of_size(size*size/2), as<int> };
    size_t startSeed = 71902647;

    std::vector<std::vector<int>> arr(size, std::vector<int>(size, 0));

    for (int density = 100; density <= 1000; density += 100) {
      fill_array(arr, size, density, startSeed);

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

      // std::cout << "Kiwaku : \n";
      // std::cout << cells << "\n";
      for (int i = 0; i < size; ++i) {
          for (int j = 0; j < size; ++j) {
              if(arr[i][j] == 1)cells(i*size+j) = {i,j,0};
          }
      }

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

      int label = 1;

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
          else {
            if(prevx != -1) 
            {
              if(curr.label > 0)
              {
                equivalences(curr.label) = std::min(equivalences(curr.label), equivalences(cells(prevx).label));
                equivalences(cells(prevx).label) = std::min(equivalences(curr.label), equivalences(cells(prevx).label));
                // Intersection with 2 clusters + simultaneous resolution matching
                // auto eqrec = cells(prevx).label;
                // cells(prevx).label = std::min(equivalences(curr.label), equivalences(eqrec));
                // equivalences(std::max(curr.label, equivalences(eqrec))) = std::min(equivalences(curr.label), equivalences(eqrec));
                // while(eqrec != std::min(equivalences(curr.label), equivalences(eqrec))){
                // equivalences(eqrec) = std::min(equivalences(curr.label), equivalences(eqrec));
                //   eqrec = equivalences(eqrec);
                // } 
                // curr.label = std::min(equivalences(curr.label), equivalences(eqrec));
              }
              else curr.label = cells(prevx).label;
            }

            if(prevy != -1)                 
            {
              if(curr.label > 0)
              {
                equivalences(curr.label) = std::min(equivalences(curr.label), equivalences(cells(prevy).label));
                equivalences(cells(prevy).label) = std::min(equivalences(curr.label), equivalences(cells(prevy).label));
                // Intersection with 2 clusters + simultaneous resolution matching
                // auto eqrec = cells(prevy).label;
                // cells(prevy).label = std::min(equivalences(curr.label), equivalences(eqrec));
                // // curr.label = std::min(equivalences(curr.label), equivalences(eqrec));
                // equivalences(std::max(curr.label, equivalences(eqrec))) = std::min(equivalences(curr.label), equivalences(eqrec));
                // while(eqrec != std::min(equivalences(curr.label), equivalences(eqrec))){
                // equivalences(eqrec) = std::min(equivalences(curr.label), equivalences(eqrec));
                //   eqrec = equivalences(eqrec);
                // }
                // curr.label = std::min(equivalences(curr.label), equivalences(eqrec));
              } 
              else curr.label = cells(prevy).label;
            }
          }
          return curr;
        }
      , cells, cells
      );  
      
      std::cout << "\n";

      // std::cout << cells << "\n";

      kwk::for_each( [&](auto e) { if(e.x > -1 && e.y > -1)screen[e.x][e.y] = e.label; } , cells);

      for(auto row : screen)
      {
        for(auto p : row)
          if (int(p) > 0)std::cout << std::right << std::setw(2) << int(p) << " ";
          else std::cout << "   ";
        std::cout << "\n";
      }
      std::cout << "\n";

      std::cout << std::setw(2) << equivalences << "\n";

      //Compress label + last resolve
      int max_label = 0;
      for (int i = 0; i <= label; i++){
        if(equivalences(i) == i){
          equivalences(i) = max_label;
          max_label++;
        } else {
          auto eqv = equivalences(i);
          while(eqv != equivalences(eqv)){
            equivalences(eqv) = equivalences(equivalences(i));
            eqv = equivalences(eqv);
          }
        }
      }

      std::cout << std::setw(2) << equivalences << "\n";

      // Resolution
      kwk::transform
      ( [&](auto c)
        {
          c.label = equivalences(c.label);
          return c;
        }
      , cells, cells
      );
      
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
    }
  }
}