#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <dllhelper.hpp>
#include <system_error>

dll::lib_handle dll::Helper::LoadLibraryInternal(const std::filesystem::path& filename)
{
    static constexpr DWORD   dwFlags = LOAD_LIBRARY_SEARCH_SYSTEM32;
    if(const auto result = LoadLibraryExW(filename.c_str(), nullptr, dwFlags);  result != nullptr) {
        return lib_handle(std::bit_cast<const lib_handle_replacer*>(result), [](const lib_handle_replacer* libptr)
            {
                auto hmodule = std::bit_cast<HMODULE>(libptr);
                ::FreeLibrary(hmodule);
            }
        );
    }
    throw std::system_error(std::error_code(::GetLastError(), std::system_category()), std::format("Failed to load {}", filename.string()));
}

dll::func_handle_t dll::Helper::GetProcAddr(dll::procname_t proc_name) const
{

    const auto res = GetProcAddress(std::bit_cast<HMODULE>(_module.get()), proc_name);
    if(res == nullptr) {
        const char* name = proc_name;
        throw std::system_error(std::error_code(::GetLastError(), std::system_category()), std::format("Function {} not found", name));
    }
    return std::bit_cast<const func_handle_replacer*>(res);
}
