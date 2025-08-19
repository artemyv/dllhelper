Instead of this library - use boost https://www.boost.org/doc/libs/1_89_0/doc/html/boost_dll/tutorial.html#boost_dll.tutorial.importing_a_c_function_from_windows_dll
if you are not afraid of all its dependencies.

How to GetProcAddress like a boss 😎
======

Demonstrates how to leverage modern C++ features to simplify manual DLL linking.
Code is using c++20.

Also header is depending on [gsl library](https://github.com/microsoft/GSL.git), 
using `gsl::not_null<void*>` and `gsl::not_null<gsl::czstring>`. This dependency could be
turned off using option DLLHELPER_USE_GSL

```c++
#include <dllhelper.hpp>
#include <Windows.h>
#include <shellapi.h>
#include <iostream>
#include <system_error>

class shellAbout
{
public:
    void invoke() const
    {
        m_shellAbout(nullptr, L"hello", L"world", nullptr);
    }
private:
    static dll::Fp<decltype(ShellAboutW)> createFuncPointer()
    {
        dll::Helper a_dll{std::filesystem::path(L"Shell32.dll")};
        dll::Fp<decltype(ShellAboutW)> shellAbout{a_dll["ShellAboutW"]};
        return shellAbout;
    }

    // With new approach there is no need to store the dll::Helper object, 
    // as it is not used after the function pointer is created.
    // The function pointer will keep the module handle alive as long as it is used.
    dll::Fp<decltype(ShellAboutW)> m_shellAbout{createFuncPointer()};
};

int main()
{
    try {
        shellAbout test;
        test.invoke();
    }
    catch(const std::system_error& e) {
        std::cerr << "Err #1: " << e.what() << std::endl;
    }
}

```
See the [example_win.cpp](src/samples/example_win.cpp) file for complete example.

And now we can same in linux
```c++
#include <dllhelper.hpp>
#include <iostream>
#include <cmath>

int main()
{
    using std::filesystem::path;
    try {
        const dll::Helper a_dll{path("libm.so.6")};
        const dll::Fp<decltype(cos)> cos_func{a_dll["cos"]};

        constexpr double value = 0.0;
        const double result = cos_func(value);
        std::cout << "cos(" << value << ") = " << result << '\n';
    }
    catch(const std::invalid_argument& ex) {
        std::cerr << "Err #1: " << ex.what() << '\n';
    }
}

```
See the [example_linux.cpp](src/samples/example_linux.cpp) file for complete example.

See [blog post](https://blog.benoitblanchon.fr/getprocaddress-like-a-boss) that 
explains original idea of this library. Original code is located at 
[bblanchon / dllhelper repository](https://github.com/bblanchon/dllhelper)
