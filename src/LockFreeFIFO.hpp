#pragma once
#include <thread>

template <typename T>
class LockFreeFIFO
{
	size_t m_capacity;
	std::atomic<size_t> m_back = 0;
	std::atomic<size_t> m_front = 0;
	T* m_data;

	static_assert(std::atomic<size_t>::is_always_lock_free);
public:
	LockFreeFIFO(size_t capacity)
		: m_capacity(capacity)
		, m_data(new T[capacity]) {}

	size_t size() const
	{
		return m_front - m_back;
	}

	bool empty() const
	{
		return m_back == m_front;
	}

	bool full() const
	{
		if (empty())
		{
			return false;
		}
		return m_front % m_capacity == m_back;
	}
	
	bool push(const T& value)
	{
		if (full())
		{
			return false;
		}

		m_data[m_front % m_capacity] = value;
		++m_front;
		return true;
	}

	bool pop(T& value_to_replace)
	{
		if (empty())
		{
			return false;
		}

		value_to_replace = m_data[m_back % m_capacity];
		++m_back;
		return true;
	}
};
