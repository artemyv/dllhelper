#include <dllhelper.hpp>
#include <dlfcn.h>

dll::lib_handle dll::Helper::LoadLibraryInternal(const std::filesystem::path& filename)
{
    const auto res = dlopen(filename.c_str(), RTLD_LAZY);
    if(res != nullptr) {
        return {res, [](void* libptr) { dlclose(libptr); }};
    }
    throw std::runtime_error(dlerror());
}

dll::handle_t dll::Helper::GetProcAddr(dll::procname_t proc_name) const
{
    const auto res = dlsym(_module.get(), proc_name);
    if(res != nullptr)
        return res;
    throw std::runtime_error(dlerror());
}