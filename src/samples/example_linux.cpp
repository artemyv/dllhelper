#include <dllhelper.h>
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

    //Error handling examples

    try {
        const dll::Helper a_dll{path("libm.so.125")};
    }
    catch(const std::invalid_argument& ex) {
        std::cerr << "Err #2: " << ex.what() << '\n';
    }

    try {
        const dll::Helper a_dll{path("libm.so.6")};
        [[maybe_unused]] const dll::Fp<decltype(cos)> cos_func{a_dll["coscoco"]};
    }
    catch(const std::invalid_argument& ex) {
        std::cerr << "Err #3: " << ex.what() << '\n';
    }
}
