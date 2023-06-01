#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <limits>
#include <random>
#include <algorithm>

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