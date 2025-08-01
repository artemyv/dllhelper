#pragma once

#include <type_traits>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class ProcPtr {
public:
  explicit ProcPtr(FARPROC ptr) noexcept : _ptr(ptr) {}

  template <typename T, typename = std::enable_if_t<std::is_function_v<T>>>
  operator T *() const noexcept {
#pragma warning(push)
#pragma warning(disable : 26490) 
    return reinterpret_cast<T *>(_ptr);
#pragma warning(pop)
  }

private:
  FARPROC _ptr;
};

class DllHelper {
public:
  explicit DllHelper(const wchar_t* filename) noexcept : _module(LoadLibraryW(filename)) 
  {
  }

  ~DllHelper() { FreeLibrary(_module); }
  DllHelper() = delete;
  DllHelper(const DllHelper&) = delete; // Copy constructor
  DllHelper& operator=(const DllHelper&) = delete; // Copy assignment
  DllHelper(DllHelper&& other) noexcept
  {
      std::swap(_module,other._module);
  }
  ; // Move constructor
  DllHelper& operator=(DllHelper&& other) noexcept// Move assignment
  {
      if(this != &other) {
          FreeLibrary(_module);
          _module = nullptr;
          std::swap(_module, other._module);
	  }
	  return *this;
  }
  operator bool() const noexcept {
    return _module != nullptr;
  }
  ProcPtr operator[](LPCSTR proc_name) const noexcept {
    return ProcPtr(GetProcAddress(_module, proc_name));
  }

private:
  HMODULE _module = nullptr;
};
