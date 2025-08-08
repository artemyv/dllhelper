#include <dllhelper.hpp>
#include <windows.h>
#include <shellapi.h>
#include <iostream>

class shellAbout
{

	public:
		void invoke()
		{
			m_shellAbout(nullptr, L"hello", L"world", nullptr);
		}
private:
	static dll::Fp<decltype(ShellAboutW)> createFuncPointer()
	{
		using std::filesystem::path;
		dll::Helper a_dll{path(L"Shell32.dll")};
		dll::Fp<decltype(ShellAboutW)> shellAbout = a_dll["ShellAboutW"];
		return shellAbout;
	}

	// With new approach there is no need to store the dll::Helper object, 
	// as it is not used after the function pointer is created.
	// The function pointer will keep the module handle alive as long as it is used.
	dll::Fp<decltype(ShellAboutW)> m_shellAbout{createFuncPointer()};

};

int main() {
	using std::filesystem::path;
	try
	{
		shellAbout test;
		test.invoke();
	}
	catch(const std::runtime_error& e)
	{
		std::cerr << "Failed: " << e.what() << std::endl;
	}

	//Error handling examples

	try {
		dll::Helper a_dll{path(L"Shell64.dll")};
		const dll::Fp<decltype(ShellAboutW)> shellAbout = a_dll["ShellAboutW"];

		shellAbout(nullptr, L"hello", L"world", nullptr);
	}
	catch(const std::runtime_error& e) {
		std::cerr << "Failed: " << e.what() << std::endl;
	}

	try {
		dll::Helper a_dll{path(L"Shell32.dll")};
		const dll::Fp<decltype(ShellAboutW)> shellAbout = a_dll["ShellAboutX"];

		shellAbout(nullptr, L"hello", L"world", nullptr);
	}
	catch(const std::runtime_error& e) {
		std::cerr << "Failed: " << e.what() << std::endl;
	}

}
