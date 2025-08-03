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
