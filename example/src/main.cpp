#include "Vector3.h"
#include "rng/Random.h"
#include <iostream>
#include <format>

using namespace math;

void print(const Vector3& v)
{
    std::cout << std::format("<{}, {}, {}>\n", v.x, v.y, v.z);
}

int main()
{
    auto v1 = Vector3{1, 2, 0};
    auto v2 = Vector3{-1, 4, 3};
    
    print(v1 + v2);
    print(v1 - v2);

    auto rand = rng::Random();
    for (int i = 0; i < 10; ++i)
    {
        std::cout << rand.RandInt(0, 10);
    }
}