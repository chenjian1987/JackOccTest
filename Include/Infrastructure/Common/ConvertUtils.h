#pragma once
#include "JCommonDefine.h"

class JACK_COMMON_INTERFACE_EXPORT ConvertUtils
{
public:
	ConvertUtils();
	~ConvertUtils();

public:
	//2d点转换为3d
	static AcGePoint3d toPoint3d(const AcGePoint2d& pt,double z=0);
	//3d点转换为2d
	static AcGePoint2d toPoint2d(const AcGePoint3d& pt);

	//角度转换为弧度
	static double angleToRadian(double angle);
	//弧度转换为角度
	static double radianToAngle(double radian);
	//static CString toString(int val);

	//点坐标系转换：世界坐标系WCS-->显示坐标系DCS
	static AcGePoint3d wcsPointToDcsPoint(const AcGePoint3d& pt);
	//点坐标系转换：用户坐标系UCS--->世界坐标系WCS
	static AcGePoint3d ucsPointToWcsPoint(const AcGePoint3d& pt);
	////点坐标系转换：世界坐标系WCS-->用户坐标系UCS
	static AcGePoint3d wcsPointToUcsPoint(const AcGePoint3d& pt);
};
