#pragma once

#include <type_traits>
#include <cstring> //for memcpy
#include <filesystem>
#include <system_error>

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
    ProcPtr operator[](const char* proc_name) noexcept
    {
        return ProcPtr(GetProcAddr(proc_name));
    }
    std::error_code error_code() const noexcept
    {
        return m_ec;
	}
    std::string error_message() const noexcept
    {
        if(error_message().empty())
        {
            return m_ec.message();
		}
        return m_error_message;
    }

private:
    void* GetProcAddr(const char* proc_name) noexcept;
    void* _module = nullptr;
    std::error_code m_ec;
	std::string m_error_message;
};
