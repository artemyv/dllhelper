#include <dllhelper.hpp>
#include <iostream>
#include <cmath>

class MathLib {
    DllHelper m_dll{"libm.so.6"};

public:
    // Function pointer type for cos(double)
    using cos_func_t = double(*)(double);
    cos_func_t cos_func = m_dll["cos"];

    operator bool() const noexcept { return m_dll && cos_func != nullptr; }
};

int main() {
    MathLib mathLib;
    if (mathLib) {
        double value = 0.0;
        double result = mathLib.cos_func(value);
        std::cout << "cos(" << value << ") = " << result << std::endl;
    } else {
        std::cerr << "Failed to load libm or find cos symbol." << std::endl;
    }
}