#include <gtest/gtest.h>
#include <dllhelper.hpp>

TEST(DllHelperTest, Positive)
{
    DllHelper mockDll("success");
    using fp = int(*)();
    fp func = mockDll["mock_function"];
    EXPECT_EQ(func(), 42);
}

TEST(DllHelperTest, MissingMethod)
{
    DllHelper mockDll("success");
    using fp = int(*)();
    EXPECT_THROW({[[maybe_unused]] fp func = mockDll["mock_not_function"];}, std::runtime_error);
}

TEST(DllHelperTest, MissingLib)
{
    EXPECT_THROW({DllHelper mockDll("failure");}, std::runtime_error);
}

TEST(DllHelperTest, ShouldNotCompile)
{
    DllHelper mockDll("success");
    using fp = int*;
	// fp func = mockDll["mock_function"]; //this should not compile
}
TEST(DllHelperTest, ShouldNotCompile2)
{
    DllHelper mockDll("success");
    struct  foo
    {
        int bar()
        {
            return 42;
        }
    };

    //using fp = decltype(foo::bar);
    // fp* func = mockDll["mock_function"]; //this should not compile
}