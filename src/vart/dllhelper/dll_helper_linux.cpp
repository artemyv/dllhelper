// SPDX-License-Identifier: MIT
#include <cerrno>
#include <dlfcn.h>
#include <format>
#include <system_error>
#include <vart/dllhelper/dllhelper.h>

vart::dll::lib_handle vart::dll::Helper::LoadLibraryInternal(const std::filesystem::path& filename)
{
    if (const auto res = dlopen(filename.c_str(), RTLD_LAZY); res != nullptr)
    {
        return {std::bit_cast<const lib_handle_replacer*>(res),
                [](const lib_handle_replacer* libptr) { dlclose(std::bit_cast<void*>(libptr)); }};
    }
    const char* msg = dlerror();
    auto        ec  = std::error_code(errno, std::generic_category());
    throw DllError(std::format("Failed to load {}{}{}", filename.string(), (msg ? ": " : ""), (msg ? msg : "")), ec);
}

vart::dll::func_handle_internal_t vart::dll::Helper::GetProcAddr(vart::dll::procname_t proc_name) const
{
    if (const auto res = dlsym(std::bit_cast<void*>(_module.get()), proc_name); res != nullptr)
        return std::bit_cast<func_handle_internal_t>(res);
    const char* msg  = dlerror();
    auto        ec   = std::error_code(errno, std::generic_category());
    const char* name = proc_name;
    throw DllError(std::format("Function {} not found{}{}", name, (msg ? ": " : ""), (msg ? msg : "")), ec);
}
