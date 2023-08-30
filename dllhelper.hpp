#pragma once

#include <type_traits>
#include <windows.h>
#include <stdexcept>

class ProcPtr {
public:
  explicit ProcPtr(FARPROC ptr) : _ptr(ptr) {}

  template <typename T, typename = std::enable_if_t<std::is_function_v<T>>>
  operator T *() const {
    return reinterpret_cast<T *>(_ptr);
  }

private:
  FARPROC _ptr;
};

class DllHelper {
public:
  explicit DllHelper(LPCTSTR filename) : _module(LoadLibrary(filename)) {
    if (NULL == _module) {
      throw std::runtime_error("Library not loaded");
    }
  }

  ~DllHelper() { FreeLibrary(_module); }

  ProcPtr operator[](LPCSTR proc_name) const {
    return ProcPtr(GetProcAddress(_module, proc_name));
  }

  static HMODULE _parent_module;

private:
  HMODULE _module;
};
