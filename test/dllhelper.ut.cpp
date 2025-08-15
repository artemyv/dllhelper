#include <gtest/gtest.h>
#include <dllhelper.hpp>

using std::filesystem::path;
TEST(DllHelperTest, Positive)
{
    dll::Helper mockDll(path("success"));
    using fp = int();
    dll::Fp<fp> func = mockDll["mock_function"];
    EXPECT_EQ(func(), 42);
}

TEST(DllHelperTest, MissingMethod)
{
    dll::Helper mockDll(path("success"));
    using fp = int();
    EXPECT_THROW({[[maybe_unused]] dll::Fp<fp> func = mockDll["mock_not_function"]; }, std::runtime_error);
}

TEST(DllHelperTest, MissingLib)
{
    EXPECT_THROW({dll::Helper mockDll(path("failure")); }, std::runtime_error);
}

TEST(DllHelperTest, ShouldNotCompile)
{
    dll::Helper mockDll(path("success"));
    using fp = int*;
    //this should not compile
    //dll::Fp<fp> func = mockDll["mock_function"];
    //func();
}

template<typename>
struct PM_traits {};
template<class T, class U>
struct PM_traits<T U::*> { using member_type = T; };

TEST(DllHelperTest, ShouldNotCompile2)
{
    dll::Helper mockDll(path("success"));
    struct  foo
    {
        int bar()
        {
            return 42;
        }
    };
    using mf = PM_traits<decltype(&foo::bar)>::member_type; // T is int() const&

    dll::Fp<mf> func = mockDll["mock_function"];
    //func(foo{});
}