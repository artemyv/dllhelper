#include <dllhelper.hpp>
#include <iostream>
#include <cmath>

int main() {
    using std::filesystem::path;
    try
    {
        DllHelper m_dll{path("libm.so.6")};
        auto cos_func = m_dll["cos"];

        double value = 0.0;
        double result = cos_func.invoke<decltype(cos)>(value);
        std::cout << "cos(" << value << ") = " << result << std::endl;
    } 
    catch(const std::runtime_error& ex)
    {
        std::cerr << "Failed: " << ex.what() << std::endl;
    }

	//Error handling examples

    try {
        DllHelper m_dll{path("libm.so.125")};
    }
    catch(const std::runtime_error& ex) {
        std::cerr << "Failed: " << ex.what() << std::endl;
    }

    try {
        DllHelper m_dll{path("libm.so.6")};
        [[maybe_unused]]auto cos_func = m_dll["coscoco"];
    }
    catch(const std::runtime_error& ex) {
        std::cerr << "Failed: " << ex.what() << std::endl;
    }
}
