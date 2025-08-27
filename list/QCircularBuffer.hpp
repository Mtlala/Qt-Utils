/**
 * @file	QCircularBuffer.hpp
 * @brief	实现了一个基于QVector的环状模板缓存类
 *
 * @tparam T 存储的值的类型。
 */

#pragma once

#include <QVector>
#include <QList>

template<typename T>
class QCircularBuffer
{
public:
	explicit QCircularBuffer(size_t capacity = 0) 
		:m_buffer(capacity), m_start(0), m_size(0) {}

	QCircularBuffer(const QCircularBuffer& other)
		: m_buffer(other.m_buffer), m_start(other.m_start), m_size(other.m_size) {}
	~QCircularBuffer() { m_buffer.clear(); }

	/**
	 * @brief			获取容器中元素的数量
	 * @return <int>	容器中元素的数量
	 */
	size_t size() const { return m_size; }

	bool isEmpty() { return m_size == 0; }

	bool isFull() { return m_size == m_buffer.size(); }

	/**
	 * @brief	按下标索引访问元素
	 *
	 * 根据指定的索引返回对应的值。如果索引超出范围，则会丢出错误
	 *
	 * @param	idx <int> 索引值，必须在 [0, size()) 范围内
	 * @return	<const T> 如果索引有效，返回对应的值；
	 *
	 * @note2	该操作复杂度为 O(1)
	 */
	inline const T& operator[](size_t idx) const
	{
		if (idx >= m_size)
		{
			throw std::out_of_range("Index out of range");
		}
		return m_buffer[(m_start + idx) % m_buffer.size()];
	}

	/**
	 * @brief	按下标索引访问元素的非const引用版本
	 *
	 * 同 `const T& operator[]` 但该接口可以修改里面数据
	 *
	 * @param	idx <int> 索引值，必须在 [0, size()) 范围内
	 * @return	<const T> 如果索引有效，返回对应的引用
	 *
	 * @note	该操作复杂度为 O(1)
	 */
	inline T& operator[](size_t idx)
	{
		if (idx >= m_size)
		{
			throw std::out_of_range("Index out of range");
		}
		return m_buffer[(m_start + idx) % m_buffer.size()];
	}

	/**
	 * @brief	清空缓冲区
	 */
	void clear()
	{
		m_size = 0;
		m_start = 0;
	}

	/**
	 * @brief	移除缓冲区头部的元素
	 * 
	 * @note	该操作复杂度为 O(1)
	 */
	void remove_front()
	{
		if (isEmpty())
		{
			return;
		}
		m_start = (m_start + 1) % m_buffer.size();
		--m_size;
	}

	/**
	 * @brief	在缓冲区尾部插入元素
	 * @param	val <T> 要插入的元素,
	 * 
	 * @note1	如果缓冲区已满, 会自动覆盖缓冲区头部
	 * @note2	该操作复杂度为 O(1)
	 */
	void push_back(const T& val)
	{
		if (m_buffer.isEmpty())
		{
			return;
		}

		if (m_size < m_buffer.size()) 
		{
			m_buffer[(m_start + m_size) % m_buffer.size()] = val;
			++m_size;
		}
		else 
		{
			m_buffer[m_start] = val;
			m_start = (m_start + 1) % m_buffer.size();
		}
	}

	/**
	 * @brief	获取缓冲区头 n 个元素
	 * @param	n <size_t> 获取元素个数, 
	 *			若小于 0, 或不足 n 个, 则返回全部
	 */
	QList<T> front(size_t n = -1)
	{
		QList<T> _res;
		n = (n <= 0) ? m_size : std::min(n, m_size);

		for (auto _i = 0; _i < n; ++_i)
		{
			_res.append((*this)[_i]);
		}
		return _res;
	}

	/**
	 * @brief	获取缓冲区尾部 n 个元素
	 * @param	n <size_t> 获取元素个数,
	 *			若小于 0, 或不足 n 个, 则返回全部
	 */
	QList<T> back(size_t n = -1)
	{
		QList<T> _res;
		n = (n <= 0) ? m_size : std::min(n, m_size);

		for (auto _i = m_size - n; _i < m_size; ++_i)
		{
			_res.append((*this)[_i]);
		}
		return _res;
	}

private:
	QVector<T> m_buffer;

	size_t m_start, m_size;
};
