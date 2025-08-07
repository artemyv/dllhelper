How to GetProcAddress like a boss 😎
======

Demonstrates how to leverage modern C++ features to simplify manual DLL linking.

```c++
#include <dllhelper.hpp>
#include <windows.h>
#include <shellapi.h>
#include <iostream>


int main() {
	using std::filesystem::path;
	try
	{
		DllHelper m_dll{path(L"Shell32.dll")};
		decltype(ShellAboutW)* shellAbout = m_dll["ShellAboutW"];

		shellAbout(NULL, L"hello", L"world", NULL);
	}
	catch(const std::runtime_error& e)
	{
		std::cerr << "Failed: " << e.what() << std::endl;
	}
}
```

And now we can same in linux
```c++
int main() {
    using std::filesystem::path;
    try
    {
        DllHelper m_dll{path("libm.so.6")};
        using cos_func_t = decltype(cos);
        cos_func_t* cos_func = m_dll["cos"];

        double value = 0.0;
        double result = cos_func(value);
        std::cout << "cos(" << value << ") = " << result << std::endl;
    } 
    catch(const std::runtime_error& ex)
    {
        std::cerr << "Failed: " << ex.what() << std::endl;
    }
}
```

See [blog post](https://blog.benoitblanchon.fr/getprocaddress-like-a-boss)