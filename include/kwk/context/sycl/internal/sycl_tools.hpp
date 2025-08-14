#pragma once

#include <kwk/concepts/container.hpp>
#include <kwk/utility/position.hpp>
#include <vector>
#include <iostream>

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
      if (index != 0)
      {
        std::cout << __FILE__ << " @ " << __LINE__
                  << " !!!!!!!!!!!!!!!!! ERROR: index != 0  (equals " << index << "\n";
      } 
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

  template<concepts::container Container>
  int pos_to_linear(Container const& container, std::optional<kwk::position<Container::static_order>> const& pos)
  {
    if (! pos.has_value()) return -1;

    kwk::position<Container::static_order> real_pos = pos.value();    

    int res = 0;
    auto shp = container.shape();

    for (int dim = 0; dim < shp.nbdims(); ++dim)
    {
      int multiply_by = 1;
      for (int dim2 = dim + 1; dim2 < shp.nbdims(); ++dim2)
      {
        multiply_by *= shp[dim2];
      }
      res += real_pos[dim] * multiply_by;
    }
    return res;
  }
}
