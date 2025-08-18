#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <dllhelper.hpp>
#include <system_error>

dll::lib_handle dll::Helper::LoadLibraryInternal(const std::filesystem::path& filename)
{
    static constexpr DWORD   dwFlags = LOAD_LIBRARY_SEARCH_SYSTEM32;
    const auto result = LoadLibraryExW(filename.c_str(), nullptr, dwFlags);
    if(result == nullptr) {
        throw std::system_error(std::error_code(::GetLastError(), std::system_category()), std::format("Failed to load {}", filename.string()));
    }
    return dll::lib_handle(static_cast<void*>(result), [](void* libptr) {::FreeLibrary(static_cast<HMODULE>(libptr)); });
}

dll::handle_t dll::Helper::GetProcAddr(dll::procname_t proc_name) const
{

    const auto res = GetProcAddress(static_cast<HMODULE>(_module.get()), proc_name);
    if(res == nullptr) {
        const char* name = proc_name;
        throw std::system_error(std::error_code(::GetLastError(), std::system_category()), std::format("Function {} not found", name));
    }
    return res;
}
