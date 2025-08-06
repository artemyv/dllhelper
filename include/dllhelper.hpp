#pragma once

#include <type_traits>
#include <cstring> //for memcpy
#include <filesystem>
#include <system_error>
#include <bit>

class ProcPtr {
public:
  constexpr explicit ProcPtr(void* ptr) noexcept : _ptr(ptr) {}

  template <typename T>
      requires (std::is_function_v<T> && !std::is_member_function_pointer_v<T*>)
  constexpr operator T* () const noexcept
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
    explicit DllHelper(const std::filesystem::path& filename) noexcept;

    ~DllHelper();
    DllHelper() = delete;
    DllHelper(const DllHelper&) = delete; // Copy constructor
    DllHelper& operator=(const DllHelper&) = delete; // Copy assignment
    DllHelper(DllHelper&& other) = delete; // Move constructor
    DllHelper& operator=(DllHelper&& other) = delete;// Move assignment
    
    constexpr operator bool() const noexcept
    {
        return _module != nullptr;
    }
    ProcPtr operator[](const char* proc_name) noexcept
    {
        return ProcPtr(GetProcAddr(proc_name));
    }
    constexpr const std::error_code& error_code() const noexcept
    {
        return m_ec;
	}
    constexpr std::string error_message() const noexcept
    {
        if(m_error_message.empty())
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
