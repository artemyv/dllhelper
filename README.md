How to GetProcAddress like a boss 😎
======

Demonstrates how to leverage modern C++ features to simplify manual DLL linking.

```c++
#include <dllhelper.hpp>
#include <windows.h>
#include <shellapi.h>
#include <iostream>
class ShellApi {
  DllHelper m_dll{L"Shell32.dll"};

public:
  operator bool() const noexcept {return m_dll && shellAbout != nullptr;}
  decltype(ShellAboutW) *shellAbout = m_dll["ShellAboutW"];
  std::error_code error_code() const noexcept { return m_dll.error_code(); }
};

int main() {
  ShellApi shellApi;
  if(shellApi) {
      shellApi.shellAbout(NULL, L"hello", L"world", NULL);
  }
  else {
	  std::cerr << "Failed: " << shellApi.error_code().message() << std::endl;
  }
}
```

And now we can same in linux
```c++
#include <dllhelper.hpp>
#include <iostream>
#include <cmath>

class MathLib {
    DllHelper m_dll{"libm.so.6"};

public:
    // Function pointer type for cos(double)
    using cos_func_t =decltype(cos);
    cos_func_t* cos_func = m_dll["cos"];

    constexpr operator bool() const noexcept { return m_dll && cos_func != nullptr; }
    std::string error_message() const {
        return m_dll.error_message();
	}
};

int main() {
    MathLib mathLib;
    if (mathLib) {
        double value = 0.0;
        double result = mathLib.cos_func(value);
        std::cout << "cos(" << value << ") = " << result << std::endl;
    } else {
        std::cerr << "Failed: " << mathLib.error_message() << std::endl;
    }
}

```

See [blog post](https://blog.benoitblanchon.fr/getprocaddress-like-a-boss)