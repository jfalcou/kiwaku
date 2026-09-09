# KIWAKU

C++20 collection of high performance data containers and related tools

## Getting it

```cmake
CPMAddPackage(NAME KIWAKU GIT_REPOSITORY https://github.com/jfalcou/kiwaku GIT_TAG main)
target_link_libraries(mine PRIVATE kiwaku::kiwaku)
```

Or point CMake at an installed copy:

```cmake
find_package(kiwaku REQUIRED)
target_link_libraries(mine PRIVATE kiwaku::kiwaku)
```

## Building it

```bash
cmake -S . -B build -G Ninja
cmake --build build --target kiwaku-test
ctest --test-dir build
```

`KIWAKU_BUILD_DOCUMENTATION=ON` adds the `kiwaku-doxygen` target, `KIWAKU_ENABLE_SANITIZERS=ON`
builds the tests under ASan and UBSan, and `KIWAKU_ENABLE_COVERAGE=ON` adds `kiwaku-coverage-report`.
