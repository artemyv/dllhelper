// SPDX-License-Identifier: MIT
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <format>
#include <system_error>
#include <vart/dllhelper/dllhelper.h>

vart::dll::lib_handle vart::dll::Helper::LoadLibraryInternal(const std::filesystem::path& filename)
{
    static constexpr DWORD dwFlags = LOAD_LIBRARY_SEARCH_SYSTEM32;
    if (const auto result = LoadLibraryExW(filename.c_str(), nullptr, dwFlags); result != nullptr)
    {
        return {std::bit_cast<const lib_handle_replacer*>(result), [](const lib_handle_replacer* libptr) {
                    auto hmodule = std::bit_cast<HMODULE>(libptr);
                    ::FreeLibrary(hmodule);
                }};
    }
    const auto ec = std::error_code(::GetLastError(), std::system_category());
    throw DllError(std::format("Failed to load {}", filename.string()), ec);
}

vart::dll::func_handle_internal_t vart::dll::Helper::GetProcAddr(vart::dll::procname_t proc_name) const
{

    const auto res = GetProcAddress(std::bit_cast<HMODULE>(_module.get()), proc_name);
    if (res == nullptr)
    {
        const char* name = proc_name;
        const auto  ec   = std::error_code(::GetLastError(), std::system_category());
        throw DllError(std::format("Function {} not found", name), ec);
    }
    return std::bit_cast<func_handle_internal_t>(res);
}
#endif // _WIN32
