Installation and Quick Start {#about}
============================
Installation & Quick Start {#install}
==========================

# Pre-requisites

**KIWAKU** requires a C++20 compliant compiler. Here is the current minimal compiler version supported:

| Compiler       | Version        |
| -------------- | -------------- |
| g++            | 11  or above   |
| clang++        | 14  or above   |

<br/>
# Retrieving the source

## Github
**KIWAKU** is available on GitHub and can be retrieved via the following command:
<br/>

@verbatim
$ git clone  https://github.com/jfalcou/kiwaku.git
@endverbatim

This retrieves the `main` branch which contains the latest stable version. Development happens
live on `main` so if you need stability, use a tagged versions.

## CPM
You can install **KIWAKU** directly via [CPM](https://github.com/cpm-cmake/CPM.cmake). After
[adding CPM to your CMake setup](https://github.com/cpm-cmake/CPM.cmake#adding-cpm), just
add the following commands:

```cmake
include(CPM)

CPMAddPackage(
        NAME eve
        URL https://github.com/jfalcou/kiwaku
)
```

# Installation from Source
If you didn't fetched **KIWAKU** from a package manager, you'll need to install it via our CMake
system.

## Setting up the Library
Create a `build` directory here and enter it. Once in the `build` directory, you can use  **CMake**
to generate the build system for **KIWAKU**.

We recommend using [Ninja](https://ninja-build.org/) but any build system is fine.

@verbatim
$ mkdir build
$ cd build
$ cmake .. -G Ninja
@endverbatim

Once **CMake** completes, you can use the `install` target to build and install **KIWAKU**. By default,
the library will be installed in the `/usr/local`directory, thus requiring root privileges on Linux.

@verbatim
$ ninja install
@endverbatim

You can select an alternative installation path by specifying the `CMAKE_INSTALL_PREFIX` option at configuration time.

@verbatim
$ cmake .. -G Ninja -DCMAKE_INSTALL_PREFIX=path/to/install
$ ninja install
@endverbatim

## Building the Documentation
You can rebuild **KIWAKU** documentation if you have the latest version of Doxygen installed
using the `doxygen` target:
<br/>

@code
ninja kwk-doxygen
@endcode

The resulting HTML files will be available in the `doc` folder.

# Using the library
## Compilation

Once installed, you can compile the following code to check if everything is alright.

@godbolt{examples/quick-start/sanity-check.cpp}

To do so, use your C++20 aware favorite compiler, for example g++.

@verbatim
$ g++ test.cpp -std=c++20  -march=native -O3 -DNDEBUG -I/path/to/install/include -o output
@endverbatim

Don't forget the `--std=c++20` option to be sure to activate C++20 support. If you use a different compiler, check your compiler user's manual to use the proper option.

You can notice we use the `-O3 -DNEDBUG` options. This is required if you want to be sure to get the best performance out of **KIWAKU**.

The `-DNDEBUG` setting can be omitted but then asserts will be inserted into the code to prevent logic errors.

## Execution

Once done, execute the binary. If you compiled for SSE4.1 for example, you'll end up with the
following results:

@verbatim
$ ./output
@endverbatim

That's it, **KIWAKU** is properly installed and ready to use.

# Advanced options
