#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <dllhelper.hpp>

DllHelper::~DllHelper() { FreeLibrary(static_cast<HMODULE>(_module)); }

DllHelper::DllHelper(const std::filesystem::path& filename) noexcept: _module(LoadLibraryW(filename.c_str()))
{
    if(_module == nullptr) {
        // I'm not bothering with parsing GetLastError() here.
        // Who get with such an API?
        m_ec = std::error_code(::GetLastError(), std::system_category());
	}

}
void* DllHelper::GetProcAddr(const char* proc_name) noexcept
{
    if(_module == nullptr)
		return nullptr;
    const auto res = GetProcAddress(static_cast<HMODULE>(_module), proc_name);
    if (res == nullptr) {
        // I'm not bothering with parsing GetLastError() here.
        // Who get with such an API?
        m_ec = std::error_code(::GetLastError(), std::system_category());
	}
    else {
		m_ec = std::error_code(); // Clear error code on success
    }
    return res;
}
