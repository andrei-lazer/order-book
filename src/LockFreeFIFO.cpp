#include "Levels.hpp"
#include "LockFreeFIFO.hpp"
#include "Event.hpp"

template <typename T>
LockFreeFIFO<T>::LockFreeFIFO(size_t capacity)
	: m_capacity(capacity),
	  m_back(0),
	  m_front(0),
	  m_data(new T[capacity])
{}


template <typename T>
LockFreeFIFO<T>::~LockFreeFIFO()
{
	delete[] m_data;
}

template <typename T>
bool LockFreeFIFO<T>::empty() const
{
	return m_back == m_front;
}

template <typename T>
bool LockFreeFIFO<T>::full() const
{
	if (empty())
	{
		return false;
	}
	return m_front % m_capacity == m_back;
}


template <typename T>
bool LockFreeFIFO<T>::push(const T& value)
{
	if (full())
	{
		return false;
	}

	m_data[m_front % m_capacity] = value;
	++m_front;
	return true;
}

template <typename T>
bool LockFreeFIFO<T>::pop(T& value_to_replace)
{
	if (empty())
	{
		return false;
	}

	value_to_replace = m_data[m_back % m_capacity];
	++m_back;

	return true;
}

template class LockFreeFIFO<Event>;
template class LockFreeFIFO<int>;
