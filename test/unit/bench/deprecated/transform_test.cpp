#include <iostream>
#include <vector>
#include <algorithm> // For std::transform

// clang++ transform_test.cpp -o e -O3 -Wall -Wextra -std=c++20 && ./e
int main() {
    std::vector<int> input_vec = {1, 2, 3, 4, 5};

    // The output vector must be large enough to hold the result.
    std::vector<int> output_vec(input_vec.size());

    // std::transform(input_start, input_end, output_start, operation)
    std::transform(input_vec.begin(), input_vec.end(), output_vec.begin(),
                   [](int n) { return n * 10; });

    std::cout << "The results were written to the output vector:" << std::endl;
    for (int n : output_vec) {
        std::cout << n << " ";
    }
    std::cout << std::endl;

    return 0;
}