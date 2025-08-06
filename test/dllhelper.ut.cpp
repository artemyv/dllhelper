#include <gtest/gtest.h>
#include <dllhelper.hpp>

TEST(DllHelperTest, Positive)
{
    DllHelper mockDll("success");
	ASSERT_TRUE(mockDll);
    using fp = int(*)();
    fp func = mockDll["mock_function"];
    ASSERT_NE(func, nullptr);
    EXPECT_EQ(func(), 42);
}

TEST(DllHelperTest, MissingMethod)
{
    DllHelper mockDll("success");
    ASSERT_TRUE(mockDll);
    using fp = int(*)();
    fp func = mockDll["mock_not_function"];
    ASSERT_EQ(func, nullptr);
    EXPECT_EQ(mockDll.error_code(), std::make_error_code(std::errc::operation_not_supported));
}

TEST(DllHelperTest, MissingLib)
{
    DllHelper mockDll("failure");
    ASSERT_FALSE(mockDll);
    EXPECT_EQ(mockDll.error_code(), std::make_error_code(std::errc::no_such_file_or_directory));
}

TEST(DllHelperTest, ShouldNotCompile)
{
    DllHelper mockDll("success");
    ASSERT_TRUE(mockDll);
    using fp = int*;
	// fp func = mockDll["mock_function"]; //this should not compile
}
TEST(DllHelperTest, ShouldNotCompile2)
{
    DllHelper mockDll("success");
    ASSERT_TRUE(mockDll);
    struct  foo
    {
        int bar()
        {
            return 42;
        }
    };

    using fp = decltype(foo::bar);
    // fp* func = mockDll["mock_function"]; //this should not compile
}