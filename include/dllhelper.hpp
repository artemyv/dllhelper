#pragma once

#include <type_traits>
#include <filesystem>
#include <system_error>
#include <bit>
#include <gsl/pointers> // for gsl::not_null
#include <gsl/zstring>

class DllHelper
{
public:
    [[nodiscard]] explicit DllHelper(const std::filesystem::path& filename): _module(LoadLibraryInternal(filename))
    {
    }
	explicit DllHelper(auto p) = delete; // Prevent implicit conversion from other types
    ~DllHelper() { FreeLibraryInternal(); }

    DllHelper() = delete;
    DllHelper(const DllHelper&) = delete; // Copy constructor
    DllHelper& operator=(const DllHelper&) = delete; // Copy assignment
    DllHelper(DllHelper&& other) = delete; // Move constructor
    DllHelper& operator=(DllHelper&& other) = delete;// Move assignment
    
    class ProcPtr
    {
    public:
        constexpr explicit ProcPtr(gsl::not_null<void*> ptr) noexcept: _ptr(ptr) {}

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
        return GetProcAddr(proc_name);
    }

private:
    static gsl::not_null<void*> LoadLibraryInternal(const std::filesystem::path& filename);
    ProcPtr GetProcAddr(gsl::not_null<gsl::czstring> proc_name) ;
    void FreeLibraryInternal() noexcept;

    gsl::not_null<void*> _module;
};
