#include <dllhelper.hpp>
#include <iostream>
#include <cmath>

int main() {
    try
    {
        DllHelper m_dll{"libm.so.6"};
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

	//Error handling examples

    try {
        DllHelper m_dll{"libm.so.125"};
    }
    catch(const std::runtime_error& ex) {
        std::cerr << "Failed: " << ex.what() << std::endl;
    }

    try {
        DllHelper m_dll{"libm.so.6"};
        using cos_func_t = decltype(cos);
        [[maybe_unused]]cos_func_t* cos_func = m_dll["coscoco"];

    }
    catch(const std::runtime_error& ex) {
        std::cerr << "Failed: " << ex.what() << std::endl;
    }
}
