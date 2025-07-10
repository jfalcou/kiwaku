#include <iostream> 
#include <vector> 
#include <algorithm> 
#include <iterator> 

// clang++ test_iter.cpp -o e -O3 -Wall -Wextra && ./e

int main() { 
    std::vector<int> array1 = {43, 11, 482}; 
    std::vector<std::string> array2 = {"one", "two", "three"}; 
 
    // Ensure both containers have the same size 
    if (array1.size() != array2.size()) { 
        std::cerr << "Arrays must be of the same size!" << std::endl; 
        return 1; 
    } 
 
    auto start = &array1[0];
    auto fct = [&](auto const& e)
    {
      std::size_t index = &e - start;
      std::cout << "a1(" << e << ") a2(" << array2[index] << ")  i(" << index << ")\n";
    };
    // Using std::for_each with iterators 
    std::for_each(array1.begin(), array1.end(),  
    fct); 
                  // [&](const int& num) { 
                  //     std::size_t index = &num - &array1[0]; // Get index of current element 
                  //     std::cout << num << ": " << array2[index] << std::endl; 
                  // }
 
    return 0; 
}
