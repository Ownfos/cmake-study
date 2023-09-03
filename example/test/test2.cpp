#include <gtest/gtest.h>
#include "Vector3.h"

using namespace math;

TEST(Math, Vector3InitZero)
{
    auto v = Vector3{};
    ASSERT_EQ(v.x, 0.0);
    ASSERT_EQ(v.y, 0.0);
    ASSERT_EQ(v.z, 0.0);
}