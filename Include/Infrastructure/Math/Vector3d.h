#pragma once
#include "MathDefine.h"
#include <iostream>
#include <cmath>


class JACKC_MATH_INTERFACE_EXPORT Vector3d
{
public:
    Vector3d();
    Vector3d(double x, double y, double z);
    Vector3d(const Vector3d& other);

public:
    Vector3d& operator=(const Vector3d& other);
    Vector3d operator+(const Vector3d& other) const;
    Vector3d operator-(const Vector3d& other) const;
    Vector3d operator*(double scale) const;
    Vector3d operator/(double scale) const;

    Vector3d& operator+=(const Vector3d& other) ;
    Vector3d& operator-=(const Vector3d& other);
    Vector3d& operator*=(const double scale);
    Vector3d& operator/=(const double scale);

    bool operator==(const Vector3d& other) const;
    bool operator!=(const Vector3d& other)const;

    double Dot(const Vector3d& other) const;
    Vector3d Cross(const Vector3d& other) const;
    double Length() const;
    double SquaredLength() const;
    Vector3d Normalized() const;
    void Normalize();

    void ToHomogeneous(double homogeneous[4]) const;
    static Vector3d FromHomogeneous(const double homogeneous[4]);

public:
    double x, y, z;

    static const Vector3d ZERO;
    static const Vector3d UNIT_X;
    static const Vector3d UNIT_Y;
    static const Vector3d UNIT_Z;

};