#include <dlfcn.h>
#include <dllhelper.hpp>

DllHelper::~DllHelper() {
    if (_module) {
        dlclose(_module);
    }
}

DllHelper::DllHelper(const std::filesystem::path& filename) noexcept
    : _module(dlopen(filename.c_str(), RTLD_LAZY))
{
    if(_module == nullptr) {
        // I'm not bothering with parsing dlerror() string here.
        // Who get with such an API?
		m_ec = std::make_error_code(std::errc::no_such_file_or_directory);
		m_error_message = dlerror();
	}
}

void* DllHelper::GetProcAddr(const char* proc_name) noexcept {
    if(_module == nullptr) return nullptr;
	const auto res = dlsym(_module, proc_name);
    if (res == nullptr) {
        // I'm not bothering with parsing dlerror() string here.
        // Who get with such an API?
        m_ec = std::make_error_code(std::errc::function_not_supported);
        m_error_message = dlerror();
    }
    else {
        m_ec = std::error_code(); // Clear error code on success
		m_error_message.clear(); // Clear error message on success
    }
    return res;
}