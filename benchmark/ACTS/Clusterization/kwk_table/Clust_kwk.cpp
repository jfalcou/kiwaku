#include <kwk/kwk.hpp>
#include <iostream>
#include <algorithm>

#include <boost/functional/hash.hpp>

// Bench
#define ANKERL_NANOBENCH_IMPLEMENT
#include "../../../nanobench.h"

#define N 3

namespace kwk
{
  void transform(auto func, auto& out, auto const&... in)
  {
    kwk::for_each( [&](auto i) { out(i) = func(in(i)...); }, out.shape() );
  }
}

struct cell
{
    int x,y,label;
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

  const int sizeX = (2<<(N-1));
  const int sizeY = (2<<(N-1));

  for(int size = (2<<(N+N/2)); size < (2<<(N+N/2+1)); size*=2){
    auto cells        = table{ of_size(size), as<cell> };
    auto equivalences = table{ of_size(size), as<equivalence> };

    int i = 0;
    while(i<size){
      int x = std::rand()%sizeX;
      int y = std::rand()%sizeY;
      auto b = cells.get_data();
      auto e = cells.get_data() + cells.numel();
      auto it = std::find(b, e, cell{x,y,0});
      if(it == e){
        cells(i) = {x,y,0};
        i++;
      }
    }

    std::sort(cells.get_data(), cells.get_data() + cells.numel());
    
    find_connections(cells);

    std::array<std::array<char,sizeX>,sizeY> screen = {};
    for(auto& row : screen)
      for(auto& p : row)
          p = 0;
    
    kwk::for_each( [&](auto e) { screen[e.y][e.x] = 'X'; }, cells);

    for(auto row : screen)
    {
      for(auto p : row)
          std::cout << std::right << std::setw(2) << int(p) << " ";
      std::cout << "\n";
    }
    
    int eqv = 0;
    kwk::transform
    ( [&, label = 1](auto curr) mutable
      {
        auto prevx = curr.connections[0];
        auto prevy = curr.connections[1];

        if(prevx == -1 && prevy == -1 ) { curr.label = label++; } 
        else {
          if(prevx != -1)
          {
            if(curr.label != 0) {
              equivalences(eqv++) = {std::max(curr.label,cells(prevx).label),std::min(curr.label,cells(prevx).label)};
              // while(equivalences(cells(prevx).label) != cells(prevx).label){

              // }
            }
            curr.label = cells(prevx).label;
          }          
    
          if(prevy != -1)                 
          {
            if(curr.label != 0)
            {
              equivalences(eqv++) = {std::max(curr.label,cells(prevy).label),std::min(curr.label,cells(prevy).label)};
              curr.label = std::min(cells(prevy).label, cells(prevx).label);
            } 
            else 
            {
              curr.label = cells(prevy).label;
            }
          }
        }

        
        return curr;
      }
    , cells, cells
    );  
    
    std::cout << "\n";

    std::cout << cells << "\n";

    std::sort(equivalences.get_data(), equivalences.get_data() + eqv);

    std::cout << equivalences << "\n";

    for(int i=0;i<eqv;++i)
    {
      auto e = equivalences(i);
      kwk::transform
      ( [&](auto c)
        {
          if(c.label == e.current) { c.label = e.ancestor; return c; }
          else return c;
        }
      , cells, cells
      );

      kwk::for_each( [&](auto e) { screen[e.y][e.x] = e.label; } , cells);

      for(auto row : screen)
      {
        for(auto p : row)
            std::cout << std::right << std::setw(2) << int(p)  << " ";
        std::cout << "\n";
      }
      std::cout << "\n";
    }

  }
}