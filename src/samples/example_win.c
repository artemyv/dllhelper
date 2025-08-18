#include <windows.h>
#include <stdio.h>

typedef int(WINAPI* ShellAboutProcW)(HWND, LPCWSTR, LPCWSTR, HICON);

int main()
{
    HMODULE hModule = LoadLibrary(TEXT("Shell32.dll"));

    if(hModule == NULL) {
        // Handle error
        DWORD error = GetLastError();
        printf("Failed to load Shell32.dll, error code: %d\n", error);
        return 1;
    }
    ShellAboutProcW shellAbout =
        (ShellAboutProcW)GetProcAddress(hModule, "ShellAboutW");
    if(shellAbout == NULL) {
        // Handle error
        DWORD error = GetLastError();
        printf("Failed to get address of ShellAboutW, error code: %d\n", error);
        FreeLibrary(hModule);
        return 1;
    }
    shellAbout(NULL, L"hello", L"world", NULL);

    FreeLibrary(hModule);
}