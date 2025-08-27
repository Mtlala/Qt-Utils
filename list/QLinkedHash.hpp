/**
 * @file	QLinkedHash.hpp
 * @brief	实现了一个基于Q Hash &QList 模板类，提供基于键值对的有序哈希表功能
 *
 * QLinkedHash 使用 QHash 和 QList 组合实现，既支持快速查找（通过 QHash），
 * 又支持按插入顺序访问（通过 QList）。
 *
 * @tparam T 存储的值的类型。
 */

#pragma once

#include <QList>
#include <QHash>

template<typename Key, typename T>
class QLinkedHash
{
	static_assert(!std::is_same<int, Key>::value,
		"The Key type of 'QLinkedHash' can't be 'int', and have to be supported by 'qHash()'");
public:
	QLinkedHash() {}
	QLinkedHash(const QLinkedHash& other)
		: m_hash(other.m_hash), m_list(other.m_list) {}
	~QLinkedHash() { m_list.clear(); m_hash.clear(); }

	/**
	 * @brief			获取容器中元素的数量
	 * @return <int>	容器中元素的数量
	 */
	size_t size() const { return m_list.size(); }

	/**
	 * @brief 将当前容器中的值转换为指定类型的列表
	 * 
	 * @tparam ConvertT 目标类型，用于转换容器中的值
	 * @return QList<ConvertT> 包含所有元素的列表，元素类型为 ConvertT
	 */
	template<typename ConvertT>
	inline const QList<ConvertT> toList() const 	
	{ 
		QList<ConvertT> _list;

		for (int _i = 0; _i < m_list.size(); ++_i)
		{
			_list.push_back(ConvertT(m_hash[m_list[_i]]));
		}
		return _list;
	}

	/**
	 * @brief	按下标索引访问元素
	 *
	 * 根据指定的索引返回对应的值。如果索引超出范围，则返回默认构造的 T 对象
	 *
	 * @param	idx <int> 索引值，必须在 [0, size()) 范围内
	 * @return	<const T> 如果索引有效，返回对应的值；否则返回默认构造的 T 对象
	 * 
	 * @note1	暂不支持返回引用直接赋值, 没有对应场景。而且因为[]运算符有
	 *			可能会隐式插入新的键值对，所以不支持
	 * 
	 * @note2	该操作复杂度为 O(1)
	 */
	inline const T operator[](int idx) const
	{
		if (idx < m_list.size() && idx >= 0)
		{
			return m_hash[m_list[idx]];
		}
		return T();
	}

	/**
	 * @brief 按键访问元素
	 *
	 * 根据指定的键返回对应的值。如果键不存在，则返回默认构造的 T 对象
	 *
	 * @param	key <Key>  键值
	 * @return	<const T> 如果键存在，返回对应的值；否则返回默认构造的 T 对象
	 * 
	 * @note1	暂不支持返回引用直接赋值, 没有对应场景。而且因为[]运算符有
	 *			可能会隐式插入新的键值对，所以不支持
	 * 
	 * @note2	该操作复杂度为 O(1)
	 */
	inline const T operator[](const Key& key) const
	{
		if (m_hash.contains(key))
		{
			return m_hash[key];
		}
		return T();
	}

	/**
	 * @brief	获取键值所在的索引位置
	 *
	 * 根据指定的键值返回对应索引位置。如果键值不存在, 返回 -1
	 *
	 * @param	key <Key> 键值
	 * @return	<int> 如果键值存在，返回对应的位置；否则返回 -1
	 * 
	 * @note	该操作复杂度为 O(N)
	 */
	inline int indexOf(const Key& key) const
	{
		return m_list.indexOf(key);
	}

	/**
	 * @brief 在指定索引位置插入一个键值对
	 *
	 * 如果键已存在，会先从列表中移除旧的键，然后在指定位置插入新的键值对
	 *
	 * @param idx <int>			插入位置的索引，必须在 [0, size()] 范围内
	 * @param key <Key>		    键值
	 * @param val <const T>		要插入的值
	 * 
	 * @note 该操作复杂度为 O(N), N为数组长度。QStringList插入和删除均是 O(N), 
	 *		 其结构大概率是 deque。QHash 查询修改均是 O(1)
	 */
	inline void insert(int idx, const Key& key, const T& val)
	{
		if (idx <= m_list.size() && idx >= 0)
		{
			if (m_hash.contains(key))
			{
				m_list.removeOne(key);
			}

			m_list.insert(idx, key);
			m_hash[key] = val;
		}		
	}

	/**
	 * @brief 在末尾追加一个键值对
	 *
	 * 等价于在索引为 size() 的位置插入键值对
	 *
	 * @param key <QString>		键值
	 * @param val <const T>		要追加的值
	 * 
	 * @note 该操作复杂度最差为 O(N), 同 insert
	 *		 但是对于新值的插入速度很快（不用移除+尾插入），为 O(1)
	 */
	inline void append(const QString& key, const T& val)
	{
		return insert(size(), key, val);
	}

	/**
	 * @brief 移除指定键的键值对
	 * @param key <QString> 要移除的键。
	 * 
	 * @note 该操作复杂度最差为 O(N), 同 insert
	 */
	void remove(const QString& key)
	{
		if (m_hash.contains(key))
		{
			m_hash.remove(key);
			m_list.removeOne(key);
		}
	}

	/**
	 * @brief 移除指定位置的键值对
	 * @param idx <int>	移除位置的索引，必须在 [0, size()) 范围内
	 *
	 * @note 该操作复杂度最差为 O(N), 同 remove
	 */
	void removeAt(int idx)
	{
		if (idx < m_list.size() && idx >= 0)
		{
			m_hash.remove(m_list[idx]);
			m_list.removeAt(idx);
		}
	}

	/**
	 * @class Iterator
	 * 
	 * @brief 定义了一个常规的迭代器，依插入顺序遍历数组
	 */
	class Iterator
	{
	public:
		Iterator(QLinkedHash* container, int index)
			: m_container(container), m_index(index) {}

		///< 解引用操作
		T& operator*() { return m_container->m_hash[m_container->m_list[m_index]]; }

		///< 前置递增
		Iterator& operator++()
		{
			++m_index;

			return *this;
		}

		///< 后置递增
		Iterator operator++(int)
		{
			Iterator temp = *this;
			++(*this);
			return temp;
		}

		///< 比较运算符
		bool operator==(const Iterator& other) const
		{
			return m_container == other.m_container && m_index == other.m_index;
		}

		bool operator!=(const Iterator& other) const
		{
			return !(*this == other);
		}

	private:
		QLinkedHash* m_container;
		int m_index;
	};

	Iterator begin() { return Iterator(this, 0); }
	Iterator end() { return Iterator(this, m_list.size()); }

private:
	QHash<Key, T>	m_hash;		///< 哈希表，用于存储键值对，提供快速查找功能
	QList<Key>		m_list;		///< 列表，用于存储键的顺序，提供按插入顺序访问的功能
};
