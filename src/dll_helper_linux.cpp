#include <dlfcn.h>
#include <dllhelper.hpp>

DllHelper::~DllHelper() {
    if (_module) {
        dlclose(_module);
    }
}

DllHelper::DllHelper(const std::filesystem::path& filename) noexcept
    : _module(dlopen(filename.c_str(), RTLD_LAZY))
{}

void* DllHelper::GetProcAddr(const char* proc_name) const noexcept {
    return _module ? dlsym(_module, proc_name) : nullptr;
}