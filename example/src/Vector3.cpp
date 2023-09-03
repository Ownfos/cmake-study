#include "Vector3.h"

namespace math
{

Vector3 Vector3::operator+(const Vector3& other) const
{
    return {x + other.x, y + other.y, z + other.z};
}

Vector3 Vector3::operator-(const Vector3& other) const
{
    return {x - other.x, y - other.y, z - other.z};
}

} // namespace math