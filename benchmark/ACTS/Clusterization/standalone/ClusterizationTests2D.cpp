// This file is part of the Acts project.
//
// Copyright (C) 2022 CERN for the benefit of the Acts project
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <cstddef>
#define BOOST_TEST_DYN_LINK

#include <boost/test/data/test_case.hpp>
#include <boost/test/unit_test.hpp>

#include "Clusterization.hpp"

#include <array>
#include <random>
#include <vector>
#include <fstream>

#include <boost/functional/hash.hpp>

// Bench
#define ANKERL_NANOBENCH_IMPLEMENT
#include "../../../nanobench.h"

// CSV file
std::ofstream res_nano;
// std::ofstream res_test;

using Rectangle = std::array<int, 4>;

std::vector<Rectangle> concat(std::vector<std::vector<Rectangle>> vecs) {
  std::vector<Rectangle> flat;
  for (std::vector<Rectangle>& v : vecs) {
    if (flat.empty()) {
      flat = std::move(v);
    } else {
      flat.reserve(flat.size() + v.size());
      std::move(v.begin(), v.end(), std::back_inserter(flat));
      v.clear();
    }
  }
  return flat;
}

template <typename RNG>
std::vector<Rectangle> segment(int x0, int y0, int x1, int y1, RNG& rng) {
  // Leave enough space for one-cell buffer around clusters
  int xmin = x0 + 3;
  int xmax = x1 - 3;
  int ymin = y0 + 3;
  int ymax = y1 - 3;

  // terminal case 1
  if (xmax < xmin or ymax < ymin) {
    return {{x0, y0, x1, y1}};
  }

  std::bernoulli_distribution cointoss;
  bool splitx = cointoss(rng);
  bool splity = cointoss(rng);

  // terminal case 2
  if (not(splitx or splity)) {
    return {{x0, y0, x1, y1}};
  }

  int x_ = std::uniform_int_distribution(xmin, xmax)(rng);
  int y_ = std::uniform_int_distribution(ymin, ymax)(rng);

  if (splitx and not splity) {
    return concat({segment(x0, y0, x_, y1, rng), segment(x_, y0, x1, y1, rng)});
  } else if (not splitx and splity) {
    return concat({segment(x0, y0, x1, y_, rng), segment(x0, y_, x1, y1, rng)});
  } else if (splitx and splity) {
    return concat({segment(x0, y0, x_, y_, rng), segment(x_, y0, x1, y_, rng),
                   segment(x0, y_, x_, y1, rng), segment(x_, y_, x1, y1, rng)});
  }
  throw std::runtime_error("unreachable");
}

struct Cell2D {
  Cell2D(int rowv, int colv) : row(rowv), col(colv) {}
  int row, col;
  Label label{NO_LABEL};
};

int getCellRow(const Cell2D& cell) {
  return cell.row;
}

int getCellColumn(const Cell2D& cell) {
  return cell.col;
}

Label& getCellLabel(Cell2D& cell) {
  return cell.label;
}

bool operator==(const Cell2D& left, const Cell2D& right) {
  return left.row == right.row and left.col == right.col;
}

bool cellComp(const Cell2D& left, const Cell2D& right) {
  return (left.row == right.row) ? left.col < right.col : left.row < right.row;
}

struct Cluster2D {
  std::vector<Cell2D> cells;
  size_t hash{0};
};

void clusterAddCell(Cluster2D& cl, const Cell2D& cell) {
  cl.cells.push_back(cell);
}

void hash(Cluster2D& cl) {
  std::sort(cl.cells.begin(), cl.cells.end(), cellComp);
  cl.hash = 0;
  for (const Cell2D& c : cl.cells) {
    boost::hash_combine(cl.hash, c.col);
  }
}

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

bool clHashComp(const Cluster2D& left, const Cluster2D& right) {
  return left.hash < right.hash;
}

int main(int argc, char *argv[]) {
  using Cell = Cell2D;
  using CellC = std::vector<Cell>;
  using Cluster = Cluster2D;
  using ClusterC = std::vector<Cluster>;

  ankerl::nanobench::Bench bench;
  // Retrieving nanobench results
  std::vector<ankerl::nanobench::Result> vres;
  ClusterC newCls;

  size_t startSeed = 71902647;
  std::mt19937_64 rnd(startSeed);

  int size = std::atoi(argv[1]);

  if(argc > 1) 
  {
    std::string fname = "./Benchmark_ccl_std_" + std::to_string(size) + ".csv";
    res_nano.open(fname);
    res_nano << "Size(N*N);Density(1/1000);Cells(Nb);Cycles/cell;Mean Nano(Cycles);Median Nano(Cycles);Min Nano(Cycles);Max Nano(Cycles);Err Nano(Cycles)\n";
  }
  else
  {
    std::cout << "Size input needed";
    exit(EXIT_FAILURE);
  }

  std::cout << "Grid_2D_rand test with parameters: " << std::endl;
  std::cout << " startSeed = " << startSeed << std::endl;

  // std::string f_test = "./Test_verif_" + std::to_string(size) + ".csv";
  // res_test.open(f_test);

  for (int density = 10; density <= 1000; density += 10) {
    std::vector<Cluster> cls;
    std::vector<Cell> cells;

    std::vector<std::vector<int>> arr(size, std::vector<int>(size, 0));
    fill_array(arr, size, density, startSeed);
    std::string ss;
    ss = "CCL " + std::to_string(size) + " : " + std::to_string(density);

    uint32_t nb_cells = 0;
    for (int i = 0; i < size; ++i) {
      for (int j = 0; j < size; ++j) {
        if(arr[i][j] == 1)
        {
          Cell2D c(i, j); 
          cells.push_back(c);
          nb_cells++;
        }
      }
    }

    
    // nanobench CCL
    bench = 
    ankerl::nanobench::Bench().minEpochIterations(1).epochs(1).run(ss, [&]{
    ankerl::nanobench::doNotOptimizeAway(newCls);

    std::shuffle(cells.begin(), cells.end(), rnd);
    newCls = createClusters<CellC, ClusterC>(cells);
    });
    
    // for (auto it = newCls.begin(); it != newCls.end(); ++it){
    //   // std::sort(it->cells.begin(), it->cells.end());
    //   for (auto c = it->cells.begin(); c != it->cells.end(); ++c){
    //     res_test << c->row << "," << c->col << "," << c->label << "\n";
    //   }
    // }
    // res_test << "\n" ;

    vres = bench.results();
    double cyc_op_mean          =   vres.begin()->average(ankerl::nanobench::Result::Measure::cpucycles);
    double cyc_op_med           =   vres.begin()->median(ankerl::nanobench::Result::Measure::cpucycles);
    double cyc_op_min           =   vres.begin()->minimum(ankerl::nanobench::Result::Measure::cpucycles);
    double cyc_op_max           =   vres.begin()->maximum(ankerl::nanobench::Result::Measure::cpucycles);
    double cyc_op_err           =   vres.begin()->medianAbsolutePercentError(ankerl::nanobench::Result::Measure::cpucycles) ;

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
  // res_test.close();
}

