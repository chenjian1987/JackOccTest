#pragma once
#include "JDevService.h"
#include <map>
#include <vector>
#include <string>
#include <math.h>
#include "JCommonDefine.h"

namespace JackC
{
    //1.赋值顺序不能变动，逻辑按照顺序进行判断处理，为的是之后方便单位扩展
//2.序号从EUT_Num到EUT_Double之间添加自然单位，是可进行计算的
    enum ExprUnitType
    {
        EUT_Invalid = 0,
        //能计算的从01开始
        EUT_Num,//数据，对应界面输入的数据 ex：1.3、10
        EUT_Length,
        EUT_Angle,
        EUT_Time,

        //如果还需要添加，从此处开始

        EUT_Int,   //目前只对EUT_Double取整时使用。
        EUT_Double,//对应界面输入的整数、数值

        //不能计算的从EUT_Bool开始
        EUT_Bool,
        EUT_String,
    };

    class JACKC_COMMON_INTERFACE_EXPORT  UnitMap
    {

    private:
#define Precisiondef 0.00001
        bool IsZero(const double& value)const
        {
            if (abs(value) < Precisiondef)
                return true;
            return false;
        }

        std::map<ExprUnitType, double> m_units;
        std::wstring m_errorStr;

    public:
        std::wstring GetErrorStr() const { return m_errorStr; }
        void SetErrorStr(const std::wstring& val) { m_errorStr = val; }

        //构造函数默认初始化为Num
        UnitMap()
        {
        };
        ~UnitMap() {};
        UnitMap(const ExprUnitType& type)
        {
            InitialFrom(type);
        }

        UnitMap(const UnitMap& types)
        {
            this->GetCopyFrom(types);
        }

        UnitMap& operator=(const UnitMap& types)
        {
            this->GetCopyFrom(types);
            return *this;
        }

        bool InitialFrom(const ExprUnitType& type, const double& value = 1)
        {
            if (!IsCalculatable(type))
                return false;
            Clear();
            m_units.insert(std::pair<ExprUnitType, double>(type, value));
            return true;
        }

        //判断是有效的单位
        bool IsValid() const
        {
            // bool型、字符串型的m_units为空，应该认为是有效的
            if (m_units.size() == 0)
                return true;
            for (auto unit = m_units.begin(); unit != m_units.end(); unit++)
            {
                ExprUnitType type = unit->first;
                if (!IsCalculatable(type) && !IsZero(unit->second))
                    return false;
            }
            return true;
        }

        bool IsNum() const
        {
            if (m_units.size() == 1)
            {
                for (auto unit = m_units.begin(); unit != m_units.end(); unit++)
                {
                    if (unit->first == EUT_Num)
                        return true;
                }
            }
            return false;
        }

        bool IsNaturalUnit() const
        {
            if (!IsValid())
                return false;
            for (int i = EUT_Num; i <= EUT_Double; i++)
            {
                ExprUnitType type = (ExprUnitType)i;
                if (GetUnitCount(type) != 0)
                    return true;
            }
            return false;
        }

        bool MapHasType(const ExprUnitType& type)
        {
            if (m_units.end() != m_units.find(type))
                return true;
            return false;
        }

        bool IsNumOrNatureUnit() const
        {
            if (IsNum() || IsNaturalUnit())
                return true;
            return false;
        }

        static bool IsCalculatable(const ExprUnitType& type)
        {
            int i = (int)type;
            if (EUT_Double >= i && i >= EUT_Num)
                return true;
            return false;
        }

        static bool IsNaturalUnit(const ExprUnitType& type)
        {
            int i = (int)type;
            if (EUT_Double > i && i > EUT_Num)
                return true;
            return false;
        }

        //获得单一单位：如果是单一单位,则返回单位;
        //如果非,则返回EUT_Invalid
        ExprUnitType GetSingleUnit() const
        {
            if (!IsValid())
                return EUT_Invalid;
            if (IsNum())
                return EUT_Num;
            ExprUnitType type = EUT_Double;
            bool isSet = false;
            for (auto unit = m_units.begin(); unit != m_units.end(); unit++)
            {
                if (unit->first == EUT_Num || unit->first == EUT_Int || unit->first == EUT_Double)
                    continue;

                if (!IsZero(unit->second) && abs(unit->second - 1) > Precisiondef)
                    return EUT_Invalid;
                else if (abs(unit->second - 1) < Precisiondef && isSet == false)
                {
                    type = unit->first;
                    isSet = true;
                }
                else if (abs(unit->second - 1) < Precisiondef && isSet == true)
                    return EUT_Invalid;
            }
            return type;
        }

        void Clear()
        {
            m_units.clear();
        }

        void GetCopyFrom(const UnitMap& typeMap)
        {
            Clear();
            for (auto it = typeMap.m_units.begin();
                it != typeMap.m_units.end();
                it++)
            {
                m_units.insert(std::pair<ExprUnitType, double>(it->first, it->second));
            }
        }

        bool UnitIsSame(const UnitMap& typeMap) const
        {
            std::vector<ExprUnitType> types1;
            this->FindUnits(types1);
            std::vector<ExprUnitType> types2;
            typeMap.FindUnits(types2);
            if (types1.size() != types2.size())
                return false;
            if (types1.size() == 1)
            {
                if (types1.at(0) != types2.at(0))
                    return false;
            }
            for (auto t = types1.begin(); t != types1.end(); t++)
            {
                if (*t == EUT_Num)
                    continue;
                if (!IsZero(GetUnitCount(*t) - typeMap.GetUnitCount(*t)))
                    return false;
            }
            return true;
        }

        double GetUnitCount(const ExprUnitType& type) const
        {
            auto it = m_units.find(type);
            if (m_units.end() == it)
                return 0;
            else
                return it->second;
        }

        void FindUnits(std::vector<ExprUnitType>& types) const
        {
            for (auto unit = m_units.begin(); unit != m_units.end(); unit++)
            {
                ExprUnitType type = unit->first;
                if (!IsZero(unit->second))
                    types.push_back(type);
            }
        }

        void Pow(const double& value)
        {
            for (std::map<ExprUnitType, double>::iterator it = m_units.begin();
                it != m_units.end();
                it++)
            {
                ExprUnitType type = it->first;
                if (!IsZero(it->second) && (IsNaturalUnit(type)))
                    it->second = it->second * value;
            }
        }

        bool Multiply(const ExprUnitType& type, const double& value = 1)
        {
            if (!IsCalculatable(type))
                return false;
            if (type == EUT_Num)
            {
                if (m_units.size() > 0)
                    return true;
            }
            else if (type == EUT_Double)
            {
                if (IsNaturalUnit())
                    return true;
                else if (IsNum())
                    m_units.clear();
            }
            else
            {
                if (IsNum())
                    m_units.clear();
                if (m_units.end() != m_units.find(EUT_Double))
                    m_units.find(EUT_Double)->second = 0;
            }

            auto it = m_units.find(type);
            if (m_units.end() == it)
            {
                m_units.insert(std::pair<ExprUnitType, double>(type, value));
            }
            else
            {
                it->second = it->second + value;
                if (type == EUT_Num || type == EUT_Double)
                    it->second = 1;
            }
            return true;
        }
        bool Divided(const ExprUnitType& type, const double& value = 1)
        {
            if (!IsCalculatable(type))
                return false;
            if (type == EUT_Double)
            {
                if (m_units.size() > 0)
                    return true;
            }
            else if (type == EUT_Num)
            {
                if (IsNumOrNatureUnit())
                    return true;
            }
            else
            {
                if (IsNum())
                    m_units.clear();
                if (m_units.end() != m_units.find(EUT_Double))
                    m_units.find(EUT_Double)->second = 0;
            }
            auto it = m_units.find(type);
            if (m_units.end() == it)
            {
                double count = value;
                if (type == EUT_Num || type == EUT_Double)
                    count = 1;
                m_units.insert(std::pair<ExprUnitType, double>(type, count));
            }
            else
            {
                it->second = it->second - value;
                if (type == EUT_Num || type == EUT_Double)
                    it->second = 1;
            }
            return true;
        }
        bool Multiply(const UnitMap& typeMap)
        {
            std::vector<ExprUnitType> unitTypes;
            typeMap.FindUnits(unitTypes);
            if (unitTypes.size() < 1)
                return true;
            for (auto t = unitTypes.begin(); t != unitTypes.end(); t++)
            {
                double count = typeMap.GetUnitCount(*t);
                if (!IsZero(count))
                    Multiply(*t, count);
            }
            return true;
        }
        bool Divided(const UnitMap& typeMap)
        {
            std::vector<ExprUnitType> unitTypes;
            typeMap.FindUnits(unitTypes);
            if (unitTypes.size() < 1)
                return true;
            for (auto t = unitTypes.begin(); t != unitTypes.end(); t++)
            {
                double count = typeMap.GetUnitCount(*t);
                if (!IsZero(count))
                    Divided(*t, count);
            }
            return true;
        }
    };

    class JACKC_COMMON_INTERFACE_EXPORT  UnitHelper
    {
    public:
        static bool CheckUnitAdd(const UnitMap& left, const UnitMap& right, UnitMap& result);
        static bool CheckUnitSubtract(const UnitMap& left, const UnitMap& right, UnitMap& result);
        static bool CheckUnitMultiply(const UnitMap& left, const UnitMap& right, UnitMap& result);
        static bool CheckUnitDivided(const UnitMap& left, const UnitMap& right, UnitMap& result);
        static bool CheckUnitModulus(const UnitMap& left, const UnitMap& right, UnitMap& result);
        static bool CheckUnitCompare(const UnitMap& left, const UnitMap& right, UnitMap& result);

    };

}

