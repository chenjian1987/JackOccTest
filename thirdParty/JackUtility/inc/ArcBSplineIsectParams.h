#pragma once
#include "GeometryUtilityOccDefine.h"




struct ArcBSplineIsectParams
{
    Standard_Real tol3d = 1.0e-7;            // 结果点“去重”的空间距离容差
    Standard_Real tolNear = 1.0e-5;          // Extrema/Discretize 的“近似相交”阈值
    Standard_Integer discretSamples = 200;   // Discretize 采样点个数
    bool refineOnSurface = true;             // 是否对结果点做“投影回曲面”的精修

};