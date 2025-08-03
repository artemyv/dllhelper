#pragma once

#include <type_traits>
#include <cstring> //for memcpy
#include <filesystem>
class ProcPtr {
public:
  explicit ProcPtr(void* ptr) noexcept : _ptr(ptr) {}

  template <typename T, typename = std::enable_if_t<std::is_function_v<T>>>
  operator T *() const noexcept {
      T* func = nullptr;
      static_assert(sizeof(func) == sizeof(_ptr), "Pointer sizes must match");
      std::memcpy(&func, &_ptr, sizeof(func));
      return func;
  }

private:
  void* _ptr;
};

class DllHelper
{
public:
    explicit DllHelper(const std::filesystem::path& filename) noexcept;

    ~DllHelper();
    DllHelper() = delete;
    DllHelper(const DllHelper&) = delete; // Copy constructor
    DllHelper& operator=(const DllHelper&) = delete; // Copy assignment
    DllHelper(DllHelper&& other) noexcept
    {
        std::swap(_module, other._module);
    }
    ; // Move constructor
    DllHelper& operator=(DllHelper&& other) noexcept// Move assignment
    {
        DllHelper temp(std::move(other));

        std::swap(_module, temp._module);
        return *this;
    }
    operator bool() const noexcept
    {
        return _module != nullptr;
    }
    ProcPtr operator[](const char* proc_name) const noexcept
    {
        return ProcPtr(GetProcAddr(proc_name));
    }

private:
    void* GetProcAddr(const char* proc_name) const noexcept;
    void* _module = nullptr;
};
