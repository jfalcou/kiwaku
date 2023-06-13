#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <limits>
#include <random>
#include <algorithm>
#include "../parseArg.hpp"

// Bench
#define ANKERL_NANOBENCH_IMPLEMENT
#include "../../../nanobench.h"

// CSV file
std::ofstream res_nano;

template<typename T>
std::vector<T> initializePoints(size_t size, unsigned int seed) {
  std::vector<T> points(size);
  std::mt19937 engine(seed);
  std::uniform_real_distribution<T> distribution(0.0, 10.0);

  for (size_t i = 0; i < size; ++i) {
    points[i] = distribution(engine);
  }

  return points;
}

template<typename T>
// Function to calculate the squared Euclidean distance between two points
T distance(const T& point, const T& centroid) {
    return std::abs(point - centroid);
}

template<typename T>
std::vector<int> assignToClusters(const std::vector<T>& points, const std::vector<T>& centroids) {
    std::vector<int> assignments(points.size());

    for (size_t i = 0; i < points.size(); ++i) {
        T minDistance = std::numeric_limits<T>::max();
        int clusterIndex = -1;

        for (size_t j = 0; j < centroids.size(); ++j) {
            T currDistance = distance(points[i], centroids[j]);
            if (currDistance < minDistance) {
                minDistance = currDistance;
                clusterIndex = j;
            }
        }

        assignments[i] = clusterIndex;
    }

    return assignments;
}

template<typename T>
std::vector<T> calculateCentroids(const std::vector<T>& points, const std::vector<int>& assignments, size_t k) {
    std::vector<T> centroids(k, 0);
    std::vector<size_t> clusterCounts(k, 0);

    for (size_t i = 0; i < points.size(); ++i) {
        centroids[assignments[i]] += points[i];
        clusterCounts[assignments[i]]++;
    }

    for (size_t i = 0; i < k; ++i) {
        if (clusterCounts[i] > 0) {
            centroids[i] /= clusterCounts[i];
        }
    }

    return centroids;
}

template<typename T>
std::vector<int> kMeansClustering(std::vector<T>& points, size_t k, unsigned int seed, size_t maxIterations) {
  // Sort points in ascending order
  std::sort(points.begin(), points.end());

  // Initialize centroids randomly
  std::mt19937 engine(seed);
  std::uniform_int_distribution<size_t> distribution(0, points.size() - 1);
  std::vector<T> centroids(k);
  for (size_t i = 0; i < k; ++i) {
    centroids[i] = points[distribution(engine)];
  }

  std::vector<int> assignments(points.size(), -1);
  size_t iterations = 0;

  while (iterations < maxIterations) {
    // Assign points to clusters
    std::vector<int> newAssignments = assignToClusters(points, centroids);

    // Check for convergence
    if (assignments == newAssignments) {
      break;
    }

    assignments = newAssignments;
    // Update centroids
    centroids = calculateCentroids(points, assignments, k);
    iterations++;
  }

  return assignments;
}

template<typename T>
void runBench(ParamArg p){
  res_nano.open("../results/Benchmark_std_nano_float.csv");
  res_nano << "Function;Size(Bytes);Mean Nano(Cycles);Median Nano(Cycles);Min Nano(Cycles);Max Nano(Cycles);Err Nano(Cycles)\n";

  std::vector<std::string> labels;
  std::vector<size_t> sizes;
  std::vector<ankerl::nanobench::Bench> benchs;

  // Number of clusters
  size_t maxIterations = 100;

  std::vector<T> points = initializePoints<T>(p.array_size, p.seed);

  // Initialize centroids and clusters
  std::vector<int> clusters(points.size(), -1);

  std::sort(points.begin(), points.end());

  for(long long s = 2;  s<pow(2, p.array_size); s=round(s*1.41)){
  benchs = {
    // nanobench bsearch
    ankerl::nanobench::Bench().minEpochIterations(10).epochs(p.num_times).run("Kmeans", [&]{
      for(unsigned int k = 0; k<p.array_size; k++){
        std::vector<T> centroids(k, 0.0);
        std::vector<int> assignments = kMeansClustering(points, k, p.seed, maxIterations);
      }
    })
  };

  labels = {"Kmeans"};
  sizes = {p.array_size};

  for (size_t i = 0; i < benchs.size(); ++i)
  {

    // Retrieving nanobench results
    std::vector<ankerl::nanobench::Result> vres;
    vres = benchs[i].results();
    double cyc_op_mean          = vres.begin()->average(ankerl::nanobench::Result::Measure::cpucycles);
    double cyc_op_med           = vres.begin()->median(ankerl::nanobench::Result::Measure::cpucycles);
    double cyc_op_max           = vres.begin()->minimum(ankerl::nanobench::Result::Measure::cpucycles);
    double cyc_op_min           = vres.begin()->maximum(ankerl::nanobench::Result::Measure::cpucycles);
    double cyc_op_err           = vres.begin()->medianAbsolutePercentError(ankerl::nanobench::Result::Measure::cpucycles);

    // writing measures in csv
    res_nano << labels[i] << ";"
    << sizes[i] << ";"
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
  
  std::vector<T> points = initializePoints<T>(p.array_size, p.seed);

  // Initialize centroids and clusters
  std::vector<T> centroids(k, 0.0);
  std::vector<int> clusters(points.size(), -1);

  std::vector<int> assignments = kMeansClustering(points, k, p.seed, maxIterations);
  // Print results
  for (size_t i = 0; i < points.size(); ++i) {
    std::cout << "Point " << points[i] << " belongs to Cluster " << assignments[i] << std::endl;
  }
}