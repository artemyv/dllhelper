#ifndef DLLHELPER_HPP
#define DLLHELPER_HPP

#include <bit>
#include <concepts>
#include <filesystem>
#include <functional>
#include <memory>
#include <stdexcept>

#if defined(WITH_GSL)
#include <gsl/pointers> // for gsl::not_null
#include <gsl/zstring>
#endif

namespace dll
{
#if defined(WITH_GSL)
    using procname_t = gsl::not_null<gsl::czstring>;
    using handle_t = gsl::not_null<void*>;
#else
    using procname_t = const char*;
    using handle_t = void*;
#endif

    template<typename T>
    concept func = std::is_function_v<T> && !std::is_member_function_pointer_v<T>;

    using lib_handle = std::shared_ptr<void>;
    template<func T>
    class Fp
    {
    public:
        [[nodiscard]] explicit Fp(lib_handle libptr, handle_t ptr) noexcept:_module(libptr), _ptr(std::bit_cast<T*>([ptr]() {void* p{ptr}; return p;}())) {}

        template <typename ...Args>
            requires (std::is_invocable_v<T, Args...>)
        auto operator()(Args&& ...args) const noexcept(std::is_nothrow_invocable_v<T, Args...>)
        {
            return std::invoke(_ptr, std::forward<Args>(args)...);
        }

    private:
        lib_handle _module; // Keep the module handle alive as long as the ProcPtr exists
        T* _ptr;
    };
    class Helper
    {
    public:
        [[nodiscard]] explicit Helper(const std::filesystem::path& filename): _module(LoadLibraryInternal(filename))
        {}

        explicit Helper(auto p) = delete; // Prevent implicit conversion from other types
        class ProcPtr
        {
        public:
            [[nodiscard]] explicit ProcPtr(lib_handle libptr, handle_t ptr) noexcept:_module(libptr), _ptr(ptr) {}

            template<func T>
            [[nodiscard]] operator Fp<T>() const noexcept
            {
                return Fp<T>(_module, _ptr);
            }

        private:
            lib_handle _module; // Keep the module handle alive as long as the ProcPtr exists
            handle_t _ptr;
        };

        [[nodiscard]] ProcPtr operator[](procname_t proc_name) const
        {
            if(!_module) {
                throw std::runtime_error("DLL not loaded");
            }
            return ProcPtr(_module, GetProcAddr(proc_name));
        }

    private:
        [[nodiscard]] static  lib_handle LoadLibraryInternal(const std::filesystem::path& filename);
        [[nodiscard]] handle_t GetProcAddr(procname_t proc_name) const;

        lib_handle _module;
    };
}
#endif // DLLHELPER_HPP
