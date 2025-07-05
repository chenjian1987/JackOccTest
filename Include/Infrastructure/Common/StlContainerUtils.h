#pragma once
#include "ElementId.h"
#include <string>
#include <iterator>
#include <algorithm>
#include <vector>
#include <list>
#include <set>
#include <deque>
#include "JCommonDefine.h"
#include <map>


namespace JackC
{
    template<typename T>
    struct type_trait {};
    struct is_sequence_container { static const bool sequence = true;};
    struct is_associative_container { static const bool associative = true;};

    struct is_std_vector : public is_sequence_container
    { static const bool std_vector = true; };
    struct is_std_deque : public is_sequence_container
    { static const bool std_deque = true; };
    struct is_std_list : public is_sequence_container
    { static const bool std_list = true; };
    struct is_std_set  : public is_associative_container
    { static const bool std_set = true; };
    struct is_std_multiset : public is_associative_container
    { static const bool std_multiset = true; };
    struct is_std_map : public is_associative_container
    { static const bool std_map = true; };
    struct is_std_multimap : public is_associative_container
    { static const bool std_multimap = true; };
    struct is_std_unordered_set : public is_associative_container
    { static const bool std_unordered_set = true; };
    struct is_std_unordered_multiset : public is_associative_container
    { static const bool std_unordered_multiset = true; };
    struct is_std_unordered_map : public is_associative_container
    { static const bool std_unordered_map = true; };
    struct is_std_unordered_multimap : public is_associative_container
    { static const bool std_unordered_multimap = true; };
    struct is_owner_ptr { static const bool owner_ptr = true; };


    //判断是否是std::vector
    template<typename T, typename Alloc>
    struct type_trait<std::vector<T, Alloc> >
        : public is_std_vector{};

    //判断是否是std::deque
    template<typename T, typename Alloc>
    struct type_trait<std::deque<T, Alloc> >
        : public is_std_deque{};
    
    //判断是否是std::list
    template<typename T, typename Alloc>
    struct type_trait<std::list<T, Alloc> >
        : public is_std_list{};
    
    //判断是否是std::set
    template<typename T, typename Comp, typename Alloc>
    struct type_trait<std::set<T, Comp, Alloc> >
        : public is_std_set{};

    //判断是否是std::multiset
    template<typename T, typename Comp, typename Alloc>
    struct type_trait<std::multiset<T, Comp, Alloc> >
        : public is_std_multiset{};

    //判断是否是std::unordered_set
    template<typename T, typename Comp, typename Alloc>
    struct type_trait<std::unordered_set<T, Comp, Alloc> >
        : public is_std_unordered_set {};

    //判断是否是std::unordered_multiset
    template<typename T, typename Comp, typename Alloc>
    struct type_trait<std::unordered_multiset<T, Comp, Alloc> >
        : public is_std_unordered_multiset {};

    //判断是否是std::map
    template<typename Key, typename Value, typename Comp, typename Alloc>
    struct type_trait<std::map<Key, Value, Comp, Alloc> >
        : public is_std_map {};

    //判断是否是std::multimap
    template<typename Key, typename Value, typename Comp, typename Alloc>
    struct type_trait<std::multimap<Key, Value, Comp, Alloc> >
        : public is_std_multimap {};

    //判断是否是std::unordered_map
    template<typename Key, typename Value, typename Comp, typename Alloc>
    struct type_trait<std::unordered_map<Key, Value, Comp, Alloc> >
        : public is_std_unordered_map {};

    //判断是否是std::unordered_multimap
    template<typename Key, typename Value, typename Comp, typename Alloc>
    struct type_trait<std::unordered_multimap<Key, Value, Comp, Alloc> >
        : public is_std_unordered_multimap {};

    //判断是否是OwnerPtr
    template<typename T, typename Deleter>
    struct type_trait<OwnerPtr<T, Deleter> > : public is_owner_ptr {};

    //重载== for vector/set
    template<typename T
        , typename VAlloc
        , typename SComp
        , typename SAlloc>
        bool operator==(const std::vector<T, VAlloc>& vs, const std::set<T, SComp, SAlloc>& st)
    {
        if (vs.size() != vs.size())
            return false;

        for (auto& v : vs)
        {
            if (st.find(v) == st.end())
                return false;
        }
        return true;
    }

    //重载== for vector/set
    template<typename T
        , typename VAlloc
        , typename SComp
        , typename SAlloc>
        bool operator==(const std::vector<T, VAlloc>& vs, const std::multiset<T, SComp, SAlloc>& st)
    {
        if (vs.size() != vs.size())
            return false;

        for (auto& v : vs)
        {
            if (st.find(v) == st.end())
                return false;
        }
        return true;
    }

    //重载== for vector/set
    template<typename T
        , typename VAlloc
        , typename SComp
        , typename SAlloc>
        bool operator==(const std::set<T, SComp, SAlloc>& st, const std::vector<T, VAlloc>& vs)
    {
        return operator==(vs, st);
    }

    //重载== for vector/set
    template<typename T
        , typename VAlloc
        , typename SComp
        , typename SAlloc>
        bool operator==(const std::multiset<T, SComp, SAlloc>& st, const std::vector<T, VAlloc>& vs)
    {
        return operator==(vs, st);
    }

    //////////////////////////////////////////////////////////////////////////
    //存值型容器相关算法（即容器内部直接存放对象）
    //////////////////////////////////////////////////////////////////////////
    class JACKC_COMMON_INTERFACE_EXPORT StlContainerUtils
    {
    public:
        template <typename T>
        static void InsertAtPostion(int index, const T& data, std::vector<T>& vector)
        {
            std::vector<T> resultVector;
            std::copy(vector.begin(), vector.begin() + index, std::back_inserter(resultVector));
            resultVector.push_back(data);
            std::copy(vector.begin() + index, vector.end(), std::back_inserter(resultVector));
            std::swap(vector, resultVector);
        }

        template<typename IndexType
            , typename Container
            , class = typename std::enable_if_t<
            std::is_arithmetic<std::remove_reference_t<IndexType>>::value, void>>
        static bool IsInContainerRange(const Container& container, IndexType index)
        {
            return index >= 0 && index < (IndexType)container.size();
        }

        template<class T, class Container>
        static bool IsInContainer(const Container& container, const T& item)
        {
            return std::find(container.begin(), container.end(), item) != container.end();
        }

        template<class T, class Container, class = std::enable_if_t<
            type_trait<Container>::std_set
            || type_trait<Container>::std_multiset
            || type_trait<Container>::std_map
            || type_trait<Container>::std_multimap
            || type_trait<Container>::std_unordered_multiset
            || type_trait<Container>::std_unordered_map
            || type_trait<Container>::std_unordered_multimap, void> >
            static bool IsInContainer(const Container& container, const T& item)
        {
            return container.find(item) != container.end();
        }

        template<class ContainerA, class ContainerB>
        static bool IsSameContentContainer(const ContainerA& containerA, const ContainerB& containerB)
        {
            return operator== (containerA, containerB);
        }

        template <class T>
        static void RemoveByIndex(std::vector<T>& vec, size_t pos)
        {
            if (!IsInContainerRange(vec, pos))
                return;

            vec.erase(vec.begin() + pos);
        }
        
        //从一个容器中移除一个元素（一般为vector/set，需要保证容器元素类型一致）
        template <class ContainerA, class T>
        static bool RemoveItem(ContainerA& arr, const T& item)
        {
            auto iter = std::find(arr.begin(), arr.end(), item);
            if (iter != arr.end())
            {
                arr.erase(iter);
                return true;
            }

            return false;
        }

        template<class A, class B>
        static bool RemoveItems(std::set<A>& arr, const std::set<B>& items)
        {
            auto count = arr.size();

            std::set<A> result;
            std::set_difference(arr.begin(), arr.end(), items.begin(), items.end(), std::inserter(result, result.end()));
            std::swap(arr, result);

            return count!=arr.size();
        }

        //////////////////////////////////////////////////////////////////////////
        //条件删除
        template <class T, class Predicate>
        static bool RemoveItemsIf(std::vector<T>& arr, Predicate removeHelper)
        {
            auto count = arr.size();
            arr.erase(std::remove_if(arr.begin(), arr.end(), removeHelper), arr.end());
            return count!=arr.size();
        }

        //将容器from中数据添加到vector容器To的末尾
        //1. 需要保证from容器内部数据类型为T
        //2. 可能会有添加重复元素
        template<typename T, class ContainerFrom>
        static bool AddItems(std::vector<T>& to, const ContainerFrom& from)
        {
            auto count = to.size();
            to.reserve(to.size() + from.size());
            to.insert(to.end(), from.begin(), from.end());
            return count!=to.size();
        }

        //将容器from中数据插入到set容器中
        //1. 需要保证from容器内部数据类型为T
        //2. set保证不会添加重复元素(相当于merge)
        template<typename T, class ContainerFrom>
        static bool AddItems(std::set<T>& to, const ContainerFrom& from)
        {
            auto count = to.size();
            to.insert(from.begin(), from.end());
            return count!=to.size();
        }

        //将容器from中数据复制到vector容器中(覆盖原来数据)
        //1. 需要保证from容器内部数据类型为T
        //2. from可以是vector或者set
        template<typename T, class ContainerFrom>
        static bool CopyItems(std::vector<T>& to, const ContainerFrom& from)
        {
            to.clear();
            to.reserve(from.size());
            to.insert(to.end(), from.begin(), from.end());
            return !to.empty();
        }

        static bool HasRepeatItem(const std::vector<ElementId>& container);

        static void RemoveRepeatItem(std::vector<ElementId>& container);

        template <class ContainerA, class T>
        static void PushBackNoRepeatItem(ContainerA& arr, const T& item)
        {
            auto iter = std::find(arr.begin(), arr.end(), item);
            if (iter == arr.end())
            {
                arr.push_back(item);
            }
        }

        //条件删除
        template <typename T, typename Predicate>
        static bool RemoveItemsIf(std::set<T>& arr, Predicate removeHelper)
        {
            auto count = arr.size();
            for (auto iter = arr.begin(); iter != arr.end(); )
            {
                //判断如果此元素应该被删除，则erase它
                if (removeHelper(*iter))
                {
                    arr.erase(iter++);
                }
                else
                {
                    ++iter;
                }
            }

            return count != arr.size();
        }
    };

    //////////////////////////////////////////////////////////////////////////
    //存指针型容器相关算法(目前只考虑容器中存放智能指针)
    //////////////////////////////////////////////////////////////////////////
    class JACKC_COMMON_INTERFACE_EXPORT OwnerPtrContainerUtils
    {
    public:
        //二元操作符：等于
        template<class T>
        class EqualPredicate
        {
        public:
            bool operator()(const JackC::OwnerPtr<T>& opLv, const JackC::OwnerPtr<T>& opRv) const
            {
                if (opLv == nullptr && opRv == nullptr)
                {
                    return true;
                }

                if (opLv == nullptr || opRv == nullptr)
                {
                    return false;
                }

                return (*opLv).EqualTo(*opRv);
            }
        };

        //二元操作符：小于
        template<class T>
        class LessPredicate
        {
        public:
            bool operator()(const JackC::OwnerPtr<T>& opLv, const JackC::OwnerPtr<T>& opRv) const
            {
                if (opLv == nullptr || opRv == nullptr)
                {
                    return false;
                }

                return (*opLv).LessThan(*opRv);
            }
        };

        //一元操作符：等于
        template <class T>
        class EqualToPredicate
        {
        public:
            EqualToPredicate(const JackC::OwnerPtr<T>& val) :m_val(val.get()) {}
            EqualToPredicate(const T& val) :m_val(&val) {}
            EqualToPredicate(const T* val) :m_val(val) {}

        public:
            bool operator()(const JackC::OwnerPtr<T>& val) const
            {
                return (*m_val).EqualTo(*val);
            }

        private:
            const T* m_val;
        };

        //判断元素是否在容器内
        template<class Container, class T>
        static bool IsInContainer(const Container& container, const JackC::OwnerPtr<T>& item)
        {
            auto iter = std::find_if(container.begin(), container.end(), OwnerPtrContainerUtils::EqualToPredicate<T>(item));
            return iter != container.end();
        }

        //判断元素是否在容器内
        template<class Container, class T>
        static bool IsInContainer(const Container& container, const T& item)
        {
            auto iter = std::find_if(container.begin(), container.end(), OwnerPtrContainerUtils::EqualToPredicate<T>(item));
            return iter != container.end();
        }

        //判断元素是否在容器内
        template<class Container, class T>
        static bool IsInContainer(const Container& container, const T* item)
        {
            if (item == nullptr)
                return false;

            auto iter = std::find_if(container.begin(), container.end(), OwnerPtrContainerUtils::EqualToPredicate<T>(item));
            return iter != container.end();
        }

        //从容器中移除一个元素
        template <class Container, class T>
        static bool RemoveItem(Container& container, const JackC::OwnerPtr<T>& item)
        {
            auto iter = std::find_if(container.begin(), container.end(), EqualToPredicate<T>(item));
            if (iter != container.end())
            {
                container.erase(iter);
                return true;
            }

            return false;
        }

        //从容器中移除一个元素
        template <class Container, class T>
        static bool RemoveItem(Container& container, const T& item)
        {
            auto iter = std::find_if(container.begin(), container.end(), EqualToPredicate<T>(item));
            if (iter != container.end())
            {
                container.erase(iter);
                return true;
            }

            return false;
        }

        //从容器中移除一个元素
        template <class Container, class T>
        static bool RemoveItem(Container& container, const T* item)
        {
            auto iter = std::find_if(container.begin(), container.end(), EqualToPredicate<T>(item));
            if (iter != container.end())
            {
                container.erase(iter);
                return true;
            }

            return false;
        }

        //////////////////////////////////////////////////////////////////////////
        //删除数组中所有满足特定条件的元素，用户只需要重载RemoveHelper实现删除条件
        //Please referer to C++ concepts: Predicate
        //RemoveItemsHelper默认删除器（删除一堆元素）
        template <class T>
        class RemoveItemsHelper
        {
        public:
            RemoveItemsHelper(const std::vector<JackC::OwnerPtr<T>>& val) :m_target(val) {}
            bool operator()(const JackC::OwnerPtr<T>& val) const
            {
                return OwnerPtrContainerUtils::IsInContainer(m_target, val);
            }

        private:
            std::vector<JackC::OwnerPtr<T>>& m_target;
        };

        template <class T, class Predicate>
        static bool RemoveItemsIf(std::vector<T>& arr, Predicate removeHelper)
        {
            auto count = arr.size();
            arr.erase(std::remove_if(arr.begin(), arr.end(), removeHelper), arr.end());
            return count != arr.size();
        }

        //添加元素到容器
        template<class Container, class T>
        static bool AddItem(Container& to, JackC::OwnerPtr<T> item)
        {
            to.insert(to.end(), TransferOwnership(item));
            return true;
        }

        //添加元素到容器
        template<class Container, class T>
        static bool AddItem(Container& to, const T* item)
        {
            if (item==nullptr)
            {
                return false;
            }
            return OwnerPtrContainerUtils::AddItem(to, item->Clone());
        }

        //添加元素到容器
        template<class Container, class T>
        static bool AddItem(Container& to, const T& item)
        {
            return OwnerPtrContainerUtils::AddItem(to, &item);
        }

        //将容器from中数据添加到to容器中
        template<class ContainerTo, class ContainerFrom>
        static bool AddItems(ContainerTo& to, const ContainerFrom& from)
        {
            auto count = to.size();
            for (auto iter = from.begin(); iter != from.end(); ++iter)
            {
                //作为容器元素的类型必需提供一个Clone复制方法
                OwnerPtrContainerUtils::AddItem(to, (*iter)->Clone());
            }
            return count != to.size();
        }
        
        template<class T, class ContainerFrom>
        static bool AddItems(std::vector<T>& to, const ContainerFrom& from)
        {
            auto count = to.size();
            to.reserve(to.size() + from.size());
            for (auto iter = from.begin(); iter != from.end(); ++iter)
            {
                //作为容器元素的类型必需提供一个Clone复制方法
                OwnerPtrContainerUtils::AddItem(to, (*iter)->Clone());
            }
            return count != to.size();
        }

        //将容器from中数据复制到to容器中(清空to容器)
        template<class ContainerTo, class ContainerFrom>
        static bool CopyItems(ContainerTo& to, const ContainerFrom& from)
        {
            to.clear();
            return OwnerPtrContainerUtils::AddItems(to, from);
        }

        template<class ContainerA, class ContainerB>
        static bool IsSameContentContainer(const ContainerA& containerA, const ContainerB& containerB)
        {
            if (containerA.size() != containerB.size())
            {
                return false;
            }

            for (auto iter = containerA.begin(); iter != containerA.end(); ++iter)
            {
                if (!OwnerPtrContainerUtils::IsInContainer(containerB, *iter))
                {
                    return false;
                }
            }

            return true;
        }

        //将SET型容器转换为VECTOR型容器
        template<class T, class Less = OwnerPtrContainerUtils::LessPredicate<T>>
        static std::vector<JackC::OwnerPtr<T>> TransformSetToVector(const std::set<JackC::OwnerPtr<T>, Less>& refInput)
        {
            std::vector<JackC::OwnerPtr<T>> refOutput;
            OwnerPtrContainerUtils::CopyItems(refOutput, refInput);
            return std::move(refOutput);
        }

        //将VECTOR型容器转换为SET型容器
        template<class T, class Less = OwnerPtrContainerUtils::LessPredicate<T>>
        static std::set<JackC::OwnerPtr<T>, Less> TransformVectorToSet(const std::vector<JackC::OwnerPtr<T>>& refInput)
        {
            std::set<JackC::OwnerPtr<T>, Less> refOutput;
            OwnerPtrContainerUtils::CopyItems(refOutput, refInput);
            return std::move(refOutput);
        }

        //从容器A中移除所有容器B的元素
        template <class ContainerA, class ContainerB>
        static bool RemoveItems(ContainerA& containerA, ContainerB& containerB)
        {
            auto count = containerA.size();

            for (auto iterB = containerB.begin(); iterB != containerB.end(); ++iterB)
            {
                OwnerPtrContainerUtils::RemoveItem(containerA, *iterB);
            }

            return count != containerA.size();
        }

        template <class T, class Predicate>
        static bool RemoveItemsIf(std::set<T>& arr, Predicate removeHelper)
        {
            auto count = arr.size();
            for (auto iter = arr.begin(); iter != arr.end(); )
            {
                //判断如果此元素应该被删除，则erase它
                if (removeHelper(*iter))
                {
                    arr.erase(iter++);
                }
                else
                {
                    ++iter;
                }
            }

            return count != arr.size();
        }
    };
}
