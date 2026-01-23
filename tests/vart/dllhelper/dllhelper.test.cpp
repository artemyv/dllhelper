// SPDX-License-Identifier: MIT
#include <gtest/gtest.h>
#include <vart/dllhelper/dllhelper.h>

using std::filesystem::path;
namespace dll = vart::dll;
TEST(DllHelperTest, Positive)
{
    const dll::Helper mockDll(path(MOCK_DLL_PATH));
    using fp = int();
    const dll::Fp<fp> func{mockDll["mock_function"]};
    EXPECT_EQ(func(), 42);
}

TEST(DllHelperTest, MissingMethod)
{
    const dll::Helper mockDll(path(MOCK_DLL_PATH));
    using fp = int();
    EXPECT_THROW({ [[maybe_unused]] const dll::Fp<fp> func{mockDll["mock_not_function"]}; }, dll::DllError);
}

TEST(DllHelperTest, MissingLib)
{
    EXPECT_THROW({ const dll::Helper mockDll(path("failure")); }, dll::DllError);
}

TEST(DllHelperTest, ShouldNotCompile)
{
    const dll::Helper mockDll(path(MOCK_DLL_PATH));
    // this should not compile
#ifdef NOT_COMPILING_SAMPLE
    dll::Fp<int*> func = mockDll["mock_function"];
    func();
#endif
}

TEST(DllHelperTest, ShouldNotCompile2)
{
    const dll::Helper mockDll(path(MOCK_DLL_PATH));
    struct foo
    {
        int bar() const noexcept { return 42; }
    };
#ifdef NOT_COMPILING_SAMPLE
    const dll::Fp<decltype(&foo::bar)> func{mockDll["mock_function"]};
#endif
}

TEST(DllHelperTest, GetMessage_WithErrorCode)
{
    const auto ec = std::make_error_code(std::errc::invalid_argument);
    const dll::DllError ex{"bad arg", ec};

    const std::string msg = dll::getMessage(ex);

    const auto expected_code = std::to_string(ec.value());
    EXPECT_NE(msg.find("Error: " + expected_code + " ("), std::string::npos);
    EXPECT_NE(msg.find("message: bad arg"), std::string::npos);
}

TEST(DllHelperTest, GetMessage_DefaultErrorCode)
{
    const dll::DllError ex{"just a message"};

    const std::string msg = dll::getMessage(ex);

    EXPECT_NE(msg.find("Error: 0 ("), std::string::npos);
    EXPECT_NE(msg.find("message: just a message"), std::string::npos);
}
