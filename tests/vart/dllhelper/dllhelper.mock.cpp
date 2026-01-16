// SPDX-License-Identifier: MIT
// Portable export macro for Windows and POSIX
#if defined(_WIN32) || defined(__CYGWIN__)
#define MOCK_DLL_EXPORT __declspec(dllexport)
#elif defined(__GNUC__) || defined(__clang__)
#define MOCK_DLL_EXPORT __attribute__((visibility("default")))
#else
#define MOCK_DLL_EXPORT
#endif

#ifdef __cplusplus
#define MOCK_DLL_EXTERN extern "C"
#else
#define MOCK_DLL_EXTERN
#endif

MOCK_DLL_EXTERN MOCK_DLL_EXPORT int mock_function() noexcept { return 42; }
