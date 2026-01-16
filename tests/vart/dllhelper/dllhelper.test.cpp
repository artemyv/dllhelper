// SPDX-License-Identifier: MIT
#include <gtest/gtest.h>
#include <vart/dllhelper/dllhelper.h>

using std::filesystem::path;
TEST(DllHelperTest, Positive) {
    const dll::Helper mockDll(path(MOCK_DLL_PATH));
    using fp = int();
    const dll::Fp<fp> func{mockDll["mock_function"]};
    EXPECT_EQ(func(), 42);
}

TEST(DllHelperTest, MissingMethod) {
    const dll::Helper mockDll(path(MOCK_DLL_PATH));
    using fp = int();
    EXPECT_THROW({ [[maybe_unused]] const dll::Fp<fp> func{mockDll["mock_not_function"]}; }, std::exception);
}

TEST(DllHelperTest, MissingLib) {
    EXPECT_THROW(
        { const dll::Helper mockDll(path("failure")); },
        std::exception); // todo make win and linux implementation to throw same type of custom made exception
}

TEST(DllHelperTest, ShouldNotCompile) {
    const dll::Helper mockDll(path(MOCK_DLL_PATH));
    // this should not compile
#ifdef NOT_COMPILING_SAMPLE
    dll::Fp<int*> func = mockDll["mock_function"];
    func();
#endif
}

TEST(DllHelperTest, ShouldNotCompile2) {
    const dll::Helper mockDll(path(MOCK_DLL_PATH));
    struct foo {
        int bar() const noexcept { return 42; }
    };
#ifdef NOT_COMPILING_SAMPLE
    const dll::Fp<decltype(&foo::bar)> func{mockDll["mock_function"]};
#endif
}
