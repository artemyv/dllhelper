#include <dllhelper.hpp>
#include <system_error>


DllHelper::lib_handle DllHelper::LoadLibraryInternal(const std::filesystem::path& filename)
{
    if(filename.string() == "success") {
        static int x;
        return lib_handle(static_cast<void*>(&x), [](void*) {});
    }
    throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory));
}
DllHelper::ProcPtr DllHelper::GetProcAddr(gsl::not_null<gsl::czstring> proc_name)
{
    if(std::string_view(proc_name) == "mock_function") {
        return ProcPtr(_module, reinterpret_cast<void*>(+[]() { return 42; }));
    }
    throw std::system_error(std::make_error_code(std::errc::operation_not_supported));
}
