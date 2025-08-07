#include <dllhelper.hpp>
#include <windows.h>
#include <shellapi.h>
#include <iostream>


int main() {
	using std::filesystem::path;
	try
	{
		DllHelper m_dll{path(L"Shell32.dll")};
		decltype(ShellAboutW)* shellAbout = m_dll["ShellAboutW"];

		shellAbout(NULL, L"hello", L"world", NULL);
	}
	catch(const std::runtime_error& e)
	{
		std::cerr << "Failed: " << e.what() << std::endl;
	}

	//Error handling examples

	try {
		DllHelper m_dll{path(L"Shell64.dll")};
		decltype(ShellAboutW)* shellAbout = m_dll["ShellAboutW"];

		shellAbout(NULL, L"hello", L"world", NULL);
	}
	catch(const std::runtime_error& e) {
		std::cerr << "Failed: " << e.what() << std::endl;
	}

	try {
		DllHelper m_dll{path(L"Shell32.dll")};
		decltype(ShellAboutW)* shellAbout = m_dll["ShellAboutX"];

		shellAbout(NULL, L"hello", L"world", NULL);
	}
	catch(const std::runtime_error& e) {
		std::cerr << "Failed: " << e.what() << std::endl;
	}

}
