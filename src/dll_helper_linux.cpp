#include <dlfcn.h>
#include <dllhelper.hpp>

void DllHelper::FreeLibraryInternal() noexcept
{
    dlclose(_module.get());
}

gsl::not_null<void*> DllHelper::LoadLibraryInternal(const std::filesystem::path& filename)
{
    const auto res =  dlopen(filename.c_str(), RTLD_LAZY);
    if(res) {
        return res;
    }
    throw std::runtime_error( dlerror());
}
ProcPtr DllHelper::GetProcAddr(gsl::not_null<const char*> proc_name)
{
    const auto res = dlsym(_module, proc_name);
    if(res)
        return ProcPtr(res);
    throw std::runtime_error( dlerror());
}