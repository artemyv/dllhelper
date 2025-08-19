#include <gtest/gtest.h>
#include <dllhelper.hpp>

using std::filesystem::path;
TEST(DllHelperTest, Positive)
{
    const dll::Helper mockDll(path("success"));
    using fp = int();
    const dll::Fp<fp> func{mockDll["mock_function"]};
    EXPECT_EQ(func(), 42);
}

TEST(DllHelperTest, MissingMethod)
{
    const dll::Helper mockDll(path("success"));
    using fp = int();
    EXPECT_THROW({[[maybe_unused]] const  dll::Fp<fp> func{mockDll["mock_not_function"]};}, std::runtime_error);
}

TEST(DllHelperTest, MissingLib)
{
    EXPECT_THROW({const dll::Helper mockDll(path("failure")); }, std::runtime_error);
}

TEST(DllHelperTest, ShouldNotCompile)
{
    const dll::Helper mockDll(path("success"));
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
    const dll::Helper mockDll(path("success"));
    struct  foo
    {
        int bar()
        {
            return 42;
        }
    };
    using mf = PM_traits<decltype(&foo::bar)>::member_type; // T is int() const&

    const dll::Fp<mf> func{mockDll["mock_function"]};
    //func(foo{});
}
