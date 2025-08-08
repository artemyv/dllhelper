#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <dllhelper.hpp>
#include <system_error>

dll::lib_handle dll::Helper::LoadLibraryInternal(const std::filesystem::path& filename)
{
    const auto result = LoadLibraryW(filename.c_str());
    if(result == nullptr) {
        // I'm not bothering with parsing GetLastError() here.
        // Who get with such an API?
        throw std::system_error( std::error_code(::GetLastError(), std::system_category()), std::format("Failed to load {}", filename.string()));
	}
    return dll::lib_handle(static_cast<void*>(result), [](void* libptr) {::FreeLibrary(static_cast<HMODULE>(libptr)); });
}
gsl::not_null<void*> dll::Helper::GetProcAddr(gsl::not_null<gsl::czstring> proc_name)
{

    const auto res = GetProcAddress(static_cast<HMODULE>(_module.get()), proc_name);
    if (res == nullptr) {
        throw std::system_error(std::error_code(::GetLastError(), std::system_category()), std::format("Function {} not found", proc_name.get()));
	}
    return res;
}
