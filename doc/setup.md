# Installation and Quick Start {#setup}

## Prerequisites

**KIWAKU** requires a C++20 compliant compiler. Below are the minimum supported versions:

| Compiler | Version |
| :--- | :--- |
| **g++** | 13 or above |
| **clang++** | 19 or above |
| **MSVC** | 19.38 (VS 2022 17.8) or above |

## Retrieving the Source

### GitHub
Clone the repository to get the latest stable version from the `main` branch:

```bash
git clone https://github.com/jfalcou/kiwaku.git
```

**Note:** Development happens live on `main`. For production environments, we recommend checking out a specific [tagged version](https://github.com/jfalcou/kiwaku/releases).

### CPM (CMake Package Manager)

If you use [CPM](https://github.com/cpm-cmake/CPM.cmake), add the following to your `CMakeLists.txt`:

```cmake
include(CPM)

CPMAddPackage( NAME kiwaku
               GITHUB_REPOSITORY jfalcou/kiwaku
               GIT_TAG main
             )
```

## Installation from Source

If you are not using a package manager, use the CMake build system. We recommend [Ninja](https://ninja-build.org/) for faster builds.

### Setting up the Library

Use the following commands to configure and build the project. Modern CMake allows you to do this without manually changing directories:

```bash
# Configure the build
cmake -S . -B build -G Ninja -DCMAKE_INSTALL_PREFIX=/path/to/install

# Install the library
cmake --build build --target install
```

On Linux, the default is `/usr/local` (requires `sudo`), use `-DCMAKE_INSTALL_PREFIX` to specify a local installation folder.

### Building the Documentation

If you have **Doxygen** installed, you can generate the HTML documentation:

```bash
cmake --build build --target kiwaku-doxygen
```

The resulting files will be available in the `doc` folder.

## Using the Library

### Compilation

Once installed, you can verify your setup with a simple test file (e.g., `test.cpp`):

```cpp
#include <kwk/kwk.hpp>
#include <iostream>

int main()
{
  auto m = kwk::matrix( kwk::of_size(4, 3), [](auto i, auto j)
  {
    return 1 + j + i * 3;
  });

  std::cout << "Test matrix:\n" << m << std::endl;
}
```

To compile using **g++**:

```bash
g++ test.cpp -std=c++20 -O3 -DNDEBUG -I/path/to/install/include -o output
```

### Execution

Run the binary to check the results:

```bash
$ ./output
Test matrix:
  [ 1 2 3 ]
  [ 4 5 6 ]
  [ 7 8 9 ]
  [ 10 11 12 ]
```

**KIWAKU** is now properly installed and ready for use.
