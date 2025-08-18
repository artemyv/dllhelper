#include <dllhelper.hpp>
#include <system_error>
#include <string_view>

dll::lib_handle dll::Helper::LoadLibraryInternal(const std::filesystem::path& filename)
{
    if(filename.string() == "success") {
        static int x;
        return lib_handle(static_cast<void*>(&x), [](void*) {});
    }
    throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory));
}

dll::handle_t dll::Helper::GetProcAddr(dll::procname_t proc_name) const
{
    if(std::string_view(proc_name) == "mock_function") {
        return reinterpret_cast<void*>(+[]() { return 42; });
    }
    throw std::system_error(std::make_error_code(std::errc::operation_not_supported));
}
