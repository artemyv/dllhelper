#include <dllhelper.hpp>

void DllHelper::FreeLibraryInternal() noexcept
{
}

gsl::not_null<void*> DllHelper::LoadLibraryInternal(const std::filesystem::path& filename)
{
    if(filename.string() == "success") {
        static int x;
        return static_cast<void*>(&x);
    }
    throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory));
}
ProcPtr DllHelper::GetProcAddr(gsl::not_null<const char*> proc_name)
{
    if(std::string_view(proc_name) == "mock_function") {
        return ProcPtr(reinterpret_cast<void*>(+[]() { return 42; }));
    }
    throw std::system_error(std::make_error_code(std::errc::operation_not_supported));
}
