# vart.dllhelper: How to GetProcAddress like a boss 😎

<!--
SPDX-License-Identifier: MIT
-->

<!-- markdownlint-disable-next-line line-length -->
![Continuous Integration Tests](https://github.com/artemyv/dllhelper/actions/workflows/ci_tests.yml/badge.svg) ![Lint Check (pre-commit)](https://github.com/artemyv/dllhelper/actions/workflows/pre-commit-check.yml/badge.svg) [![Coverage](https://coveralls.io/repos/github/artemyv/dllhelper/badge.svg?branch=main)](https://coveralls.io/github/artemyv/dllhelper?branch=main)


`vart.dllhelper` is a minimal C++ library attempting to follow
[The Beman Standard](https://github.com/bemanproject/beman/blob/main/docs/beman_standard.md).
It strives to achieve a clean, modern C++ project structure.

**Demonstrates**: how to leverage modern C++ features to simplify manual dynamic libraries loading in run-time. Uses C++20 features.

Instead of this library, you can use
[Boost.DLL](https://www.boost.org/doc/libs/latest/doc/html/boost_dll/tutorial.html#boost_dll.tutorial.importing_a_c_function_from_windows_dll)
if you’re comfortable with its dependency footprint.

## License

`vart.dllhelper` is licensed under the MIT License.

## Usage

Library header depends on the [GSL library](https://github.com/microsoft/GSL.git),
using `gsl::not_null<void*>` and `gsl::not_null<gsl::czstring>`. This dependency is
controlled via the CMake option `DLLHELPER_USE_GSL`.

### Usage: Windows example calling ShellAboutW function from Shell32.dll

```c++
#include <vart/dllhelper/dllhelper.h>
#include <Windows.h>
#include <shellapi.h>
#include <iostream>
#include <system_error>

class shellAbout {
  public:
    void invoke() const { m_shellAbout(nullptr, L"hello", L"world", nullptr); }

  private:
    static dll::Fp<decltype(ShellAboutW)> createFuncPointer() {
        dll::Helper                    a_dll{std::filesystem::path(L"Shell32.dll")};
        dll::Fp<decltype(ShellAboutW)> shellAbout{a_dll["ShellAboutW"]};
        return shellAbout;
    }

    dll::Fp<decltype(ShellAboutW)> m_shellAbout{createFuncPointer()};
};

int main() {
    try {
        shellAbout test;
        test.invoke();
    } catch (const std::system_error& e) {
        std::cerr << "Err #1: " << e.what() << std::endl;
    }
}

```
See the [win.cpp](examples/win.cpp) file for complete example.

### Usage: Linux example calling double std::cos(double) function from libm.so.6

```c++
#include <vart/dllhelper/dllhelper.h>
#include <iostream>

int main() {
    using std::filesystem::path;
    try {
        const dll::Helper             a_dll{path("libm.so.6")};
        const dll::Fp<double(double)> cos_func{a_dll["cos"]};

        constexpr double value  = 0.0;
        const double     result = cos_func(value);
        std::cout << "cos(" << value << ") = " << result << '\n';
    } catch (const std::invalid_argument& ex) {
        std::cerr << "Err #1: " << ex.what() << '\n';
    }

    try {
        const dll::Helper a_dll{path("libm.so.125")};
    } catch (const std::invalid_argument& ex) {
        std::cerr << "Err #2: " << ex.what() << '\n';
    }
}
```
See the [linux.cpp](examples/linux.cpp) file for complete example.

## Reference

See [blog post](https://blog.benoitblanchon.fr/getprocaddress-like-a-boss) that
explains original idea of this library. Original code is located at
[bblanchon / dllhelper repository](https://github.com/bblanchon/dllhelper)

## Dependencies

### Build Environment

This project requires at least the following to build:

* A C++ compiler that conforms to the C++20 standard or greater
* CMake 3.28 or later
* (Test Only) GoogleTest
* Optional GSL library

You can disable building tests by setting CMake option
`VART_DLLHELPER_BUILD_TESTS` to `OFF` when configuring the project.

### Supported Platforms

This project officially supports:

* GCC versions 11–15
* LLVM Clang++ (with libstdc++ or libc++) versions 17–21
* MSVC version 19.44.35215.0 (i.e., the [latest version on GitHub-hosted Windows runners](https://github.com/actions/runner-images/blob/main/images/windows/Windows2022-Readme.md))

> [!NOTE]
>
> Versions outside of this range would likely work as well,
> especially if you're using a version above the given range
> (e.g. HEAD/ nightly).
> These development environments are verified using our CI configuration.

### Configure and Build the Project Using CMake Presets

This project recommends using [CMake Presets](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html)
to configure, build and test the project.
Appropriate presets for major compilers have been included by default.
You can use `cmake --list-presets` to see all available presets.

Here is an example to invoke the `gcc-debug` preset.

```shell
cmake --workflow --preset gcc-debug
```

Generally, there are two kinds of presets, `debug` and `release`.

The `debug` presets are designed to aid development, so it has debugging
instrumentation enabled and many sanitizers enabled.

> [!NOTE]
>
> The sanitizers that are enabled vary from compiler to compiler.
> See the toolchain files under ([`infra/cmake`](infra/cmake/)) to determine the exact configuration used for each preset.

The `release` presets are designed for production use, and
consequently have the highest optimization turned on (e.g. `O3`).
