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

    //Error handling examples

    try {
        dll::Helper a_dll{path("libm.so.125")};
    }
    catch(const std::runtime_error& ex) {
        std::cerr << "Failed: " << ex.what() << std::endl;
    }

    try {
        dll::Helper a_dll{path("libm.so.6")};
        [[maybe_unused]] dll::Fp<decltype(cos)> cos_func = a_dll["coscoco"];
    }
    catch(const std::runtime_error& ex) {
        std::cerr << "Failed: " << ex.what() << std::endl;
    }
}
