#include "kmeans1D.hpp"

int main() {
  // Input points
  size_t size = 20;
  unsigned int seed = 42;
  size_t maxIterations = 100;
  // Number of clusters
  size_t k = 3;

  std::vector<double> points = initializePoints<double>(size, seed);

  // Initialize centroids and clusters
  std::vector<double> centroids(k, 0.0);
  std::vector<int> clusters(points.size(), -1);

  std::sort(points.begin(), points.end());
  // Perform k-means clustering
  std::vector<int> assignments = kMeansClustering(points, k, seed, maxIterations);

    // Print results
  for (size_t i = 0; i < points.size(); ++i) {
    std::cout << "Point " << points[i] << " belongs to Cluster " << assignments[i] << std::endl;
  }

  return 0;
}