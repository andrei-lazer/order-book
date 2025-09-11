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
	LockFreeFIFO(size_t capacity);
	~LockFreeFIFO();
	size_t size() const;
	bool empty() const;
	bool full() const;
	bool push(const T& value);
	bool pop(T& value_to_replace);
};
