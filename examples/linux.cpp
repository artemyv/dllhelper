// SPDX-License-Identifier: MIT
#include <filesystem>
#include <iostream>
#include <string>
#include <vart/dllhelper/dllhelper.h>

int main()
{
    using std::filesystem::path;
    namespace dll = vart::dll;
    try
    {
        const dll::Helper             a_dll{path("libm.so.6")};
        const dll::Fp<double(double)> cos_func{a_dll["cos"]};

        constexpr double              value  = 0.0;
        const double                  result = cos_func(value);
        std::cout << "cos(" << value << ") = " << result << '\n';
    }
    catch (const dll::DllError& ex)
    {
        std::cerr << "Err #1: " << getMessage(ex) << '\n';
    }

    // Error handling examples

    try
    {
        const dll::Helper a_dll{path("libm.so.125")};
    }
    catch (const dll::DllError& ex)
    {
        std::cerr << "Err #2: " << getMessage(ex) << '\n';
    }

    try
    {
        const dll::Helper                              a_dll{path("libm.so.6")};
        [[maybe_unused]] const dll::Fp<double(double)> cos_func{a_dll["coscoco"]};
    }
    catch (const dll::DllError& ex)
    {
        std::cerr << "Err #3: " << getMessage(ex) << '\n';
    }
}
