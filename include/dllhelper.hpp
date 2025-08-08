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

    class ProcPtr
    {
    public:
        [[nodiscard]] constexpr explicit ProcPtr(gsl::not_null<void*> ptr) noexcept: _ptr(ptr) {}

        template <typename T>
            requires (std::is_function_v<T> && !std::is_member_function_pointer_v<T*>)
        [[nodiscard]] constexpr operator T* () const noexcept
        {
            return std::bit_cast<T*>(_ptr);
        }

    private:
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
    static void FreeLibraryInternal(void*) noexcept;
	using lib_handle = std::unique_ptr<void, decltype(&FreeLibraryInternal)>;
    [[nodiscard]] static  lib_handle LoadLibraryInternal(const std::filesystem::path& filename);
    [[nodiscard]] ProcPtr GetProcAddr(gsl::not_null<gsl::czstring> proc_name) ;

    lib_handle _module;
};
