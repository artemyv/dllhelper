// SPDX-License-Identifier: MIT
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <gtest/gtest.h>
#include <vart/dllhelper/dllhelper.h>

namespace
{
static thread_local bool g_fail_next_allocation = false;

struct FailNextAllocationGuard
{
    FailNextAllocationGuard() { g_fail_next_allocation = true; }
    ~FailNextAllocationGuard() { g_fail_next_allocation = false; }
};
} // namespace

void* operator new(std::size_t sz)
{
    if (g_fail_next_allocation)
    {
        g_fail_next_allocation = false;
        throw std::bad_alloc();
    }
    if (void* p = std::malloc(sz))
        return p;
    throw std::bad_alloc();
}

void operator delete(void* p) noexcept
{
    std::free(p);
}
void operator delete(void* p, std::size_t) noexcept
{
    std::free(p);
}

namespace dll = vart::dll;

TEST(GetMessageBadAllocTest, FallsBackToWhat)
{
    const dll::DllError           ex{"short", {}};
    const FailNextAllocationGuard fail_once;
    const std::string             msg = dll::getMessage(ex);
    EXPECT_EQ(msg, std::string(ex.what()));
}
