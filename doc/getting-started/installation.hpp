#error This file is for documentation only - DO NOT INCLUDE
/**

@page kwk_installation Installation

KIWAKU is a **header-only** library with **no required runtime dependency**. Installing it is
making the headers reachable from your compiler; there is nothing to build or link for the
library itself. The dependency is a single header-only companion library, **Kumi** (a C++20
tuple library); for the unit tests, **TTS** is used as well, but neither is needed at runtime.

@section kwk_installation_requirements Requirements

- A **C++23-compatible compiler** (or at least C++20 feature-complete — the installed package
  is configured with `cxx_std_20`). The KIWAKU test suite is built with
  `-Wall -Wextra -Werror -Wdocumentation`.
- **CMake 3.24 or newer** to drive the configuration, whether you consume KIWAKU with
  <a href="https://github.com/cpm-cmake/CPM.cmake">CPM</a>, an installed package, or from the
  sources.

@section kwk_installation_cpm With CPM

The simplest way to pull KIWAKU into a CMake project is
[CPM.cmake](https://github.com/cpm-cmake/CPM.cmake). Add it to your top-level `CMakeLists.txt` and
then request the package:

```cmake
CPMAddPackage(NAME KIWAKU GIT_REPOSITORY https://github.com/jfalcou/kiwaku GIT_TAG main)
```

Once fetched, link against the package target `kiwaku::kiwaku`; it brings the KIWAKU headers
(and the headers of its companion, Kumi) into your include path:

```cmake
target_link_libraries(my_app PRIVATE kiwaku::kiwaku)
```

Then include KIWAKU with:

```cpp
#include <kiwaku/kiwaku.hpp>
```

@section kwk_installation_cmake With an installed package

Install KIWAKU to a prefix using CMake's install flow (headers plus a CMake package config are
produced by the setup script):

```sh
cmake -S . -B build -DKIWAKU_BUILD_TEST=OFF
cmake --install build --prefix /path/to/kiwaku-prefix
```

Then consume it from another project with `find_package` and link the target it exports
(`kiwaku::kiwaku`; the variable `KIWAKU_LIBRARIES` is also defined):

```cmake
find_package(kiwaku REQUIRED)
target_link_libraries(my_app PRIVATE kiwaku::kiwaku)
```

@section kwk_installation_standalone From the sources

To work on KIWAKU itself or to build the test suite and/or Doxygen:

```sh
cmake -S . -B build -DKIWAKU_BUILD_TEST=ON -DKIWAKU_BUILD_DOCUMENTATION=ON
cmake --build build
```

Useful options:

- `KIWAKU_BUILD_TEST` (default `ON`) — build the unit-test suite.
- `KIWAKU_BUILD_DOCUMENTATION` (default `OFF`) — generate the Doxygen site via the
  `kiwaku-doxygen` target into `build/doc`.
- `KIWAKU_ENABLE_SANITIZERS` (default `OFF`) — build tests with AddressSanitizer and
  UndefinedBehaviorSanitizer.
- `KIWAKU_ENABLE_COVERAGE` (default `OFF`) — build tests with code-coverage instrumentation.
- `KIWAKU_QUIET` — silence the configure-time option summary.

The public umbrella header is `include/kiwaku/kiwaku.hpp`; the architecture and tutorial pages
refer to the individual headers they rely on.

**/
