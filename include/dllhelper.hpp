#pragma once

#include <type_traits>
#include <filesystem>
#include <stdexcept>
#include <bit>
#include <memory>
#include <gsl/pointers> // for gsl::not_null
#include <gsl/zstring>
class DllHelper
{
public:
    [[nodiscard]] explicit DllHelper(const std::filesystem::path& filename): _module(LoadLibraryInternal(filename))
    {
    }

	explicit DllHelper(auto p) = delete; // Prevent implicit conversion from other types
    using lib_handle = std::shared_ptr<void>;
    class ProcPtr
    {
    public:
        [[nodiscard]] explicit ProcPtr(lib_handle libptr, gsl::not_null<void*> ptr) noexcept:_module(libptr), _ptr(ptr) {}

        template <typename T, typename ...Args>
            requires (std::is_function_v<T> && !std::is_member_function_pointer_v<T*>)
		auto invoke(Args&& ...args) const noexcept(std::is_nothrow_invocable_v<T, Args...>)
        {
            const auto f = std::bit_cast<T*>(_ptr);
			return f(std::forward<Args>(args)...);
        }

    private:
        lib_handle _module; // Keep the module handle alive as long as the ProcPtr exists
        gsl::not_null<void*> _ptr;
    };

    [[nodiscard]] ProcPtr operator[](gsl::not_null<gsl::czstring> proc_name)
    {
        if(!_module) {
            throw std::runtime_error("DLL not loaded");
		}
        return GetProcAddr(proc_name);
    }

private:
    [[nodiscard]] static  lib_handle LoadLibraryInternal(const std::filesystem::path& filename);
    [[nodiscard]] ProcPtr GetProcAddr(gsl::not_null<gsl::czstring> proc_name) ;

    lib_handle _module;
};
