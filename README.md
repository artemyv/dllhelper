<!-- markdownlint-disable-next-line line-length -->
![Continuous Integration Tests](https://github.com/artemyv/dllhelper/actions/workflows/ci_tests.yml/badge.svg) ![Lint Check (pre-commit)](https://github.com/artemyv/dllhelper/actions/workflows/pre-commit-check.yml/badge.svg) [![Coverage](https://coveralls.io/repos/github/artemyv/dllhelper/badge.svg?branch=main)](https://coveralls.io/github/artemyv/dllhelper?branch=main) 


`vart.dllhelper` is a minimal C++ library attempting to follow
[The Beman Standard](https://github.com/bemanproject/beman/blob/main/docs/beman_standard.md).
It strives to achieve a clean, modern C++ project structure.

**Demonstrates**: how to leverage modern C++ features to simplify manual dynamic libraries loading in run-time. Uses C++20 features.

Instead of this library, you can use
[Boost.DLL](https://www.boost.org/doc/libs/latest/doc/html/boost_dll/tutorial.html#boost_dll.tutorial.importing_a_c_function_from_windows_dll)
if you’re comfortable with its dependency footprint.

**How to GetProcAddress like a boss** 😎

Library header depends on the [GSL library](https://github.com/microsoft/GSL.git),
using `gsl::not_null<void*>` and `gsl::not_null<gsl::czstring>`. This dependency is
controlled via the CMake option `DLLHELPER_USE_GSL`.

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

And now the same on Linux
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

See [blog post](https://blog.benoitblanchon.fr/getprocaddress-like-a-boss) that
explains original idea of this library. Original code is located at
[bblanchon / dllhelper repository](https://github.com/bblanchon/dllhelper)
