#ifndef DLLHELPER_HPP
#define DLLHELPER_HPP

#include <bit>
#include <concepts>
#include <filesystem>
#include <functional>
#include <memory>

#if defined(WITH_GSL)
#include <gsl/pointers> // for gsl::not_null
#include <gsl/zstring>
#endif

namespace dll
{
    struct lib_handle_replacer;
    struct func_handle_replacer;
#if defined(WITH_GSL)
    using procname_t = gsl::not_null<gsl::czstring>;
    using func_handle_t = gsl::not_null<const func_handle_replacer*>;
#else
    using procname_t = const char*;
    using func_handle_t = const func_handle_replacer*;
#endif

    inline const func_handle_replacer* getRawHandle(func_handle_t ptr) noexcept
    {
        const func_handle_replacer* p{ptr};
        return p;
    }
    template<typename T>
    concept func = std::is_function_v<T> && !std::is_member_function_pointer_v<T>;

    using lib_handle = std::shared_ptr<const lib_handle_replacer>;
    template<func T>
    class Fp
    {
    public:
        [[nodiscard]] explicit Fp(lib_handle libptr, func_handle_t ptr) noexcept:_module(libptr), _ptr(std::bit_cast<T*>(getRawHandle(ptr))) {}

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
            [[nodiscard]] explicit ProcPtr(lib_handle libptr, func_handle_t ptr) noexcept:_module(libptr), _ptr(ptr) {}

            template<func T>
            [[nodiscard]] explicit operator Fp<T>() const noexcept
            {
                return Fp<T>(_module, _ptr);
            }

        private:
            lib_handle _module; // Keep the module handle alive as long as the ProcPtr exists
            func_handle_t _ptr;
        };

        [[nodiscard]] ProcPtr operator[](procname_t proc_name) const
        {
            return ProcPtr(_module, GetProcAddr(proc_name));
        }

    private:
        [[nodiscard]] static  lib_handle LoadLibraryInternal(const std::filesystem::path& filename);
        [[nodiscard]] func_handle_t GetProcAddr(procname_t proc_name) const;

        lib_handle _module;
    };
}
#endif // DLLHELPER_HPP
