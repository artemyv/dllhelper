How to GetProcAddress like a boss 😎
======

Demonstrates how to leverage modern C++ features to simplify manual DLL linking.

```c++
#include "dllhelper.hpp"
#include <shellapi.h>

class ShellApi {
  DllHelper m_dll{L"Shell32.dll"};

public:
  operator bool() const noexcept {return m_dll && shellAbout != nullptr;}
  decltype(ShellAboutW) *shellAbout = m_dll["ShellAboutW"];
};

int main() {
  ShellApi shellApi;
  if(shellApi)
    shellApi.shellAbout(NULL, L"hello", L"world", NULL);
}
```

See [blog post](https://blog.benoitblanchon.fr/getprocaddress-like-a-boss)