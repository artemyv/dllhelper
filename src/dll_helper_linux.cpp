#include <dllhelper.hpp>
#include <dlfcn.h>

dll::lib_handle dll::Helper::LoadLibraryInternal(const std::filesystem::path& filename)
{
    const auto res =  dlopen(filename.c_str(), RTLD_LAZY);
    if(res) {
        return lib_handle(res, [](void*libptr){ dlclose(libptr); } );
    }
    throw std::runtime_error( dlerror());
}
gsl::not_null<void*> dll::Helper::GetProcAddr(gsl::not_null<gsl::czstring> proc_name)
{
    const auto res = dlsym(_module.get(), proc_name);
    if(res)
        return res;
    throw std::runtime_error( dlerror());
}