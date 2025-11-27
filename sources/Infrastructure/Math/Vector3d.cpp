#include "Vector3d.h"




const Vector3d Vector3d::ZERO(0, 0, 0);
const Vector3d Vector3d::UNIT_X(1.0, 0.0, 0);
const Vector3d Vector3d::UNIT_Y(0.0, 1.0, 0.0);
const Vector3d Vector3d::UNIT_Z(0.0, 0.0, 1.0);


Vector3d::Vector3d()
{
    x = 0.0; y = 0.0; z = 0.0;
}
Vector3d::Vector3d(double dx, double dy, double dz)
{
    x = dx; y = dy; z = dz;
}
Vector3d::Vector3d(const Vector3d& other)
{
    x = other.x;
    y = other.y;
    z = other.z;
}


Vector3d& Vector3d::operator=(const Vector3d& other)
{
    if (this != &other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
    }
    return *this;
}

Vector3d Vector3d::operator+(const Vector3d& other) const
{
    Vector3d ans;
    ans.x += other.x;
    ans.y += other.y;
    ans.z += other.z;
    return ans;
}
Vector3d Vector3d::operator-(const Vector3d& other) const
{
    Vector3d ans;
    ans.x -= other.x;
    ans.y -= other.y;
    ans.z -= other.z;
    return ans;
}
Vector3d Vector3d::operator*(double scale) const
{
    return Vector3d(x * scale, y * scale, z * scale);
}
Vector3d Vector3d::operator/(double scale) const
{
    if (std::abs(scale) < 1e-12)
        throw std::runtime_error("scale is zero");
    return Vector3d(x / scale, y / scale, z / scale);
}


Vector3d& Vector3d::operator+=(const Vector3d& other)
{
    x = x + other.x;
    y = y + other.y;
    z = z + other.z;
    return *this;
}
Vector3d& Vector3d::operator-=(const Vector3d& other)
{
    x = x - other.x;
    y = y - other.y;
    z = z - other.z;
    return *this;
}
Vector3d& Vector3d::operator*=(const double scale)
{
    x *= scale;
    y *= scale;
    z *= scale;
    return *this;
}
Vector3d& Vector3d::operator/=(const double scale)
{
    if (std::abs(scale) < 1e-12)
        throw std::runtime_error("scale is zero");
    x /= scale;
    y /= scale;
    z /= scale;
    return *this;
}


bool Vector3d::operator==(const Vector3d& other) const
{
    return std::abs(x - other.x) < 1e-8 && std::abs(y - other.y) < 1e-8 && std::abs(z - other.z) < 1e-8;
}

bool Vector3d::operator!=(const Vector3d& other) const
{
    return !(*this == other);
}


double Vector3d::Dot(const Vector3d& other) const
{
    return x * other.x + y * other.y + z * other.z;
}

Vector3d Vector3d::Cross(const Vector3d& other) const
{
    return Vector3d(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
}

double Vector3d::Length() const 
{
    return std::sqrt(x * x + y * y + z * z);
}

double Vector3d::SquaredLength() const 
{
    return x * x + y * y + z * z;
}


Vector3d Vector3d::Normalized() const
{
    double length = Length();
    if (length<1e-15)
        throw std::runtime_error("scale is zero");
    return *this / length;
}

void Vector3d::Normalize()
{
    double length = Length();
    if (length)
        throw std::runtime_error("scale is zero");
    x /= length;
    y /= length;
    z /= length;
}

void Vector3d::ToHomogeneous(double homo[4]) const
{
    homo[0] = x;
    homo[1] = y;
    homo[2] = z;
    homo[3] = 1;
}

Vector3d Vector3d::FromHomogeneous(const double homo[4])
{
    if(std::abs(homo[3]<1e-12))
        throw std::runtime_error("scale is zero");
    return Vector3d(homo[0] / homo[3], homo[1] / homo[3], homo[2] / homo[3]);
}