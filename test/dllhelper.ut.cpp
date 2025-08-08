#include <gtest/gtest.h>
#include <dllhelper.hpp>

using std::filesystem::path;
TEST(DllHelperTest, Positive)
{
    DllHelper mockDll(path("success"));
    using fp = int();
    auto func = mockDll["mock_function"];
    EXPECT_EQ(func.invoke<fp>(), 42);
}

TEST(DllHelperTest, MissingMethod)
{
    DllHelper mockDll(path("success"));
    EXPECT_THROW({[[maybe_unused]] auto func = mockDll["mock_not_function"];}, std::runtime_error);
}

TEST(DllHelperTest, MissingLib)
{
    EXPECT_THROW({DllHelper mockDll(path("failure"));}, std::runtime_error);
}

TEST(DllHelperTest, ShouldNotCompile)
{
    DllHelper mockDll(path("success"));
    using fp = int*;
	auto func = mockDll["mock_function"]; 
    //this should not compile
   // func.invoke<fp>();
}
TEST(DllHelperTest, ShouldNotCompile2)
{
    DllHelper mockDll(path("success"));
    struct  foo
    {
        int bar()
        {
            return 42;
        }
    };

    auto func = mockDll["mock_function"]; 
    //this should not compile
	//func.invoke<decltype(foo::bar)>();
}