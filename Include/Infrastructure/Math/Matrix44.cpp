#include "Matrix44.h"



const Matrix44 Matrix44::IDENTITY;





Matrix44::Matrix44()
{
    Identity();
}

Matrix44::Matrix44(const double data[4][4])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m[i][j] = data[i][j];
        }
    }
}

Matrix44::Matrix44(const Matrix44& mat)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m[i][j] = mat.m[i][j];
        }
    }
}






Matrix44& Matrix44::operator=(const Matrix44& other) 
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m[i][j] = other.m[i][j];
        }
    }
    return *this;
}

Matrix44 Matrix44::operator*(const Matrix44& other) const
{
    Matrix44 ans;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            ans.m[i][j] = 0;
            for (int k = 0; k < 4; k++)
            {
                ans.m[i][j] += m[i][k] * other.m[k][j];
            }
        }
    }
    return ans;
}

Vector3d Matrix44::operator*(const Vector3d& vec) const
{
    double homo[4];
    vec.ToHomogeneous(homo);

    double ans[4] = { 0,0,0,0 };
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            ans[i] += m[i][j] * homo[j];
        }
    }
    return Vector3d::FromHomogeneous(ans);
}

double Matrix44::operator()(int row, int cell)
{
    if (row < 0 || row >= 4 || cell < 0 || cell >= 4) {
        throw std::out_of_range("Matrix index out of range");
    }
    return m[row][cell];
}

void Matrix44::SetValue(int row, int col, double value)
{
    if (row < 0 || row >= 4 || col < 0 || col >= 4)
        throw std::out_of_range("Matrix index out of range");
    m[row][col] = value;
}

// 单位阵
void Matrix44::Identity()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m[i][j] = i == j ? 1 : 0;
        }
    }
}

// 转置矩阵
Matrix44 Matrix44::Transposed() const
{
    Matrix44 ans;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            ans.m[i][j] = m[j][i];
        }
    }
    return ans;
}

// 行列式计算
double Matrix44::Daterminant() const
{
    double ans = 0;
    for (int j = 0; j < 4; j++)
    {
        double cof = 0;
        //创建33矩阵
        double upper[3][3];
        int upper_i = 0;
        for (int i = 1; i < 4; i++)
        {
            int upper_j = 0;
            for (int k = 0; k < 4; k++)
            {
                if (k != j)
                {
                    upper[upper_i][upper_j] = m[i][k];
                    upper_j++;
                }
            }
            upper_i++;
        }

        double det = upper[0][0] * (upper[1][1] * upper[2][2] - upper[1][2] * upper[2][1]) -
            upper[0][1] * (upper[1][0] * upper[2][2] - upper[1][2] * upper[2][0]) +
            upper[0][2] * (upper[1][0] * upper[2][1] - upper[1][1] * upper[2][0]);
        double sign = (j % 2 == 0) ? 1.0 : -1.0;
        cof = sign * det;

        ans += m[0][j] * cof;
    }
    return ans;
}

Matrix44 Matrix44::Inverse() const 
{ 
    Matrix44 result = *this;Matrix44 identity;
    identity.Identity();

    // 创建增广矩阵 [this | identity]
    double augmented[4][8];
    for (int i = 0; i < 4; ++i) 
    {
        for (int j = 0; j < 4; ++j) 
        {
            augmented[i][j] = result.m[i][j];
            augmented[i][j + 4] = identity.m[i][j];
        }
    }
    // 高斯-约当消元
    for (int col = 0; col < 4; ++col) 
    {
        // 寻找主元
        int pivot_row = col;
        double max_val = std::abs(augmented[col][col]);

        for (int row = col + 1; row < 4; ++row) 
        {
            double val = std::abs(augmented[row][col]);
            if (val > max_val) 
            {
                max_val = val;
                pivot_row = row;
            }
        }
        if (max_val < 1e-15) 
        {
            throw std::runtime_error("Matrix is singular, cannot invert");
        }

        // 交换行
        if (pivot_row != col) 
        {
            for (int j = 0; j < 8; ++j) 
            {
                std::swap(augmented[col][j], augmented[pivot_row][j]);
            }
        }

        // 归一化主元行
        double pivot = augmented[col][col];
        for (int j = 0; j < 8; ++j) 
        {
            augmented[col][j] /= pivot;
        }
        // 消元
        for (int row = 0; row < 4; ++row) 
        {
            if (row != col)
            {
                double factor = augmented[row][col];
                for (int j = 0; j < 8; ++j) 
                {
                    augmented[row][j] -= factor * augmented[col][j];
                }
            }
        }
    }
    // 提取逆矩阵
    for (int i = 0; i < 4; ++i) 
    {
        for (int j = 0; j < 4; ++j) 
        {
            result.m[i][j] = augmented[i][j + 4];
        }
    }

    return result;
}


Matrix44 Matrix44::Translation(double dx, double dy, double dz)
{
    Matrix44 ans;
    ans.Identity();
    ans.m[0][3] = dx;
    ans.m[1][3] = dy;
    ans.m[2][3] = dz;
    ans.m[3][3] = 1;
    return ans;
}

Matrix44 Matrix44::Scale(double sx, double sy, double sz) 
{
    Matrix44 result;
    result.m[0][0] *= sx;
    result.m[1][1] *= sy;
    result.m[2][2] *= sz;
    result.m[3][3] = 1.0;
    return result;
}

Matrix44 Matrix44::Rotate(Vector3d& vec, double angle)
{
    double axisLength = vec.Length();
    if (axisLength < 1e-15)
        throw std::invalid_argument("rotation axis cannot be zero vector");

    Vector3d unitAxis = vec.Normalized();
    double ux = unitAxis.x;
    double uy = unitAxis.y;
    double uz = unitAxis.z;

    //转换为弧度
    double rad = angle * PI / 180.0;
    double cosA = std::cos(rad);
    double sinA = std::sin(rad);
    double fCosA = 1.0 - cosA;

    Matrix44 ans;
    ans.Identity();

    ans.m[0][0] = cosA + ux * ux * fCosA;
    ans.m[0][1] = ux * uy * fCosA - uz * sinA;
    ans.m[0][2] = ux * uz * fCosA + uy * sinA;

    ans.m[1][0] = uy * ux * fCosA + uz * sinA;
    ans.m[1][1] = cosA + uy * uy * fCosA;
    ans.m[1][2] = uy * uz * fCosA - ux * sinA;

    ans.m[2][0] = uz * ux * fCosA - uy * sinA;
    ans.m[2][1] = uz * uy * fCosA + ux * sinA;
    ans.m[2][2] = cosA + uz * uz * fCosA;

    return ans;
}