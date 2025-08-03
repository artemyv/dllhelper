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
