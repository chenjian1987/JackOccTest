#pragma once
#include "GeometryUtilityDefine.h"
#include "GeometryUtilityOccDefine.h"









class JACKC_GEOM_UT_EXPORT GeomPointPlanRelationshipUtility
{
public:
    // 调用 gp_Pln Distance接口，只适合平面
    bool IsPointOnPlaneByDistance(const gp_Pnt& point, double A, double B, double C, double D);

    // 数学方式实现, 只适合平面
    bool IsPointOnPlaneByMath(const gp_Pnt& point, double A, double B, double C, double D);

    // 通过 投影器 
    bool IsPointOnPlaneByProject(const gp_Pnt& point, double A, double B, double C, double D);

    // 通过 分析工具
    bool IsPointOnPlaneByShapeAnalysis(const gp_Pnt& point, double A, double B, double C, double D);

    // 通过 极值计算
    bool IsPointOnPlaneByExtrema(const gp_Pnt& point, double A, double B, double C, double D);

    // 通过 GeomAdaptor
    bool IsPointOnPlaneByGeomAdaptor(const gp_Pnt& point, double A, double B, double C, double D);


    // 通过 面域分类器
    bool IsPointOnPlaneByFaceClassifier(const gp_Pnt& point, double A, double B, double C, double D);

    // 通过 距离计算
    bool IsPointOnPlaneByDistShapeShape(const gp_Pnt& point, double A, double B, double C, double D);







};