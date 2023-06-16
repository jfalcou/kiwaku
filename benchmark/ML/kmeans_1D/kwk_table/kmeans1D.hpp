#pragma once
#include <cstddef>
#include <iostream>
#include <vector>
#include <random>
#include <limits>
#include <random>
#include <algorithm>
#include "../parseArg.hpp"
#include "kwk/settings/source.hpp"
#include "kwk/settings/type.hpp"
#include "kwk/utility/container/shape.hpp"
#include <kwk/kwk.hpp>

// Bench
#define ANKERL_NANOBENCH_IMPLEMENT
#include "../../../nanobench.h"

// CSV file
std::ofstream res_nano;

template<typename T>
auto initializePoints(size_t size, unsigned int seed) {
  std::vector<T> points(size);
  auto KWKpoints = kwk::table{ kwk::source = points, kwk::of_size(size) };

  std::mt19937 engine(seed);
  std::uniform_real_distribution<T> distribution(0.0, 100.0);

  for (size_t i = 0; i < size; ++i) {
    KWKpoints(i) = distribution(engine);
  }

  return KWKpoints;
}

template<typename T>
// Function to calculate the squared Euclidean distance between two points
T distance(auto& point, auto& centroid) {
  // Modify for kwk
  return std::abs(point - centroid);
}

template<typename T>
auto assignToClusters(auto& points, const auto& centroids) {
  std::vector<int> assignments(points.numel());
  auto KWKassignments = kwk::table{ kwk::source = assignments, kwk::of_size(points.numel()) };

  for (int i = 0; i < points.numel(); ++i) {
    T minDistance = std::numeric_limits<T>::max();
    int clusterIndex = -1;

    for (int j = 0; j < centroids.numel(); ++j) {
      T currDistance = distance<T>(points(i), centroids(j));
      if (currDistance < minDistance) {
        minDistance = currDistance;
        clusterIndex = j;
      }
    }

    KWKassignments(i) = clusterIndex;
  }

  return KWKassignments;
}

template<typename T>
auto calculateCentroids(auto& points, auto& assignments, size_t k) {
  std::vector<size_t> clusterCounts(k, 0);
  std::vector<T> centroids(k, 0);

  auto KWKcentroids = kwk::table{ kwk::source = centroids, kwk::of_size(k)};
  auto KWKclusterCounts = kwk::table{ kwk::source =clusterCounts, kwk::of_size(k) };

  for (int i = 0; i < points.numel(); ++i) {
    KWKcentroids(assignments(i)) += points(i);
    KWKclusterCounts(assignments(i))++;
  }

  for (size_t i = 0; i < k; ++i) {
    if (KWKclusterCounts(i) > 0) {
      KWKcentroids(i) /= KWKclusterCounts(i);
    }
  }

  return KWKcentroids;
}

template<typename T>
auto kMeansClustering(auto& points, size_t k, unsigned int seed, size_t maxIterations) {
  // Sort points in ascending order
  std::sort(points.get_data(), points.get_data() + points.numel());

  // Initialize centroids randomly
  std::mt19937 engine(seed);
  std::uniform_int_distribution<size_t> distribution(0, points.numel() - 1);

  std::vector<T> centroids(k);
  auto KWKcentroids = kwk::table{ kwk::source = centroids, kwk::of_size(k)};

  for (size_t i = 0; i < k; ++i) {
    KWKcentroids(i) = points(distribution(engine));
  }

  std::vector<int> assignments(points.numel(), -1);
  auto KWKassignments = kwk::table{ kwk::source = assignments, kwk::of_size(points.numel()) };

  size_t iterations = 0;

  while (iterations < maxIterations) {
    // Assign points to clusters
    auto newAssignments = assignToClusters<T>(points, KWKcentroids);

    // Check for convergence
    if (std::equal(KWKassignments.get_data(), KWKassignments.get_data() + KWKassignments.numel(), newAssignments.get_data())) {
      break;
    }

    KWKassignments = newAssignments;
    // Update centroids
    KWKcentroids = calculateCentroids<T>(points, KWKassignments, k);
    iterations++;
  }

  return KWKassignments;
}

template<typename T>
void runBench(ParamArg p){
  std::string fname = "../results/Benchmark_std_nano_float_" + std::to_string(p.array_size) + ".csv";
  res_nano.open(fname);
  res_nano << "Function;Array Size;Clusters;Mean Nano(Cycles);Median Nano(Cycles);Min Nano(Cycles);Max Nano(Cycles);Err Nano(%)\n";

  std::vector<std::string> labels;
  std::vector<size_t> sizes;
  std::vector<ankerl::nanobench::Bench> benchs;

  // Number of clusters
  size_t maxIterations = 100;

  labels = {"Kmeans"};
  sizes = {p.array_size};

  auto points = initializePoints<T>(p.array_size, p.seed);
  std::sort(points.get_data(), points.get_data() + points.numel());

  for(unsigned int k = 0; k<=p.array_size; k++){
    std::string ss = "Kmeans " + std::to_string(k) + " / " + std::to_string(p.array_size);
    benchs = {
      // nanobench bsearch
      ankerl::nanobench::Bench().minEpochIterations(10).epochs(p.num_times).run(ss, [&]{
        auto assignments = kMeansClustering<T>(points, k, p.seed, maxIterations);
      })
    };

    for (size_t i = 0; i < benchs.size(); ++i)
    {
      // Retrieving nanobench results
      std::vector<ankerl::nanobench::Result> vres;
      vres = benchs[i].results();
      double cyc_op_mean          = vres.begin()->average(ankerl::nanobench::Result::Measure::cpucycles);
      double cyc_op_med           = vres.begin()->median(ankerl::nanobench::Result::Measure::cpucycles);
      double cyc_op_min           = vres.begin()->minimum(ankerl::nanobench::Result::Measure::cpucycles);
      double cyc_op_max           = vres.begin()->maximum(ankerl::nanobench::Result::Measure::cpucycles);
      double cyc_op_err           = vres.begin()->medianAbsolutePercentError(ankerl::nanobench::Result::Measure::cpucycles);

      // writing measures in csv
      res_nano << labels[i] << ";"
      << sizes[i] << ";"
      << k << ";"
      << cyc_op_mean << ";"
      << cyc_op_med << ";"
      << cyc_op_min << ";"
      << cyc_op_max << ";"
      << cyc_op_err << "\n";
    }
  }
  res_nano.close();
}

template<typename T>
void runTest(ParamArg p){
  // Number of clusters
  size_t maxIterations = 100;
  size_t k = 3;

  auto points = initializePoints<T>(p.array_size, p.seed);
  std::sort(points.get_data(), points.get_data() + points.numel());

  auto assignments = kMeansClustering<T>(points, k, p.seed, maxIterations);
  // Print results
  for (int i = 0; i < points.numel(); ++i) {
    std::cout << "Point " << points(i) << " belongs to Cluster " << assignments(i) << std::endl;
  }
}
