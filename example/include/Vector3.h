#ifndef MATH_VECTOR3_H
#define MATH_VECTOR3_H

namespace math
{

struct Vector3
{
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    Vector3 operator+(const Vector3& other) const;
    Vector3 operator-(const Vector3& other) const;
};

} // namespace math

#endif
