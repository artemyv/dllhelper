// SPDX-License-Identifier: MIT
#include <Windows.h>
#include <iostream>
#include <shellapi.h>
#include <vart/dllhelper/dllhelper.h>

namespace dll = vart::dll;

class shellAbout
{
  public:
    void invoke() const { m_shellAbout(nullptr, L"hello", L"world", nullptr); }

  private:
    static dll::Fp<decltype(ShellAboutW)> createFuncPointer()
    {
        dll::Helper                    a_dll{std::filesystem::path(L"Shell32.dll")};
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
    try
    {
        shellAbout test;
        test.invoke();
    }
    catch (const dll::DllError& e)
    {
        std::cerr << "Err #1: " << getMessage(e) << std::endl;
    }

    // Error handling examples
    using std::filesystem::path;

    try
    {
        std::cerr << "Going to use wrong dll name: Shell64.dll to demonstrate error handling using exceptions\n";
        dll::Helper                          a_dll{path(L"Shell64.dll")};
        const dll::Fp<decltype(ShellAboutW)> shellAbout{a_dll["ShellAboutW"]};

        shellAbout(nullptr, L"hello", L"world", nullptr);
    }
    catch (const dll::DllError& e)
    {
        std::cerr << "Err #2: " << getMessage(e) << std::endl;
    }

    try
    {
        std::cerr << "Going to use wrong function name: ShellAboutX to demonstrate error handling using exceptions\n";
        dll::Helper                          a_dll{path(L"Shell32.dll")};
        const dll::Fp<decltype(ShellAboutW)> shellAbout{a_dll["ShellAboutX"]};

        shellAbout(nullptr, L"hello", L"world", nullptr);
    }
    catch (const dll::DllError& e)
    {
        std::cerr << "Err #3: " << getMessage(e) << std::endl;
    }
}
