#pragma once

#include <map>
#include "DbObjectContainer.h"


namespace JackC
{
    template <class Key,class T,class KeyCompare=std::less<Key>>
    class NdbMap:public DbObjectContainer
    {
        typedef std::true_type is_map;
    public:
        typedef typename std::map<Key, T, KeyCompare>::key_type        key_type;
        typedef typename std::map<Key, T, KeyCompare>::value_type      value_type;
        typedef typename std::map<Key, T, KeyCompare>::size_type       size_type;
        typedef typename std::map<Key, T, KeyCompare>::Key_compare     key_compare_type;

        typedef typename std::map<Key, T, KeyCompare>::iterator          iterator;
        typedef typename std::map<Key, T, KeyCompare>::const_iterator    const_iterator;
        typedef typename std::map<Key, T, KeyCompare>::reverse_iterator  reverse_iterator;
        typedef typename std::map<Key, T, KeyCompare>::const_reverse_iterator  const_reverse_iterator;
    private:
        std::map<Key, T, KeyCompare> m_map;
    public:
        NdbMap(){}
        NdbMap(const NdbMap& another)
        {
            m_map = another.m_map;
        }
        NdbMap(const std::map<Key, T, KeyCompare>& map)
        {
            m_map = map;
        }

        NdbMap& operator=(const NdbMap& another)
        {
            if (this != &another)
            {
                m_map = another.m_map;
            }
            return *this;
        }
        NdbMap& operator=(NdbMap&& another)
        {
            if (this != &another)
            {
                m_map = std::move(another.m_map);
            }
            return *this;
        }

        NdbMap& operator=(const std::map<Key, T, KeyCompare>& map)
        {
            if (&m_map != &map)
            {
                m_map = map;
            }
            return *this;
        }
        NdbMap& operator=(const std::map<Key, T, KeyCompare>&& map)
        {
            if (&m_map != &map)
            {
                m_map = std::move(map);
            }
            return *this;
        }

        const std::map<Key, T, KeyCompare>& StdMap() const
        {
            return m_map;
        }
        std::map<Key, T, KeyCompare>& StdMapFW()
        {
            return m_map;
        }

        operator const std::map<Key, T, KeyCompare>& () const
        {
            return m_map;
        }
        operator std::map<Key, T, KeyCompare>& () 
        {
            return m_map;
        }
        //iterator
    public:
        iterator begin()
        {
            return m_map.begin();
        }
        iterator end()
        {
            return m_map.end();
        }
        reverse_iterator rbegin()
        {
            return m_map.rbegin(); //反向遍历，指向map最后一个元素
        }
        const_reverse_iterator rbegin() const
        {
            return m_map.rbegin();
        }

        reverse_iterator rend()
        {
            return m_map.rend();    
        }

        const_reverse_iterator rend() const
        {
            return m_map.rend();
        }

        const_iterator cbegin() const
        {
            return m_map.begin();
        }

        const_iterator cend() const
        {
            return m_map.cend(); //最后一个元素的旁边
        }

        const_reverse_iterator crbegin() const
        {
            return m_map.rbegin();
        }

        const_reverse_iterator crend() const
        {
            return m_map.rend();
        }
        //modifiers
        public:
            template <class P>
            std::pair<iterator, bool> insert(P&& val)
            {
                return m_map.insert(std::forward<P>(val));
            }
            size_type erase(const Key& k)
            {
                return m_map.erase(k);   //移除元素
            }
            void clear()
            {
                m_map.clear();
            }
            // Capacity
    public:
        bool empty() const
        {
            return m_map.empty();
        }

        size_type size() const
        {
            return m_map.size();
        }

        size_type max_size() const
        {
            return m_map.max_size();
        }
        //Element access
    public:
        T& operator[] (const Key& k)
        {
            return m_map[k];
        }
        T& operator[](Key&& k)
        {
            return m_map[std::forward<Key>(k)];
        }
        T& at(const Key& k)
        {
            return m_map.at(k);
        }

        const T& at(const Key& k) const
        {
            return m_map.at(k);
        }
        //operations
    public:
        iterator find(const key_type& k)
        {
            return m_map.find(k);
        }
        const_iterator find(const key_type& k) const
        {
            return m_map.find(k);
        }

        size_type count(const key_type& k) const
        {
            return m_map.count(k);
        }

        iterator lower_bound(const key_type& k)
        {
            return m_map.lower_bound(k);
        }

        const_iterator lower_bound(const key_type& k) const
        {
            return m_map.lower_bound(k);
        }

        iterator upper_bound(const key_type& k)
        {
            return m_map.upper_bound(k);
        }

        const_iterator upper_bound(const key_type& k) const
        {
            return m_map.upper_bound(k);
        }

        std::pair<iterator, iterator> equal_range(const key_type& k)
        {
            return m_map.equal_range(k);
        }

        std::pair<const_iterator, const_iterator> equal_range(const key_type& k) const
        {
            return m_map.equal_range(k);
        }



    };
}