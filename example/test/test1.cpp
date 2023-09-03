#include <gtest/gtest.h>
#include "Vector3.h"

using namespace math;

TEST(Math, Vector3Add)
{
    auto v1 = Vector3{ .y = 2.0 };
    auto v2 = Vector3{ .x = 1.0 };
    auto result = v1 + v2;

    ASSERT_EQ(result.x, 1.0);
    ASSERT_EQ(result.y, 2.0);
    ASSERT_EQ(result.z, 0.0);
}

TEST(Math, Vector3Subtract)
{
    auto v1 = Vector3{ .x = 2.0, .z = 3.0 };
    auto v2 = Vector3{ .x = 1.0, .y = 1.0 };
    auto result = v1 - v2;

    ASSERT_EQ(result.x, 1.0);
    ASSERT_EQ(result.y, -1.0);
    ASSERT_EQ(result.z, 3.0);
}