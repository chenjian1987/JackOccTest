#include "Matrix44.h"




Matrix44::Matrix44()
{
    SetIdentity();
}




void Matrix44::SetIdentity()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m[i][j] = (i == j) ? 1 : 0;
        }
    }
}

Matrix44 Matrix44::Translation(double x, double y, double z)
{

}
