#include <dllhelper.hpp>

DllHelper::~DllHelper() = default;

DllHelper::DllHelper(const std::filesystem::path& filename) noexcept: _module{nullptr}
{
    if(filename.string() == "success") {
        _module = this;
        return;
	}
    if(_module == nullptr) {
        // I'm not bothering with parsing GetLastError() here.
        // Who get with such an API?
		m_ec = std::make_error_code(std::errc::no_such_file_or_directory);
	}
}
void* DllHelper::GetProcAddr(const char* proc_name) noexcept
{
    if(_module == nullptr)
		return nullptr;
    if(std::string_view(proc_name) == "mock_function") {
        return reinterpret_cast<void*>(+[]() { return 42; });
    }
    m_ec = std::make_error_code(std::errc::operation_not_supported);
    return nullptr;
}
