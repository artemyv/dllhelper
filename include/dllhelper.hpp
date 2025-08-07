#pragma once

#include <type_traits>
#include <cstring> //for memcpy
#include <filesystem>
#include <system_error>
#include <bit>
#include <gsl/pointers> // for gsl::not_null
class ProcPtr {
public:
  constexpr explicit ProcPtr(void* ptr) noexcept : _ptr(ptr) {}

  template <typename T>
      requires (std::is_function_v<T> && !std::is_member_function_pointer_v<T*>)
  [[nodiscard]] constexpr operator T* () const noexcept
  {
      static_assert(sizeof(T*) == sizeof(_ptr), "Pointer sizes must match");
      return std::bit_cast<T*>(_ptr);
  }

private:
  void* _ptr;
};

class DllHelper
{
public:
    [[nodiscard]] explicit DllHelper(const std::filesystem::path& filename): _module(LoadLibraryInternal(filename))
    {
    }

    ~DllHelper() { FreeLibraryInternal(); }

    DllHelper() = delete;
    DllHelper(const DllHelper&) = delete; // Copy constructor
    DllHelper& operator=(const DllHelper&) = delete; // Copy assignment
    DllHelper(DllHelper&& other) = delete; // Move constructor
    DllHelper& operator=(DllHelper&& other) = delete;// Move assignment
    
    [[nodiscard]] ProcPtr operator[](const char* proc_name)
    {
        if( proc_name == nullptr) {
            throw std::system_error(std::make_error_code(std::errc::invalid_argument));
        }

        return GetProcAddr(proc_name);
    }

private:
    static gsl::not_null<void*> LoadLibraryInternal(const std::filesystem::path& filename);
    ProcPtr GetProcAddr(gsl::not_null<const char*> proc_name) ;
    void FreeLibraryInternal() noexcept;

    gsl::not_null<void*> _module;
};
