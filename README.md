How to GetProcAddress like a boss 😎
======

Demonstrates how to leverage modern C++ features to simplify manual DLL linking.
Code requires using c++20 (for std::bit_cast and std::format features)

Possible modifications - is to move bitcast to the library code making the header 
C++17 friendly. Library itself should still be compiled using c++20.

Also header is depending on [gsl library](https://github.com/microsoft/GSL.git), 
using `gsl::not_null<void*>` and `gsl::not_null<gsl::czstring>`. First can be 
replaced with `void*` and second one with `const char*` if this dependency is not 
acceptable.

```c++
#include <dllhelper.hpp>
#include <windows.h>
#include <shellapi.h>
#include <iostream>

class shellAbout
{
public:
	void invoke()
	{
		m_shellAbout(nullptr, L"hello", L"world", nullptr);
	}
private:
	static dll::Fp<decltype(ShellAboutW)> createFuncPointer()
	{
		dll::Helper a_dll{std::filesystem::path(L"Shell32.dll")};
		dll::Fp<decltype(ShellAboutW)> shellAbout = a_dll["ShellAboutW"];
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
	catch(const std::runtime_error& e) {
		std::cerr << "Failed: " << e.what() << std::endl;
	}
}
```
See the [example_win.cpp](src/example_win.cpp) file for complete example.

And now we can same in linux
```c++
#include <dllhelper.hpp>
#include <iostream>
#include <cmath>

int main()
{
    using std::filesystem::path;
    try {
        dll::Helper a_dll{path("libm.so.6")};
        dll::Fp<decltype(cos)> cos_func = a_dll["cos"];

        double value = 0.0;
        double result = cos_func(value);
        std::cout << "cos(" << value << ") = " << result << std::endl;
    }
    catch(const std::runtime_error& ex) {
        std::cerr << "Failed: " << ex.what() << std::endl;
    }
}
```
See the [example_linux.cpp](src/example_linux.cpp) file for complete example.

See [blog post](https://blog.benoitblanchon.fr/getprocaddress-like-a-boss)