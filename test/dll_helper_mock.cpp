#include <dllhelper.hpp>
#include <system_error>
#include <string_view>

dll::lib_handle dll::Helper::LoadLibraryInternal(const std::filesystem::path& filename)
{
    if(filename.string() == "success") {
        static int x;
        return {
                    std::bit_cast<lib_handle_replacer*>(&x),
                    [](const lib_handle_replacer*) {
                        //mock cleanup - nothing to do
                    }
                };
    }
    throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory));
}
static int mock_function() noexcept
{
    return 42;
}

dll::func_handle_t dll::Helper::GetProcAddr(dll::procname_t proc_name) const
{
    if(std::string_view(proc_name) == "mock_function") {
        auto ptr = &mock_function;

        return std::bit_cast<const func_handle_replacer*>(ptr);
    }
    throw std::system_error(std::make_error_code(std::errc::operation_not_supported));
}
