#pragma once

#include "JDevService.h"
#include "JCommonDefine.h"

namespace JackC
{
    //部分常量值定义
    class JACKC_COMMON_INTERFACE_EXPORT  Constants
    {
    public:

        // DOUBLE_EPS =0.000001, 用于对两个没有单位的双精度浮点数进行比较，比如视图缩放比例等。
        static const double DOUBLE_EPS;

        // FLOAT_EPS = 0.001f 用于对两个没有单位的单精度浮点数进行比较，比如视图缩放比例等。
        static const double FLOAT_EPS;

        // LENGTH_EPS = 1E-2(即,0.01mm), 用于对两个长度类型的浮点数进行比较，比如判断两条线段是否等长、两点之间的距离值等。
        // 注：内部长度单位为毫米
        static const double LENGTH_EPS;

        // ANGLE_EPS = 1E-4, 用于对两个角度类型的浮点数进行比较，比如判断两条直线之间的夹角等。
        // 注：内部角度单位为弧度
        static const double ANGLE_EPS;

        // MIN_MODEL_SIZE = 1.0(即,1mm), 用于确定单个构件允许的最小尺寸，比如判断一面墙的最短长度等。
        // 注：构件的部分细节尺寸允许小于MIN_MODEL_SIZE，但构件的整体尺寸（包围盒）必须在所有维度上大于该值。
        static const double MIN_MODEL_SIZE;

        // MAX_MODEL_SIZE = = 1E6(即,1km),用于确定单个构件允许的最大尺寸，比如判断一面墙的最长长度等。
        // 注：构件的整体尺寸（包围盒）必须在所有维度上小于该值。
        static const double MAX_MODEL_SIZE;

        //同MAX_MODEL_SIZE, MAX_NEG_MODEL_SIZE=-1E6
        //只是这是一个负值，表示反向
        static const double MIN_NEG_MODEL_SIZE;

        // MAX_MODEL_SPACE_SIZE = 1E7(即,10km), 用于确定构件可以存在的世界坐标系空间范围。
        // 注：构件的包围盒任何一个维度都不应超出(-MAX_MODEL_SPACE_SIZE/2, MAX_MODEL_SPACE_SIZE/2)这一范围。
        static const double MAX_MODEL_SPACE_SIZE;

        // DISTANCE_ITERATION_EPS = 1E-4(即,0.0001mm), 用于在进行几何计算时确定距离计算精度。此容差值一般只在几何库封装层使用。
        // 注：对于使用迭代计算的算法，应该使用此容差作为迭代结束判断依据，而不能直接使用LENGTH_EPS。
        static const double DISTANCE_ITERATION_EPS;

        // MATH_INFINITY= 1E100, 全局的正无穷大值
        static const double MATH_INFINITY;

        // MATH_NEGINFINITY = -1E100,全局的负无穷大值
        static const double MATH_NEGINFINITY;

        //PI相关常数定义
        static const double MATH_PI_6;
        static const double MATH_PI_4;
        static const double MATH_PI_2;
        static const double MATH_PI;
        static const double MATH_3PI_2;
        static const double MATH_2PI;

        // 1inch=25.4mm
        static const double INCH_TO_MM;
    };
}
