#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <dllhelper.hpp>

DllHelper::~DllHelper() { FreeLibrary(static_cast<HMODULE>(_module)); }

DllHelper::DllHelper(const std::filesystem::path& filename) noexcept: _module(LoadLibraryW(filename.c_str()))
{}
void* DllHelper::GetProcAddr(const char* proc_name) const noexcept
{
    return GetProcAddress(static_cast<HMODULE>(_module), proc_name);
}
