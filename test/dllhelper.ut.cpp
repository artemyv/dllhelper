#include <gtest/gtest.h>
#include <dllhelper.h>

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
    //this should not compile
#ifdef NOT_COMPILING_SAMPLE
    dll::Fp<int*> func = mockDll["mock_function"];
    func();
#endif
}

TEST(DllHelperTest, ShouldNotCompile2)
{
    const dll::Helper mockDll(path("success"));
    struct  foo
    {
        int bar() const noexcept
        {
            return 42;
        }
    };
#ifdef NOT_COMPILING_SAMPLE
    const dll::Fp<decltype(&foo::bar)> func{mockDll["mock_function"]};
#endif
}
