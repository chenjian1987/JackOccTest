#pragma once
#include "MathDefine.h"
#include "Vector3d.h"
#include <cmath>
#include <iostream>


class JACKC_MATH_INTERFACE_EXPORT Matrix44
{
public:
    Matrix44();
    Matrix44(const double data[4][4]);
    Matrix44(const Matrix44& other);

public:
    Matrix44& operator=(const Matrix44& other);

    Matrix44 operator*(const Matrix44& other) const;
    Vector3d operator*(const Vector3d& vec) const;

    void Identity();
    Matrix44 Transposed() const;
    Matrix44 Inverse() const;
    double Daterminant() const;

    static Matrix44 Translation(double dx, double dy, double dz);
    static Matrix44 Scale(double dx, double dy, double dz);
    static Matrix44 Rotate(Vector3d& vec,double angle);

    // …Ë÷√æÿ’Ûµƒ÷µ
    void SetValue(int row, int col, double value);

private:
    double operator()(int row, int cell);

public :
    static const Matrix44 IDENTITY;

public:
    double m[4][4];
    static constexpr double PI = 3.14159265358979323846;
};