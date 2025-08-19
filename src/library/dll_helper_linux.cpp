#include <dllhelper.hpp>
#include <dlfcn.h>

dll::lib_handle dll::Helper::LoadLibraryInternal(const std::filesystem::path& filename)
{
    if(const auto res = dlopen(filename.c_str(), RTLD_LAZY); res != nullptr) {
        return {std::bit_cast<const lib_handle_replacer*>(res), [](const lib_handle_replacer* libptr) { dlclose(std::bit_cast<void*>(libptr)); }};
    }
    throw std::runtime_error(dlerror());
}

dll::func_handle_t dll::Helper::GetProcAddr(dll::procname_t proc_name) const
{
    if(const auto res = dlsym(std::bit_cast<void*>(_module.get()), proc_name);res != nullptr)
        return std::bit_cast<const func_handle_replacer*>(res);
    throw std::runtime_error(dlerror());
}